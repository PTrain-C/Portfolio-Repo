// @ts-check
import { defineConfig } from 'astro/config';
import mdx from '@astrojs/mdx';

// GitHub Pages serves this repo at https://ptrain-c.github.io/Portfolio-Repo/.
// If the site moves to a custom domain, Vercel, or Netlify, set `site` to the
// new origin and drop `base`.
export default defineConfig({
  site: 'https://ptrain-c.github.io',
  base: '/Portfolio-Repo',
  output: 'static',
  integrations: [mdx()],
});
