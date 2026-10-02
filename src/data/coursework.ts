// Coursework from Peter's resume. `code` only where it's confirmed; `project`
// links a class to the project that came out of it; `note` for anything
// planned or outside a regular class.

export type Course = { name: string; code?: string; project?: string; note?: string };

export const degree = {
  school: 'University of Southern California',
  title: 'B.S. Electrical and Computer Engineering',
  dates: 'Aug 2024 to May 2028',
  gpa: '3.54',
};

export const coursework: { group: string; blurb: string; courses: Course[] }[] = [
  {
    group: 'Circuits & Devices',
    blurb: 'Analog circuits and the physics under them.',
    courses: [
      { code: 'EE 202L', name: 'Linear Circuits', project: 'notch-filter' },
      { code: 'EE 338', name: 'Semiconductor Devices' },
      { code: 'EE 477', name: 'MOS VLSI Circuit Design', note: 'Planned, Spring 2027' },
    ],
  },
  {
    group: 'Signals & Embedded',
    blurb: 'Signals, microcontrollers, and connected hardware.',
    courses: [
      { code: 'EE 301', name: 'Signals and Systems' },
      { code: 'EE 109', name: 'Digital Logic & Embedded Systems' },
      { code: 'EE 250', name: 'Internet of Things', project: 'poke-the-poker' },
    ],
  },
  {
    group: 'Math & Physics',
    blurb: 'The foundations.',
    courses: [
      { code: 'EE 141', name: 'Applied Linear Algebra' },
      { code: 'MATH 245', name: 'Differential Equations' },
      { code: 'PHYS 152', name: 'Physics: Electricity & Magnetism' },
      { code: 'PHYS 153', name: 'Physics: Optics & Modern Physics' },
    ],
  },
  {
    group: 'Outside class',
    blurb: 'Learning on my own time.',
    courses: [
      {
        name: 'IBM Quantum Qiskit Global Summer School',
        note: '2026. Quantum Fundamentals certificate, including a dynamic GHZ-state circuit builder and a circuit parameter analysis utility',
      },
      { name: 'Verilog practice (HDLBits)', note: 'Jun 2026 to now. Combinational logic, sequential circuits, and FSMs' },
    ],
  },
];
