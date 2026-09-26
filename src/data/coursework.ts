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
      { name: 'Semiconductor Devices' },
      { name: 'Physical Electronics' },
      { code: 'EE 477', name: 'MOS VLSI Circuit Design', note: 'Planned, Spring 2027' },
    ],
  },
  {
    group: 'Signals & Embedded',
    blurb: 'Signals, microcontrollers, and connected hardware.',
    courses: [
      { name: 'Signals and Systems' },
      { name: 'Embedded Systems' },
      { code: 'EE 250', name: 'Internet of Things', project: 'poke-the-poker' },
    ],
  },
  {
    group: 'Math & Physics',
    blurb: 'The foundations.',
    courses: [
      { name: 'Applied Linear Algebra' },
      { name: 'Differential Equations' },
      { name: 'Physics: Electricity & Magnetism' },
      { name: 'Physics: Optics & Modern Physics' },
    ],
  },
  {
    group: 'Outside class',
    blurb: 'Learning on my own time.',
    courses: [
      {
        name: 'IBM Quantum Qiskit Global Summer School',
        note: '2026. Quantum circuits, error correction, hardware-software interfaces',
      },
      { name: 'Digital logic self-study (HDLBits)', note: 'Jun 2026 to now. Verilog, simulated in Verilator' },
    ],
  },
];
