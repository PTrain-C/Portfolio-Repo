// Helpers for the generated circuit art. Everything is deterministic so the
// artwork is identical on every build.

/** Small seeded PRNG (mulberry32). */
export function rng(seed: number) {
  let a = seed >>> 0;
  return () => {
    a = (a + 0x6d2b79f5) >>> 0;
    let t = a;
    t = Math.imul(t ^ (t >>> 15), t | 1);
    t ^= t + Math.imul(t ^ (t >>> 7), t | 61);
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
  };
}

type Pt = [number, number];

/**
 * A PCB-style bus: `count` parallel traces that follow the same route with
 * mitred 45/90 degree corners. `moves` are relative [dx, dy] steps.
 * Returns one SVG path string per trace plus the end point of each.
 */
export function bus(start: Pt, moves: Pt[], count: number, pitch: number) {
  const pts: Pt[] = [start];
  for (const [dx, dy] of moves) {
    const [x, y] = pts[pts.length - 1];
    pts.push([x + dx, y + dy]);
  }
  const dirs = moves.map(([dx, dy]) => {
    const len = Math.hypot(dx, dy) || 1;
    return [dx / len, dy / len] as Pt;
  });
  const normal = ([dx, dy]: Pt): Pt => [-dy, dx];

  const traces: { d: string; end: Pt; start: Pt }[] = [];
  for (let k = 0; k < count; k++) {
    const off = (k - (count - 1) / 2) * pitch;
    const out: Pt[] = [];
    pts.forEach((p, i) => {
      let o: Pt;
      if (i === 0) o = normal(dirs[0]);
      else if (i === pts.length - 1) o = normal(dirs[dirs.length - 1]);
      else {
        const n1 = normal(dirs[i - 1]);
        const n2 = normal(dirs[i]);
        const dot = n1[0] * n2[0] + n1[1] * n2[1];
        o = [(n1[0] + n2[0]) / (1 + dot), (n1[1] + n2[1]) / (1 + dot)];
      }
      out.push([p[0] + o[0] * off, p[1] + o[1] * off]);
    });
    traces.push({
      d: 'M' + out.map(([x, y]) => `${x.toFixed(1)} ${y.toFixed(1)}`).join(' L'),
      start: out[0],
      end: out[out.length - 1],
    });
  }
  return traces;
}
