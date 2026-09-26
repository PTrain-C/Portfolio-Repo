// Site-wide settings. Anything set to `null` is still waiting on Peter and
// renders as a visible "pending" marker instead of a broken link.

export const site = {
  name: 'Peter Connolly',
  initials: 'PC',
  identity:
    'EE student at USC, focused on analog/mixed-signal circuits, PCB design, and quantum hardware.',
  // TODO(Peter): drop a headshot in /public (e.g. /public/headshot.jpg) and set
  // this to 'headshot.jpg'. Until then a placeholder is shown.
  headshot: null as string | null,
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

// Shown on the home page. The first four come from the brief's focus areas;
// the last is a placeholder for things outside engineering.
export const interests: { title: string; text: string; placeholder?: boolean }[] = [
  {
    title: 'Analog & mixed-signal',
    text: 'Circuits where the math on paper has to meet the behavior on the bench.',
  },
  {
    title: 'PCB design',
    text: 'Turning a schematic into a board someone can actually build.',
  },
  {
    title: 'Quantum hardware',
    text: 'The physical side of quantum computing, and the community around it.',
  },
  {
    title: 'Space systems',
    text: 'Hardware that has to survive vibration, heat, and cold before it ever flies.',
  },
  {
    title: 'Outside of engineering',
    text: 'Hobbies and interests go here.',
    placeholder: true,
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
