#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define V 5000  //number of vertices
#define E 20000  //number of (undirected) edges
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
    int min = INT_MAX, min_index=0;
    for (int v=0; v<V; v++)
        if (!sptSet[v] && dist[v]<=min)
            min=dist[v], min_index=v;
    return min_index;
}

void dijkstra(int src) {
    int dist[V];
    bool sptSet[V];

    // initialize distances and visited set
    for (int i=0;i<V;i++) dist[i]=INF, sptSet[i] = false;
    dist[src] = 0;

    // repeat V-1 times: pick closest unvisited u, then relax edges u->v
    for (int count=0; count<V-1; count++) {
        int u=minDistance(dist,sptSet);
        sptSet[u]= true;

        //Relax edges from u to all v
        for (int v=0; v<V; v++)
            if (!sptSet[v] && graph[u][v]!=INF &&
                dist[u]+graph[u][v]<dist[v])
                dist[v]=dist[u]+graph[u][v];
    }
}

int main () {
    srand(time(NULL));
    generateGraph();

    clock_t start, end;

    start=clock();
    dijkstra(0);
    end=clock();
    printf("C Dijkstra: %f s\n", (double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}