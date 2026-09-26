import { getCollection } from 'astro:content';
import { showDrafts } from '../config';

export async function getProjects() {
  const all = await getCollection('projects');
  return all
    .filter((p) => showDrafts || p.data.status === 'published')
    .sort((a, b) => a.data.order - b.data.order);
}

export async function getExperience() {
  const all = await getCollection('experience');
  return all.sort((a, b) => a.data.order - b.data.order);
}

export async function getCredentials() {
  const all = await getCollection('credentials');
  return all.sort((a, b) => a.data.order - b.data.order);
}
