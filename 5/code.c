/*
 Experiment 5: Single-Source Shortest Path
 Implements Dijkstra (for non-negative edges) and Bellman-Ford (for graphs with possible negative edges).

Input (interactive):
 - First line: two integers: V E    (V = number of vertices, E = number of edges)
 - Next E lines: three integers each: u v w    (directed edge from u to v with weight w)
 - Last line: source vertex s


Vertices are numbered 0 .. V-1.

Behavior:
 - If any edge weight is negative, program runs Bellman-Ford from source.
 - Otherwise it runs Dijkstra (O(V^2) simple implementation).
 - Bellman-Ford detects negative-weight cycles reachable from the source and reports no solution.

Output:
 - For each vertex prints shortest distance from source (or "INF" if unreachable)
 - Also prints the path from source to that vertex when reachable.

Compile: gcc -std=c11 -O2 -Wall code.c -o exp5
Run: ./exp5
Example input:
 5 7
 0 1 6
 0 3 7
 1 2 5
 1 3 8
 1 4 -4
 2 1 -2
 3 2 -3
 0

This example contains negative edges; Bellman-Ford will be used.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Edge {
	int u, v;
	int w;
} Edge;

int min_index(int V, long long dist[], bool visited[]) {
	long long best = LLONG_MAX;
	int idx = -1;
	for (int i = 0; i < V; ++i) {
		if (!visited[i] && dist[i] < best) {
			best = dist[i];
			idx = i;
		}
	}
	return idx;
}

void print_path(int v, int parent[]) {
	if (v == -1) return;
	int stack[10000];
	int top = 0;
	int cur = v;
	while (cur != -1 && top < 10000) {
		stack[top++] = cur;
		cur = parent[cur];
	}
	for (int i = top - 1; i >= 0; --i) {
		printf("%d", stack[i]);
		if (i) printf(" -> ");
	}
}

void run_dijkstra(int V, Edge edges[], int E, int src) {
	// Build adjacency lists (linked lists) for directed graph
	typedef struct Node { int v; int w; struct Node* next; } Node;
	Node** adj = (Node**)calloc(V, sizeof(Node*));
	for (int i = 0; i < E; ++i) {
		Node* n = (Node*)malloc(sizeof(Node));
		n->v = edges[i].v;
		n->w = edges[i].w;
		n->next = adj[edges[i].u];
		adj[edges[i].u] = n;
	}

	long long *dist = (long long*)malloc(V * sizeof(long long));
	int *parent = (int*)malloc(V * sizeof(int));
	bool *visited = (bool*)malloc(V * sizeof(bool));
	for (int i = 0; i < V; ++i) {
		dist[i] = LLONG_MAX;
		parent[i] = -1;
		visited[i] = false;
	}
	dist[src] = 0;

	for (int count = 0; count < V; ++count) {
		int u = min_index(V, dist, visited);
		if (u == -1) break;
		visited[u] = true;
		for (Node* it = adj[u]; it != NULL; it = it->next) {
			int v = it->v;
			int w = it->w;
			if (!visited[v] && dist[u] != LLONG_MAX) {
				long long nd = dist[u] + (long long)w;
				if (nd < dist[v]) {
					dist[v] = nd;
					parent[v] = u;
				}
			}
		}
	}

	printf("\nUsing Dijkstra (non-negative weights)\n");
	printf("Vertex : Distance : Path\n");
	for (int i = 0; i < V; ++i) {
		printf("%3d : ", i);
		if (dist[i] == LLONG_MAX) {
			printf("INF\n");
		} else {
			printf("%lld : ", dist[i]);
			print_path(i, parent);
			printf("\n");
		}
	}

	// free
	for (int i = 0; i < V; ++i) {
		Node* cur = adj[i];
		while (cur) {
			Node* tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(adj);
	free(dist);
	free(parent);
	free(visited);
}

void run_bellman_ford(int V, Edge edges[], int E, int src) {
	long long *dist = (long long*)malloc(V * sizeof(long long));
	int *parent = (int*)malloc(V * sizeof(int));
	for (int i = 0; i < V; ++i) {
		dist[i] = LLONG_MAX;
		parent[i] = -1;
	}
	dist[src] = 0;

	for (int i = 0; i < V - 1; ++i) {
		bool changed = false;
		for (int j = 0; j < E; ++j) {
			int u = edges[j].u;
			int v = edges[j].v;
			int w = edges[j].w;
			if (dist[u] != LLONG_MAX && dist[u] + (long long)w < dist[v]) {
				dist[v] = dist[u] + (long long)w;
				parent[v] = u;
				changed = true;
			}
		}
		if (!changed) break;
	}

	// Check for negative-weight cycles reachable from source
	bool neg_cycle = false;
	for (int j = 0; j < E; ++j) {
		int u = edges[j].u;
		int v = edges[j].v;
		int w = edges[j].w;
		if (dist[u] != LLONG_MAX && dist[u] + (long long)w < dist[v]) {
			neg_cycle = true;
			break;
		}
	}

	if (neg_cycle) {
		printf("\nBellman-Ford detected a negative-weight cycle reachable from source. No solution.\n");
	} else {
		printf("\nUsing Bellman-Ford\n");
		printf("Vertex : Distance : Path\n");
		for (int i = 0; i < V; ++i) {
			printf("%3d : ", i);
			if (dist[i] == LLONG_MAX) {
				printf("INF\n");
			} else {
				printf("%lld : ", dist[i]);
				print_path(i, parent);
				printf("\n");
			}
		}
	}

	free(dist);
	free(parent);
}

int main(void) {
    int V, E;
    printf("Enter number of vertices and edges (V E): ");
    if (scanf("%d %d", &V, &E) != 2) {
        fprintf(stderr, "Expected: V E\n");
        return 1;
    }
    if (V <= 0 || E < 0) {
        fprintf(stderr, "Invalid V or E\n");
        return 1;
    }

    Edge *edges = (Edge*)malloc(E * sizeof(Edge));
    bool has_negative = false;
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        printf("Edge %d - Enter source, destination, weight (u v w): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Expected edge: u v w\n");
            free(edges);
            return 1;
        }
        if (u < 0 || u >= V || v < 0 || v >= V) {
            fprintf(stderr, "Vertex index out of range: %d -> %d\n", u, v);
            free(edges);
            return 1;
        }
        edges[i].u = u; edges[i].v = v; edges[i].w = w;
        if (w < 0) has_negative = true;
    }

    int src;
    printf("Enter source vertex (0 to %d): ", V - 1);
    if (scanf("%d", &src) != 1) {
        fprintf(stderr, "Expected source vertex\n");
        free(edges);
        return 1;
    }
    if (src < 0 || src >= V) {
        fprintf(stderr, "Source out of range\n");
        free(edges);
        return 1;
    }

    if (has_negative) {
        run_bellman_ford(V, edges, E, src);
    } else {
        run_dijkstra(V, edges, E, src);
    }

    free(edges);
    return 0;
}

