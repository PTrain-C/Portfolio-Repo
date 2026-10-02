// Builds a private-preview copy of the site that works from any folder:
// every page is its own .html file and every internal URL is relative.
//
//   node scripts/build-preview.mjs <out-dir>
//
// Output: <out-dir>/index.html (a small page that opens the site) and
// <out-dir>/site/** (the site itself).
import { execSync } from 'node:child_process';
import { cpSync, mkdirSync, readdirSync, readFileSync, rmSync, statSync, writeFileSync } from 'node:fs';
import { dirname, join, relative, sep } from 'node:path';

const BASE = '/Portfolio-Repo/';
const out = process.argv[2];
if (!out) throw new Error('Usage: node scripts/build-preview.mjs <out-dir>');

execSync('npx astro build', { stdio: 'inherit', env: { ...process.env, PREVIEW: 'true' } });

const src = 'dist-preview';
const site = join(out, 'site');
rmSync(out, { recursive: true, force: true });
mkdirSync(site, { recursive: true });
cpSync(src, site, { recursive: true });

const walk = (dir) =>
  readdirSync(dir).flatMap((f) => {
    const p = join(dir, f);
    return statSync(p).isDirectory() ? walk(p) : [p];
  });

// "/Portfolio-Repo/projects#x" -> "../projects.html#x", relative to `file`.
function rewrite(url, file) {
  const up = relative(dirname(file), site).split(sep).filter(Boolean).join('/');
  const prefix = up ? `${up}/` : '';
  const [path, hash] = url.slice(BASE.length).split('#');
  let target = path.replace(/\/$/, '');
  if (target === '') target = 'index.html';
  else if (!/\.[a-z0-9]+$/i.test(target)) target = `${target}.html`;
  return prefix + target + (hash !== undefined ? `#${hash}` : '');
}

const files = walk(site);
for (const file of files) {
  if (!/\.(html|css|js)$/.test(file)) continue;
  const text = readFileSync(file, 'utf8');
  const next = text.replace(/\/Portfolio-Repo\/[^"'()\s>]*/g, (m) => rewrite(m, file));
  if (next !== text) writeFileSync(file, next);
}

writeFileSync(
  join(out, 'index.html'),
  `<title>Peter Connolly Portfolio</title>
<style>
  body { background: #1e3d31; color: #f1efe6; font: 16px system-ui, sans-serif; }
  main { padding: 3rem 1.25rem; text-align: center; }
  a { color: #e3ad48; }
</style>
<main>
  <p>Opening the site preview…</p>
  <p><a href="site/index.html">Open it</a> if nothing happens.</p>
</main>
<script>location.replace('site/index.html');</script>
`,
);

console.log(`Preview written to ${out} (${files.length} site files)`);
