#include "wifi_config_fft.h"
#include "config_fft.h"

static int status = WL_IDLE_STATUS;

static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP);

void wifi_setup() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true) { delay(1000); }
  }

  // Single-arg begin for open networks (USC Guest Wireless is open)
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);
    delay(5000);
  }

  Serial.println("WiFi connected");
  printCurrentNet();
  printWifiData();

  timeClient.setTimeOffset(TIMEZONE_OFFSET_SEC);
  timeClient.begin();
}

void wifi_tick() {
  timeClient.update();
}

int get_current_time() {
  return timeClient.getHours() * 100 + timeClient.getMinutes();
}

void printWifiData() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  byte mac[6];
  WiFi.macAddress(mac);
}

void printCurrentNet() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}
