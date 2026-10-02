// @ts-check
import { defineConfig } from 'astro/config';
import mdx from '@astrojs/mdx';
import remarkMath from 'remark-math';
import rehypeKatex from 'rehype-katex';

// GitHub Pages serves this repo at https://ptrain-c.github.io/Portfolio-Repo/.
// If the site moves to a custom domain, Vercel, or Netlify, set `site` to the
// new origin and drop `base`.
// PREVIEW=true builds a copy for private previews (see scripts/build-preview.mjs):
// one .html file per page and no client-side router.
const preview = process.env.PREVIEW === 'true';

export default defineConfig({
  site: 'https://ptrain-c.github.io',
  base: '/Portfolio-Repo',
  output: 'static',
  outDir: preview ? './dist-preview' : './dist',
  build: { format: preview ? 'file' : 'directory' },
  integrations: [mdx()],
  markdown: {
    remarkPlugins: [remarkMath],
    rehypePlugins: [[rehypeKatex, { output: 'html' }]],
    shikiConfig: { theme: 'github-dark-dimmed' },
  },
});
