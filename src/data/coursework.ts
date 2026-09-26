// TODO(Peter): replace every placeholder with real courses. Set
// `placeholder: false` (or just delete the key) once an entry is real.

export type Course = { code: string; name: string; placeholder?: boolean };

export const coursework: { group: string; blurb: string; courses: Course[] }[] = [
  {
    group: 'Circuits & Devices',
    blurb: 'Analog, mixed-signal, and device physics.',
    courses: [
      { code: 'EE ___', name: 'Course name', placeholder: true },
      { code: 'EE ___', name: 'Course name', placeholder: true },
      { code: 'EE ___', name: 'Course name', placeholder: true },
    ],
  },
  {
    group: 'Signals & Systems',
    blurb: 'Linear systems, filtering, communications.',
    courses: [
      { code: 'EE ___', name: 'Course name', placeholder: true },
      { code: 'EE ___', name: 'Course name', placeholder: true },
    ],
  },
  {
    group: 'Digital & Computer Engineering',
    blurb: 'Logic design, architecture, embedded.',
    courses: [
      { code: 'EE ___', name: 'Course name', placeholder: true },
      { code: 'EE ___', name: 'Course name', placeholder: true },
    ],
  },
  {
    group: 'Quantum',
    blurb: 'Quantum information and hardware.',
    courses: [{ code: 'EE ___', name: 'Course name', placeholder: true }],
  },
];
