#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define V 10  //number of vertices
#define E 20  //number of (undirected) edges
#define MAX_WEIGHT 10   //random edge weights (1...10)
#define INF 999999  //a large 'infinite' distance

struct Edge {int u, v, w; };
int graph[V][V];    //adjacency matrix with 40000 possible entries
struct Edge edges[2*E]; //edge list with both directions

//--------------------------
//  Random Graph Generation
//--------------------------
void generateGraph(){
    for (int i=0;i<V;i++)
        for (int j=0;j<V;j++)
            graph[i][j] = (i==j) ? 0 : INF; //checking whether the i and j represents same node thus weight of 0
    
    for (int i=0; i<E;i++) {
        int u = rand() % V;
        int v = rand() % V;
        if (u==v) continue;  //Skip self loops
        int w = rand()%MAX_WEIGHT+1;
        graph[u][v] = w;    /*undirected = symmetric*/
        graph[v][u] = w;   

        edges[2*i] = (struct Edge){u,v,w};
        edges[2*i+1] = (struct Edge){v,u,w};
    }
}

//-----------------------
// Dijkstra (Adj Matrix)
//-----------------------
int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index= -1;
    for (int v=0; v<V; v++) {
        if (!sptSet[v] && dist[v]<=min) {
            min=dist[v], min_index=v;
        }
    }
    return min_index;
}

// Adding parent[] array to keep track of best shortest path
void dijkstra(int src, int parent[]) {
    int dist[V];
    bool sptSet[V];

    // initialize distances and visited set
    for (int i=0;i<V;i++) {
        dist[i]=INF, sptSet[i] = false;
        parent[i] = -1; // no parent yet
    }
    dist[src] = 0;

    // repeat V-1 times: pick closest unvisited u, then relax edges u->v
    for (int count=0; count<V-1; count++) {
        int u=minDistance(dist,sptSet);
        if (u == -1) break;
        sptSet[u]= true;

        //Relax edges from u to all v
        for (int v=0; v<V; v++) {
            if (!sptSet[v] && graph[u][v]!=INF &&
                dist[u]+graph[u][v]<dist[v]) {
                dist[v]=dist[u]+graph[u][v];
                parent[v] = u;  // store shortest path tree edge
            }
        } 
    }
}

void bellmanFord(int src) {
    int dist[V];
    for (int i=1;i<=V;i++) dist[i] = INF;
    dist[src]=0;

    // V-1 relaxation passes over all directed edges
    for (int i=1;i<=V-1;i++) {
        for (int j=0;j<2*E;j++) {
            int u=edges[j].u, v=edges[j].v, w=edges[j].w;
            if (dist[u]!=INF && dist[u]+w<dist[v])
            dist[v] = dist[u]+w;
        }
    }
}

void floydWarshall() {
    int dist[V][V];

    // copy adjacency matrix into dist
    for (int i=0;i<V;i++)
        for (int j=0;j<V;j++)
            dist[i][j]=graph[i][j];

    // dynamic programming: allow intermediate nodes up to k
    for (int k=0;k<V;k++)
        for (int i=0;i<V;i++)
            for (int j=0;j<V;j++)
                if (dist[i][k]+dist[k][j]<dist[i][j])
                    dist[i][j]= dist[i][k]+dist[k][j];
}

//Exporting the full graph to Graphviz to generate an image of the graph
void exportGraphToDOT(const char *filename) {
    FILE *f = fopen(filename, "w");
    if(!f) {
        printf("Error opening the file!\n");
        return;
    }

    fprintf(f, "graph G {\n");

    for (int u = 0; u<V; u++) {
        for (int v = u+1; v<V; v++) {  //avoiding duplicates in undirected graphs
            if (graph[u][v] != INF && graph[u][v] != 0) {
                fprintf(f, " %d -- %d [label=%d];\n", u, v, graph[u][v]);
            }
        }
    }

    fprintf(f, "}\n");
    fclose(f);
}

// Export SPT to Graphiz
void exportSPTToDOT(const char *filename, int parent[]) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(f, "graph SPT {\n");
    // show all nodes including isolated ones
    for (int i = 0; i < V; i++)
        fprintf(f, " %d;\n", i);

    for (int v = 0; v<V; v++) {
        if (parent[v] != -1) {
            // edge from parent[v] -> v with weight
            fprintf(f, " %d -- %d [label=%d, color=red, penwidth=2];\n", parent[v], v, graph[parent[v]][v]);
        }
    }

    // highlight source(0)
    fprintf(f, " 0 [style=filled, fillcolor=yellow];\n");

    fprintf(f, "}\n");
    fclose(f);
}

int main () {
    srand(time(NULL));
    generateGraph();

    clock_t start, end;
    int parent[V];

    start=clock();
    dijkstra(0, parent);
    end=clock();
    printf("C Dijkstra: %f s\n", (double)(end-start)/CLOCKS_PER_SEC);

    start=clock();
    bellmanFord(0);
    end=clock();
    printf("C Bellman-Ford: %f s\n", (double)(end-start)/CLOCKS_PER_SEC);

    start=clock();
    floydWarshall();
    end=clock();
    printf("C Floyd-Warshall: %f s\n", (double)(end-start)/CLOCKS_PER_SEC);

    //Exporting graphs
    exportGraphToDOT("graph.dot");
    exportSPTToDOT("spt.dot", parent);

    return 0;
}