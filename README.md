# Portfolio

Peter's personal site. Astro, fully static, deploys to GitHub Pages.

## Run it

```sh
npm install
npm run dev        # http://localhost:4321/Portfolio-Repo/ (drafts visible)
npm run build      # production build, drafts hidden
SHOW_DRAFTS=true npm run build   # production build including drafts
```

## Where content lives

| What | File |
| --- | --- |
| Name, identity line, headshot, links | `src/config.ts` |
| Projects (one file each) | `src/content/projects/*.mdx` |
| Experience (one file each) | `src/content/experience/*.md` |
| Credentials | `src/content/credentials/*.md` |
| Skills | `src/data/skills.ts` |
| Coursework | `src/data/coursework.ts` |
| Home bio | `src/pages/index.astro` |

### Adding a project

Add a new `.mdx` file in `src/content/projects/`. Frontmatter:

- `mine`: exactly what Peter did. Keep it literal.
- `others`: `{ part, by }` for anything someone else owned. `by` can be a role ("A collaborator") instead of a name.
- `outcome`: where the project ended up (e.g. "Design and fabrication only").
- `status: draft` keeps it out of production builds. Switch to `published` when it's ready.

Use `<Missing>...</Missing>` for gaps and `<Figure src="..." caption="..." />` for photos (put images in `public/`).

## Deploying

`.github/workflows/deploy.yml` builds and deploys on every push to `main`. One-time setup: repo Settings → Pages → Source: **GitHub Actions**. If you move to a custom domain, Vercel, or Netlify, update `site` and remove `base` in `astro.config.mjs`.

## Still needed

- [ ] Full name, LinkedIn, GitHub, email (`src/config.ts`)
- [ ] Headshot
- [ ] Real bio (current one is a placeholder)
- [ ] QEE role dates
- [ ] BSPD: what it stands for, what it does, solo or team, photos
- [ ] Notch filter: what it fed into, target frequency, bench photos, name John or keep "a collaborator"
- [ ] FFTF: what it stands for, what it monitored/controlled, what the firmware ran on and who wrote it, team size, photos
- [ ] Poke the Poker: what it is, scope, role, photos
- [ ] Coursework list

## Deliberately left off

- Security clearance (resume only)
- QRNG/FPGA project (not until there's real progress)
- Any resume page or PDF
