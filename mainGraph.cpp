#include "graph.h"

int main()
{
    Graph g(10);

    // Add vertices
    g.addVertex('A');
    g.addVertex('B');
    g.addVertex('C');
    g.addVertex('D');
    g.addVertex('E');
    g.addVertex('F');
    g.addVertex('G');
    g.addVertex('H');
    g.addVertex('I');
    g.addVertex('J');

    // Add edges
    g.addEdge('A', 'B', 1);
    g.addEdge('A', 'C', 2);
    g.addEdge('B', 'D', 3);
    g.addEdge('C', 'E', 4);
    g.addEdge('C', 'D', 5);
    g.addEdge('E', 'D', 6);
    g.addEdge('A', 'H', 7);
    g.addEdge('B', 'G', 8);
    g.addEdge('F', 'I', 9);
    g.addEdge('F', 'J', 10);
    g.addEdge('I', 'J', 11);
    g.addEdge('C', 'F', 12);
    g.addEdge('D', 'F', 13);
    g.addEdge('C', 'H', 14);

    cout << "Original Adjacency List:" << endl;
    g.displayList();

    cout << "Original Adjacency Matrix:" << endl;
    g.displayMatrix();

    cout << endl;

    // Test BFS and DFS
    cout << "Traversal Tests:" << endl;
    g.bfsL('A');
    g.bfsM('A');
    g.dfsL('A');
    g.dfsM('A');

    cout << endl;

    // Test cycle detection
    cout << "Cycle Test:" << endl;

    if(g.detectCycle())
    {
        cout << "Cycle detected" << endl;
    }
    else
    {
        cout << "No cycle detected" << endl;
    }

    cout << endl;

    // A6 Part 2 Task 1
    cout << "Dijkstra Test:" << endl;
    g.dijkstra('A');

    // A6 Part 2 Task 2
    cout << "Kruskal Test:" << endl;
    g.kruskal();

    // A6 Part 2 Task 3
    cout << "Prim Test:" << endl;
    g.prim('A');

    cout << endl;

    return 0;
}