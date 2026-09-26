import { defineCollection } from 'astro:content';
import { glob } from 'astro/loaders';
import { z } from 'astro/zod';

const status = z.enum(['draft', 'published']).default('draft');

// One file per project. Adding a project is adding a file here, nothing else.
const projects = defineCollection({
  loader: glob({ pattern: '**/*.{md,mdx}', base: './src/content/projects' }),
  schema: z.object({
    title: z.string(),
    // Expanded name, if the title is an acronym.
    fullName: z.string().optional(),
    summary: z.string(),
    period: z.string().optional(),
    order: z.number().default(99),
    status,
    team: z.enum(['solo', 'team', 'unknown']).default('unknown'),
    // Exactly what Peter did. Keep these literal; never widen them.
    mine: z.array(z.string()).default([]),
    // Parts owned by someone else. `by` can be a name or a generic role.
    others: z.array(z.object({ part: z.string(), by: z.string() })).default([]),
    // Where the project ended up (e.g. "Design and fab only").
    outcome: z.string().optional(),
    // Skill ids from src/data/skills.ts. Only skills Peter used himself.
    skills: z.array(z.string()).default([]),
    // Image in /public, e.g. 'projects/notch-filter.jpg'. Replaces the art.
    cover: z.string().optional(),
    // Generated cover art used when there is no photo.
    art: z.enum(['pcb', 'bode', 'compare', 'gpio']).default('pcb'),
  }),
});

const experience = defineCollection({
  loader: glob({ pattern: '**/*.{md,mdx}', base: './src/content/experience' }),
  schema: z.object({
    org: z.string(),
    role: z.string(),
    unit: z.string().optional(),
    location: z.string().optional(),
    // Leave out if not confirmed yet; the page shows "Dates to come".
    start: z.string().optional(),
    end: z.string().optional(),
    current: z.boolean().default(false),
    order: z.number(),
    summary: z.string(),
    highlights: z.array(z.string()).default([]),
    // Skill ids from src/data/skills.ts.
    skills: z.array(z.string()).default([]),
  }),
});

const credentials = defineCollection({
  loader: glob({ pattern: '**/*.{md,mdx}', base: './src/content/credentials' }),
  schema: z.object({
    title: z.string(),
    issuer: z.string(),
    date: z.coerce.string(),
    order: z.number().default(99),
    skills: z.array(z.string()).default([]),
  }),
});

export const collections = { projects, experience, credentials };
