# Graph-Algorithms-Playground
“A playground for graph algorithms — from shortest paths to future adventures in C &amp; Python.”

This repository contains implementations of shortest path algorithms in **C**.  
Currently includes **Dijkstra's algorithm** using an adjacency matrix, with random graph generation and runtime benchmarking.

## Features
- Random graph generator (configurable vertices and edges).
- Dijkstra’s algorithm (adjacency matrix implementation).
- Benchmarking with `clock()` to measure runtime.
- Easily extendable for other algorithms (Bellman-Ford, Floyd-Warshall, adjacency list, etc.).

## Build & Run
Compile with `gcc`:

```bash
gcc graph.c -o graph
./graph
```

Example output:
```
C Dijkstra: 0.194000 s
```

## Algorithm Explanation: Dijkstra’s Algorithm
Dijkstra’s algorithm is a **greedy shortest path algorithm**.  
It works by repeatedly selecting the unvisited vertex with the smallest known distance, then relaxing all of its outgoing edges.

- Time complexity (adjacency matrix): **O(V²)**
- Time complexity (adjacency list with min-heap): **O(E log V)**
- Works only with **non-negative edge weights**.

## Roadmap
- [ ] Add Bellman-Ford algorithm
- [ ] Add Floyd-Warshall algorithm
- [ ] Implement adjacency list version
- [ ] Graph visualization with Graphviz
- [ ] Compare performance across algorithms
