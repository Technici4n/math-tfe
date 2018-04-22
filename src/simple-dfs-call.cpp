// Pour chaque sommet $u$
for(int u = 0; u < N; ++u) {
	if(!visited[u]) {
		// Nouvelle composante connexe
		// donc on appelle l'algorithme.
		dfs(u);
	}
}