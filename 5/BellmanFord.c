#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100

typedef struct Node {
    int v, w;
    struct Node* next;
} Node;

Node* adj[MAX];
int V, E;

void addEdge(int u, int v, int w) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->v = v;
    newNode->w = w;
    newNode->next = adj[u];
    adj[u] = newNode;
}

void bellmanFord(int src) {
    int dist[MAX];

    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    // Relax edges V-1 times
    for (int i = 1; i <= V - 1; i++) {
        for (int u = 0; u < V; u++) {
            Node* temp = adj[u];
            while (temp) {
                int v = temp->v;
                int w = temp->w;
                if (dist[u] != INT_MAX && dist[u] + w < dist[v])
                    dist[v] = dist[u] + w;
                temp = temp->next;
            }
        }
    }

    // Check negative cycle
    for (int u = 0; u < V; u++) {
        Node* temp = adj[u];

        while (temp) {
            int v = temp->v;
            int w = temp->w;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                printf("\nError: Negative weight cycle detected!\n");
                return;
            }
            temp = temp->next;
        }
    }

    printf("\nShortest distances from %d:\n", src);
    for (int i = 0; i < V; i++)
        printf("%d -> %d = %d\n", src, i, dist[i]);
}

int main() {
    int u, v, w, src;
    printf("Enter vertices and edges:\n");
    scanf("%d %d", &V, &E);

    for (int i = 0; i < V; i++)
        adj[i] = NULL;
    printf("Enter edges (u v weight):\n");

    for (int i = 0; i < E; i++) {
        scanf("%d %d %d", &u, &v, &w);
        addEdge(u, v, w);
    }

    printf("Enter source: ");
    scanf("%d", &src);
    bellmanFord(src);
    return 0;
}