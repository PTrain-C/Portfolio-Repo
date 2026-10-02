// Site-wide settings. Anything set to `null` is still waiting on Peter and
// renders as a visible "pending" marker instead of a broken link.

export const site = {
  name: 'Peter Connolly',
  initials: 'PC',
  identity:
    'Electrical engineer working toward analog and board-level design. Requirements at Lockheed Martin, flight hardware test at Draper.',
  // File in /public. Currently the LinkedIn photo; swap in a new one any time.
  headshot: 'headshot.webp' as string | null,
};

export const contact = {
  email: 'peter.mai.connolly@gmail.com',
  linkedin: 'https://www.linkedin.com/in/peter-mai-connolly',
  // TODO(Peter): GitHub profile URL, if you want it shown.
  github: null as string | null,
  // The contact form posts here. FormSubmit forwards each message to `email`.
  // The very first submission sends Peter a one-time activation email; click
  // the link in it and every message after that is delivered.
  formEndpoint: 'https://formsubmit.co/ajax/peter.mai.connolly@gmail.com',
};

// HubSpot tracking code ID (HubSpot > Settings > Tracking & Analytics >
// Tracking Code; it is the number in js.hs-scripts.com/<id>.js). While null,
// no tracking script is added to the site.
export const hubspotPortalId: string | null = null;

// Shown on the home page.
export const interests: { title: string; text: string; placeholder?: boolean }[] = [
  {
    title: 'Analog front ends',
    text: 'Low-noise power and signal chains for quantum control systems.',
  },
  {
    title: 'Photonic I/O',
    text: 'Co-packaged optics, where the electronics are modulator drivers, transimpedance amplifiers, and bias and thermal control loops.',
  },
  {
    title: 'Cryogenic readout ICs',
    text: 'The chips that read out quantum hardware at very low temperatures.',
  },
  {
    title: 'Trusted microelectronics',
    text: 'Chip design and validation for defense and space.',
  },
];

// Draft content (status: draft) is shown everywhere for now. Set
// HIDE_DRAFTS=true at build time to leave drafts out.
export const showDrafts = process.env.HIDE_DRAFTS !== 'true';

/** Prefix an internal path with the configured base (e.g. /Portfolio-Repo). */
export function href(path = '/'): string {
  const base = import.meta.env.BASE_URL.replace(/\/$/, '');
  const clean = path.startsWith('/') ? path : `/${path}`;
  return `${base}${clean}`;
}
