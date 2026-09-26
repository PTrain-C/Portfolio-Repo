export type Skill = { name: string; note?: string };

export const skills: { group: string; items: Skill[] }[] = [
  {
    group: 'EDA',
    items: [
      { name: 'KiCAD' },
      { name: 'Altium Designer', note: 'learning' },
      { name: 'LTSpice' },
    ],
  },
  {
    group: 'Programming',
    items: [{ name: 'Python' }, { name: 'C/C++' }, { name: 'MATLAB' }],
  },
  {
    group: 'Interfaces',
    items: ['MQTT', 'I2C', 'SPI', 'UART', 'CAN', 'GPIO', 'ADC'].map((name) => ({ name })),
  },
  { group: 'Quantum', items: [{ name: 'Qiskit' }] },
  {
    group: 'Requirements / Docs',
    items: [{ name: 'IBM DOORS/Jazz' }, { name: 'MIL-STD-681F' }],
  },
  {
    group: 'HDL',
    // Self-study only. Do not attach this to a project.
    items: [{ name: 'SystemVerilog', note: 'self-study, HDLBits' }],
  },
];
