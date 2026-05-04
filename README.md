# A6-part-2
assignment 6 part 2.

A6_P2 - Shortest Path and MST Algorithms

Name: Abraham Rios
Course: CSC 300
Assignment: Assignment 6 Part 2


This program creates an undirected weighted graph using an adjacency list and an adjacency matrix. It tests the graph using BFS, DFS, cycle detection, Dijkstra's shortest path algorithm, Kruskal's minimum spanning tree algorithm, and Prim's minimum spanning tree algorithm.

Files Included:
graph.h
graph.cpp
mainGraph.cpp
README.txt
Task4.pdf
screenshot.png

Program Output:
The program prints the original adjacency list and adjacency matrix. It then runs BFS and DFS starting from vertex A. After that, it checks if the graph has a cycle. Finally, it runs the three required A6 Part 2 algorithms:

1. Dijkstra's algorithm starting from vertex A
2. Kruskal's minimum spanning tree algorithm
3. Prim's minimum spanning tree algorithm starting from vertex A

Notes:
Dijkstra's algorithm uses a min heap priority queue.
Kruskal's algorithm uses union-find.
Prim's algorithm uses a min heap priority queue.

The graph uses vertices A through J and weighted undirected edges.