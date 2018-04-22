// Au départ, aucun des sommets n'a été visité (cc[$u$] = -1  $\ \forall u$)
int cc[N];

void dfs(int u, int id) {
	if(cc[u] == -1) { // Si $u$ n'a pas été assigné
		cc[u] = id;
		// Pour chaque sommet $v$ adjacent
		for(int v : adjacent_vertices[u]) {
			dfs(v, id); // On continue récursivement
		}
	}
}

// On initialise à $-1$
for(int u = 0; u < N; ++u) cc[u] = -1;

// Composante connexe actuelle
int id = 0;
// Pour chaque sommet $u$
for(int u = 0; u < N; ++u) {
	if(cc[u] == -1) {
		// Nouvelle composante connexe
		// donc on appelle l'algorithme.
		dfs(u, id++);
	}
}

