// Site-wide settings. Anything set to `null` is still waiting on Peter and
// renders as a visible "pending" marker instead of a broken link.

export const site = {
  // TODO(Peter): full name as it should appear on the site.
  name: 'Peter',
  initials: 'P',
  identity:
    'EE student at USC, focused on analog/mixed-signal circuits, PCB design, and quantum hardware.',
  // TODO(Peter): drop a headshot in /public (e.g. /public/headshot.jpg) and set
  // this to 'headshot.jpg'. Until then the initials avatar is shown.
  headshot: null as string | null,
};

export const links: { label: string; href: string | null }[] = [
  // TODO(Peter): real URLs.
  { label: 'LinkedIn', href: null },
  { label: 'GitHub', href: null },
  // TODO(Peter): use the form 'mailto:you@example.com'.
  { label: 'Email', href: null },
];

// Draft content (status: draft) is always visible in `npm run dev`.
// Production builds hide it unless SHOW_DRAFTS=true is set.
export const showDrafts =
  import.meta.env.DEV || process.env.SHOW_DRAFTS === 'true';

/** Prefix an internal path with the configured base (e.g. /Portfolio-Repo). */
export function href(path = '/'): string {
  const base = import.meta.env.BASE_URL.replace(/\/$/, '');
  const clean = path.startsWith('/') ? path : `/${path}`;
  return `${base}${clean}`;
}
