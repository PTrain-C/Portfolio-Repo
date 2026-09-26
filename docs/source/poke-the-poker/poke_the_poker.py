# ChatGPT helped with general debugging and lower level logic for the confidence and influence scores as well as the output MATRIX
# The high level logic, idea, MQTT, ADC and sensor readings, and most of the rest of the code is our own (with some borrowed from previous labs we completed).

import time
import RPi.GPIO as GPIO
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008
import paho.mqtt.client as mqtt

GPIO.setmode(GPIO.BOARD)
LED_PIN = 11
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.output(LED_PIN, GPIO.LOW)

SPI_PORT = 0
SPI_DEVICE = 0
mcp = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))

SOUND_CHANNEL = 1
SOUND_THRESHOLD = 200
DECISION_TIMEOUT = 10.0

INTER_PLAYER_DELAY = 5

MQTT_BROKER = "test.mosquitto.org"
MQTT_PORT = 1883
CONTROL_TOPIC = "pokethepoker"

player_count = 0            # defined by MQTT inputs
stop_flag = False           # MQTT "end" message sets True
player_confidences = {}     # based on time to make decision, 0 for timeout/fold
player_folded = {}          # True if player has folded

round_index = 0             # our cycle of active players round index

# each decision event:
# {'round': int, 'player': int, 'fold': bool, 'confidence': int}
decision_events = []

def led_on():
    GPIO.output(LED_PIN, GPIO.HIGH)

def led_off():
    GPIO.output(LED_PIN, GPIO.LOW)

def led_blink(duration=0.1):
    led_on()
    time.sleep(duration)
    led_off()

def countdown_between_players(seconds=INTER_PLAYER_DELAY):
    # prints cooldown timer between player turns
    # LED is solid during this countdown
    # stop_flag can interrupt

    global stop_flag
    total = int(seconds)

    led_on()

    for remaining in range(total, 0, -1):
        if stop_flag:
            led_off()
            return
        print(f"Next player's turn in {remaining} seconds...")
        time.sleep(1.0)

    led_off()

def wait_for_decision(timeout_s = DECISION_TIMEOUT, threshold = SOUND_THRESHOLD, samples_above_threshold = 3):
    """
    waits until either:
      - sound above threshold is detected (returns decision_time in seconds), or
      - timeout expires (returns None), or
      - stop_flag is set (returns None).
    """
    global stop_flag

    print("Waiting for player decision...")
    start_time = time.time()
    consecutive = 0

    while True:
        if stop_flag:
            print("Stop flag received during decision wait")
            return None

        now = time.time()
        if now - start_time > timeout_s:
            print("Timeout - no sound detected")
            return None

        sound_value = mcp.read_adc(SOUND_CHANNEL)
        print(f"Sound Sensor Value: {sound_value}")

        if sound_value > threshold:
            consecutive += 1
        else:
            consecutive = 0

        if consecutive >= samples_above_threshold:
            decision_time = now - start_time
            print(f"Sound detected after {decision_time:.3f} s")
            return decision_time

        time.sleep(0.01)  # 10 ms between samples

def time_to_confidence(decision_time, max_time=DECISION_TIMEOUT):

    #decision_time = 0 -> 100 %
    #no decision (None) -> 0 %

    if decision_time is None:
        return 0

    clamped = max(0.0, min(decision_time, max_time))
    confidence = (max_time - clamped) / max_time * 100.0
    return int(confidence)

def summarize_confidences():

    #Print each player's per round confidence and average for whole game, plus overall average across all players.

    if not player_confidences:
        print("\nNo confidence data collected.")
        return

    print("\n========== Game summary ==========")
    all_scores = []

    for player_id, scores in player_confidences.items():
        if scores:
            avg = sum(scores) / len(scores)
            all_scores.extend(scores)
        else:
            avg = 0.0

        folded_str = " (folded)" if player_folded.get(player_id, False) else ""
        print(f"Player {player_id}{folded_str}: {scores}  ->  average {avg:.1f} %")

    if all_scores:
        overall_avg = sum(all_scores) / len(all_scores)
        print(f"\nOverall average confidence: {overall_avg:.1f} %")
    print("==================================\n")

def count_active_players():
    
    # just to find how many have folded, helps with deciding when game ends automatically
    
    active = 0
    for pid in range(1, player_count + 1):
        if not player_folded.get(pid, False):
            active += 1
    return active

def print_influence_table():
    # Fully explained in documentation
    global decision_events, player_count

    if not decision_events:
        print("\nNo influence data collected.")
        return

    max_round = max(ev["round"] for ev in decision_events)

    successes = {pid: [0] * (max_round + 1) for pid in range(1, player_count + 1)}
    opportunities = {pid: [0] * (max_round + 1) for pid in range(1, player_count + 1)}

    last_agg_player = None
    last_agg_round = None

    for ev in decision_events:
        pid = ev["player"]
        r = ev["round"]
        fold = ev["fold"]

        # If someone else acts while there is an aggressor,
        # that is an opportunity under the aggressor.
        if last_agg_player is not None and pid != last_agg_player:
            opportunities[last_agg_player][last_agg_round] += 1
            if fold:
                successes[last_agg_player][last_agg_round] += 1

        # Non-folding decision becomes new aggressor
        if not fold:
            last_agg_player = pid
            last_agg_round = r
        # If fold == True, we keep the same aggressor

    scores = {pid: [0.0] * (max_round + 1) for pid in range(1, player_count + 1)}
    for pid in range(1, player_count + 1):
        for r in range(1, max_round + 1):
            opp = opportunities[pid][r]
            if opp > 0:
                scores[pid][r] = successes[pid][r] / opp
            else:
                scores[pid][r] = 0.0

    print("\n========== Influence score (next-player fold fraction) ==========")
    header = "Player/Round".ljust(12)
    for r in range(1, max_round + 1):
        header += f"R{r:02d}".rjust(8)
    print(header)

    for pid in range(1, player_count + 1):
        row = f"P{pid}".ljust(12)
        vals = scores[pid]
        for r in range(1, max_round + 1):
            v = vals[r]
            cell = f"{v: .3f}"
            row += cell.rjust(8)
        print(row)

    print("===============================================================\n")

def print_target_influence_coverage():
    # Fully explained in documentation
    # ChatGPT helped us with the logic for influence
    global decision_events, player_count

    if not decision_events:
        print("\nNo target-based influence data collected.")
        return

    possible_targets = {pid: set() for pid in range(1, player_count + 1)}
    influenced_targets = {pid: set() for pid in range(1, player_count + 1)}

    last_agg_player = None

    for ev in decision_events:
        pid = ev["player"]
        fold = ev["fold"]

        if last_agg_player is not None and pid != last_agg_player:
            possible_targets[last_agg_player].add(pid)
            if fold:
                influenced_targets[last_agg_player].add(pid)

        if not fold:
            last_agg_player = pid
        # if fold, aggressor stays the same

    print("\n========== Target-based influence coverage ==========")
    for pid in range(1, player_count + 1):
        total = len(possible_targets[pid])
        hits = len(influenced_targets[pid])
        if total > 0:
            score = hits / total
        else:
            score = 0.0

        print(
            f"Player {pid}: influenced {hits}/{total} distinct targets"
            f" -> score {score:.2f} ({score*100:.1f} %)"
        )
    print("=====================================================\n")

def print_export_matrix():
    # ChatGPT also helped with creating this matrix
    """
    Copy MATRIX from ssh'd terminal, paste into plot_poke.py then save, run plot_poke.py on control terminal, yay graphs

    # MATRIX row format:
    # [0] event_index
    # [1] round_of_influencer
    # [2] influencer_player
    # [3] influencer_confidence
    # [4] influencer_fold_flag (1 if folded, 0 if stayed)
    # [5] next_round
    # [6] next_player
    # [7] next_confidence
    # [8] next_fold_flag (1 if folded, 0 if stayed)
    """
    global decision_events

    if len(decision_events) < 2:
        print("\nNot enough decision events for export matrix.")
        return

    print("\n========== Export matrix (copy-paste to laptop) ==========")
    print("MATRIX = [")
    for idx in range(len(decision_events) - 1):  # last event has no next
        curr = decision_events[idx]
        nxt = decision_events[idx + 1]

        row = [
            idx,
            curr["round"],
            curr["player"],
            curr["confidence"],
            1 if curr["fold"] else 0,
            nxt["round"],
            nxt["player"],
            nxt["confidence"],
            1 if nxt["fold"] else 0,
        ]
        print(f"  {row},")
    print("]")
    print("==========================================================\n")

def on_connect(client, userdata, flags, rc):
    print(f"Connected to MQTT broker with result code {rc}")
    client.subscribe(CONTROL_TOPIC)
    print(f"Subscribed to topic: {CONTROL_TOPIC}")

def on_message(client, userdata, msg):
    global player_count, stop_flag
    payload = msg.payload.decode().strip()
    print(f"MQTT message on {msg.topic}: '{payload}'")

    if payload.lower() == "end":
        print("Received 'end' command - stopping main loop")
        stop_flag = True
        return

    try:
        value = int(payload)
        if value >= 0:
            player_count = value
            print(f"Player count set to: {player_count}")
        else:
            print("Ignoring negative player count")
    except ValueError:
        print("Ignoring non-integer, non-'end' control message")

def setup_mqtt():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
    client.loop_start()
    return client

def main():
    global stop_flag, player_count, player_confidences, player_folded
    global round_index, decision_events

    mqtt_client = setup_mqtt()

    try:
        print("Waiting for player count via MQTT...")
        while not stop_flag and player_count <= 0:
            time.sleep(0.1)

        if stop_flag:
            print("Stop flag received before start, exiting")
            return

        player_confidences = {}
        player_folded = {}
        for pid in range(1, player_count + 1):
            player_confidences[pid] = []
            player_folded[pid] = False

        round_index = 0
        decision_events = []

        print(f"Starting main loop with player_count = {player_count}")

        while not stop_flag:
            round_index += 1
            print(f"\n===== Starting round {round_index} =====")

            for player_id in range(1, player_count + 1):
                if stop_flag:
                    break

                if player_folded.get(player_id, False):
                    print(f"\nPlayer {player_id} has folded earlier. Skipping.")
                    continue

                print(f"\nPlayer {player_id}'s turn")
                led_off()

                decision_time = wait_for_decision()

                if stop_flag:
                    break

                if decision_time is None:
                    confidence = 0
                    player_folded[player_id] = True
                    folded_this_turn = True
                    print(f"Player {player_id} has folded (timeout). "
                          f"Confidence this round: {confidence} %")
                else:
                    confidence = time_to_confidence(decision_time)
                    folded_this_turn = False
                    print(f"Player {player_id} confidence: {confidence} %")
                    led_blink(0.2)

                player_confidences[player_id].append(confidence)

                decision_events.append({
                    "round": round_index,
                    "player": player_id,
                    "fold": folded_this_turn,
                    "confidence": confidence,
                })

                active_remaining = count_active_players()
                if active_remaining <= 1:
                    print("\nOnly one player remaining. Ending game.")
                    stop_flag = True
                    break

                countdown_between_players(INTER_PLAYER_DELAY)

            if stop_flag:
                break

            print("\nRound complete. Waiting for more commands...")
            led_on()
            time.sleep(1.0)
            led_off()

    except KeyboardInterrupt:
        print("\nKeyboardInterrupt, exiting cleanly")

    finally:
        summarize_confidences()
        print_influence_table()
        print_target_influence_coverage()
        print_export_matrix()
        stop_flag = True
        mqtt_client.loop_stop()
        mqtt_client.disconnect()
        led_off()
        GPIO.cleanup()

if __name__ == "__main__":
    main()