# Portfolio

Peter's personal site. Astro, fully static, deploys to GitHub Pages.

## Run it

```sh
npm install
npm run dev        # http://localhost:4321/Portfolio-Repo/
npm run build      # production build into dist/
HIDE_DRAFTS=true npm run build   # leave out anything marked status: draft
```

## Where content lives

| What | File |
| --- | --- |
| Name, identity line, headshot, links | `src/config.ts` |
| Projects (one file each) | `src/content/projects/*.mdx` |
| Experience (one file each) | `src/content/experience/*.mdx` |
| Credentials | `src/content/credentials/*.md` |
| Skill categories (and which are "key skills" on Home) | `src/data/skills.ts` |
| Interests | `src/config.ts` |
| Coursework | `src/data/coursework.ts` |
| Home bio | `src/pages/index.astro` |
| Images shown on the site | `public/img/<project>/` |
| Raw source docs, reports, and code (not published) | `docs/source/<project>/` |

Put raw uploads (PDFs, code, original photos) in `docs/source/`, never in `src/content/`: every `.md` in the content folders is treated as a page. Web-ready images go in `public/img/`.

### Adding a project

Add a new `.mdx` file in `src/content/projects/`. Frontmatter:

- `mine`: exactly what Peter did. Keep it literal.
- `others`: `{ part, by }` for anything someone else owned.
- `outcome`: where the project ended up.
- `skills`: skill ids from `src/data/skills.ts`. An unknown id fails the build.
- `theme`: the page's look. One of `board`, `track` (racing), `cigar` (guitar), `felt` (poker), `hydro`, `bench` (breadboard), `space`, `naval`, `quantum`.
- `hero`: how the top is arranged: `split`, `poster` (image fills the right), `tall` (portrait photo that hangs into the page), or `scene` (just the theme art).
- `rail`: where "My part / Skills" sits: `right`, `left`, or `top`.
- `cover` (card image) and `heroImage` (top of page), both paths inside `public/`.
- `experience`: id of the role it came from; `links`; `sources` (documents the claims come from).
- `status: draft` marks it unfinished.

Experience entries use `summary`, `highlights`, `skills`, `theme`, `rail`, `projects` (related project ids), and `sources`.

Components for MDX bodies live in `src/components/content/`: `Chain` (clickable signal chain or state machine), `Tabs` (image viewer), `NotchExplorer`, `MeasuredBode`, `Excerpt` (shows real lines from a file in `docs/source`), `Stats`, `Pair`, `TeamDots`, `ThermalProfile`. Also `<Missing>` for gaps, `<Figure>` for photos, and `$...$` / `$$...$$` for math.

## Deploying

`.github/workflows/deploy.yml` builds and deploys on every push to `main`. One-time setup: repo Settings → Pages → Source: **GitHub Actions**. If you move to a custom domain, Vercel, or Netlify, update `site` and remove `base` in `astro.config.mjs`.

## Still needed

- [ ] Headshot
- [ ] GitHub link, if you want one shown (`src/config.ts`)
- [ ] Hobbies for the interests section
- [ ] Circuit Sensei and Poke the Poker demo screenshots
- [ ] Food For Thought photos
- [ ] Poke the Poker: confirm the split with Christopher
- [ ] BSPD: a line on the layout choices

## Contact form

`/contact` posts to FormSubmit (`contact.formEndpoint` in `src/config.ts`), which forwards each message to the email in `contact.email`. The first message ever sent triggers a one-time activation email from FormSubmit to that inbox; click the link in it once and every message after that is delivered. If the form fails, the page offers a mailto link with the message filled in.

## Deliberately left off

- Security clearance and phone number (resume only)
- QRNG/FPGA project (not until there's real progress)
- Any resume page or PDF
