#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct edge
{
    struct vertex* dest;
    edge* eNext;
    int weight;
};

struct vertex
{
    char data;
    vertex* vNext;
    edge* aHead;
};

class Graph
{
    private:
        vertex* vertices;      // linked list of vertices
        int** adjMatrix;       // adjacency matrix
        bool* visited;         // visited array for traversals
        int maxVertices;
        int numVertices;

    public:
        Graph(int maxV);
        ~Graph();

        // Core operations
        void addVertex(char data);
        void addEdge(char src, char dest, int weight);
        void removeVertex(char data);
        void removeEdge(char src, char dest);

        // Traversal algorithms
        void bfsL(char start);
        void bfsM(char start);
        void dfsL(char start);
        void dfsM(char start);

        // Cycle detection
        bool detectCycle();

        // A6 Part 2 algorithms
        void dijkstra(char start);
        void kruskal();
        void prim(char start);

        // Display
        void displayList();
        void displayMatrix();
};

#endif