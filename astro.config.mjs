// @ts-check
import { defineConfig } from 'astro/config';
import mdx from '@astrojs/mdx';
import remarkMath from 'remark-math';
import rehypeKatex from 'rehype-katex';

// The repo is named ptrain-c.github.io, so GitHub Pages serves it at the root:
// https://ptrain-c.github.io/. If the site moves to a custom domain, Vercel, or
// Netlify, just change `site`.
// PREVIEW=true builds a copy for private previews (see scripts/build-preview.mjs):
// one .html file per page and no client-side router. That script rewrites the
// /Portfolio-Repo/ prefix into relative links, so the preview keeps it as its base.
const preview = process.env.PREVIEW === 'true';

export default defineConfig({
  site: 'https://ptrain-c.github.io',
  base: preview ? '/Portfolio-Repo' : '/',
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
