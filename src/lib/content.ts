import { getCollection } from 'astro:content';
import { showDrafts } from '../config';
import { categories, lookupSkill } from '../data/skills';

export async function getProjects() {
  const all = await getCollection('projects');
  all.forEach((p) => p.data.skills.forEach(lookupSkill));
  return all
    .filter((p) => showDrafts || p.data.status === 'published')
    .sort((a, b) => a.data.order - b.data.order);
}

export async function getExperience() {
  const all = await getCollection('experience');
  all.forEach((e) => e.data.skills.forEach(lookupSkill));
  return all.sort((a, b) => a.data.order - b.data.order);
}

export async function getCredentials() {
  const all = await getCollection('credentials');
  all.forEach((c) => c.data.skills.forEach(lookupSkill));
  return all.sort((a, b) => a.data.order - b.data.order);
}

export type Usage = { label: string; kind: 'Project' | 'Experience' | 'Credential'; path?: string };

/** For every skill id, where it shows up across the site. */
export async function getSkillUsage() {
  const [projects, experience, credentials] = await Promise.all([
    getProjects(),
    getExperience(),
    getCredentials(),
  ]);
  const usage = new Map<string, Usage[]>();
  for (const c of categories) for (const s of c.skills) usage.set(s.id, []);

  for (const e of experience)
    for (const id of e.data.skills)
      usage.get(id)!.push({ label: e.data.org, kind: 'Experience', path: `/experience/${e.id}` });
  for (const p of projects)
    for (const id of p.data.skills)
      usage.get(id)!.push({ label: p.data.title, kind: 'Project', path: `/projects/${p.id}` });
  for (const c of credentials)
    for (const id of c.data.skills)
      usage.get(id)!.push({ label: c.data.issuer, kind: 'Credential' });

  return usage;
}
