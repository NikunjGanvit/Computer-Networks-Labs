#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<string.h>


#define MAX_NODES 10  // Maximum number of nodes in the graph


// Structure to represent a node's adjacency list (for linked neighbors)
struct Edge {
   int destination;
   int cost;
   struct Edge* next;
};


// Function to find the node with the minimum distance that is not yet visited
int minDistance(int dist[], int visited[], int V) {
   int min = INT_MAX, min_index;
   for (int v = 0; v < V; v++) {
       if (!visited[v] && dist[v] <= min) {
           min = dist[v];
           min_index = v;
       }
   }
   return min_index;
}


// Function to implement Dijkstra's algorithm and print the routing table
void dijkstra(struct Edge* graph[], int start, int V) {
   int dist[V];            // Stores the shortest distance from the start node
   int visited[V];         // Marks visited nodes
   int parent[V];          // Stores the predecessor of each node for path reconstruction


   // Initialize distances, visited, and parent arrays
   for (int i = 0; i < V; i++) {
       dist[i] = INT_MAX;
       visited[i] = 0;
       parent[i] = -1;
   }


   dist[start] = 0;


   // Dijkstra's algorithm loop
   for (int count = 0; count < V - 1; count++) {
       int u = minDistance(dist, visited, V);  // Get the node with the minimum distance
       visited[u] = 1;  // Mark the node as visited


       // Update the distance of adjacent nodes
       struct Edge* temp = graph[u];
       while (temp != NULL) {
           int v = temp->destination;
           int weight = temp->cost;
           if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
               dist[v] = dist[u] + weight;
               parent[v] = u;
           }
           temp = temp->next;
       }
   }


   // Print the routing table
   printf("\nRouting Table:\n");
   printf("Destination | Next Hop | Path | Cost\n");
   printf("---------------------------------------\n");


   for (int i = 0; i < V; i++) {
       if (i == start) {
           continue; // Skip the start node
       }


       // Reconstruct the path
       int nextHop = i;
       while (parent[nextHop] != start && parent[nextHop] != -1) {
           nextHop = parent[nextHop];
       }


       // Display destination, next hop, path, and cost
       char pathStr[256] = "";
       snprintf(pathStr, sizeof(pathStr), "%d <- %d", i, start);
       for (int temp = parent[i]; temp != -1; temp = parent[temp]) {
           char tempStr[256];
           snprintf(tempStr, sizeof(tempStr), " <- %d", temp);
           strcat(pathStr, tempStr);
       }


       printf("%d \t\t %d \t\t %s \t %d\n", i, nextHop, pathStr, dist[i]);
   }
}


// Function to add an edge to the graph
void addEdge(struct Edge* graph[], int src, int dest, int cost) {
   struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
   newEdge->destination = dest;
   newEdge->cost = cost;
   newEdge->next = graph[src];
   graph[src] = newEdge;
}


int main() {
   int V, start;


   printf("Enter the number of nodes (vertices) in the graph: ");
   scanf("%d", &V);


   // Initialize the graph with NULL (no edges initially)
   struct Edge* graph[MAX_NODES] = {NULL};


   printf("Enter the start node (0 to %d): ", V - 1);
   scanf("%d", &start);


   // Input the edges for each node
   for (int i = 0; i < V; i++) {
       int numNeighbors;
       printf("\nEnter the number of neighbors for node %d: ", i);
       scanf("%d", &numNeighbors);
      
       for (int j = 0; j < numNeighbors; j++) {
           int neighbor, cost;
           printf("Enter neighbor node and cost for edge %d -> ", i);
           scanf("%d %d", &neighbor, &cost);
           addEdge(graph, i, neighbor, cost); // Add the edge from node i to the neighbor
       }
   }


   // Run Dijkstra's algorithm
   dijkstra(graph, start, V);


   return 0;
}
