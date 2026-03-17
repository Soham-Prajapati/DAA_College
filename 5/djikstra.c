#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100

typedef struct Node
{
    int v, w;
    struct Node *next;
} Node;

Node *adj[MAX];
int V, E;

// Add edge
void addEdge(int u, int v, int w)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->v = v;
    newNode->w = w;
    newNode->next = adj[u];
    adj[u] = newNode;
}

void dijkstra(int src)
{
    int dist[MAX], visited[MAX] = {0};
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;
    for (int i = 0; i < V; i++)
    {
        int u = -1, min = INT_MAX;
        // Find min distance node
        for (int j = 0; j < V; j++)
        {
            if (!visited[j] && dist[j] < min)
            {
                min = dist[j];
                u = j;
            }
        }
        if (u == -1)
            break;
        visited[u] = 1;

        // Traverse adjacency list
        Node *temp = adj[u];
        while (temp)
        {
            int v = temp->v;
            int w = temp->w;
            if (dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
            temp = temp->next;
        }
    }
    printf("\nShortest distances from %d:\n", src);
    for (int i = 0; i < V; i++)
        printf("%d -> %d = %d\n", src, i, dist[i]);
}

int main()
{
    int u, v, w, src;
    printf("Enter vertices and edges:\n");
    scanf("%d %d", &V, &E);

    // Initialize
    for (int i = 0; i < V; i++)
        adj[i] = NULL;

    printf("Enter edges (u v weight):\n");

    for (int i = 0; i < E; i++)
    {
        scanf("%d %d %d", &u, &v, &w);

        //  Error detection
        if (w < 0)
        {
            printf("Error: Negative edge detected. Dijkstra cannot be applied.\n");
            return 0;
        }
        addEdge(u, v, w);
    }

    printf("Enter source: ");
    scanf("%d", &src);
    dijkstra(src);
    return 0;
}