const int N; // nombre de sommets (numérotés de $0$ à $N-1$)

// Pour chaque sommet, on garde une liste des sommets adjacents
vector<int> adjacent_vertices[N];

// Au départ, aucun des sommets n'a été visité
bool visited[N] = {false};

void dfs(int u) {
	if(!visited[u]) { // Si $u$ n'a pas été visité
		visited[u] = true;
		// Pour chaque sommet $v$ adjacent
		for(int v : adjacent_vertices[u]) {
			dfs(v); // On continue récursivement
		}
		
		// Cataloguer $u$ d'une façon ou d'une autre
		/* ... */
	}
}