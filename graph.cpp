#include "graph.h"
#include <queue>
#include <vector>
#include <climits>

// Used for Dijkstra's min heap
struct DijkstraNode
{
    int vertexIndex;
    int distance;
};

// Makes priority_queue work like a min heap for Dijkstra
struct CompareDijkstraNode
{
    bool operator()(DijkstraNode a, DijkstraNode b)
    {
        return a.distance > b.distance;
    }
};

// Used for Kruskal's algorithm
struct MSTEdge
{
    int src;
    int dest;
    int weight;
};

// Used for Prim's min heap
struct PrimEdge
{
    int src;
    int dest;
    int weight;
};

// Makes priority_queue work like a min heap for Prim
struct ComparePrimEdge
{
    bool operator()(PrimEdge a, PrimEdge b)
    {
        return a.weight > b.weight;
    }
};

// Find function for union-find
int findParent(int parent[], int index)
{
    while(parent[index] != index)
    {
        index = parent[index];
    }

    return index;
}

// Union function for union-find
void unionSets(int parent[], int a, int b)
{
    int rootA = findParent(parent, a);
    int rootB = findParent(parent, b);

    parent[rootB] = rootA;
}

Graph::Graph(int maxV)
{
    maxVertices = maxV;
    numVertices = 0;
    vertices = nullptr;

    visited = new bool[maxVertices] {false};

    adjMatrix = new int*[maxVertices];

    for(int i = 0; i < maxVertices; i++)
    {
        adjMatrix[i] = new int[maxVertices];

        for(int j = 0; j < maxVertices; j++)
        {
            adjMatrix[i][j] = 0;
        }
    }
}

Graph::~Graph()
{
    delete[] visited;

    for(int i = 0; i < maxVertices; i++)
    {
        delete[] adjMatrix[i];
    }

    delete[] adjMatrix;

    vertex* tempVertex;

    while(vertices)
    {
        edge* tempEdge;

        while(vertices->aHead)
        {
            tempEdge = vertices->aHead;
            vertices->aHead = vertices->aHead->eNext;
            delete tempEdge;
        }

        tempVertex = vertices;
        vertices = vertices->vNext;
        delete tempVertex;
    }
}

void Graph::addVertex(char data)
{
    if(numVertices >= maxVertices)
    {
        cout << "Max number of vertices reached. Can't add!" << endl;
        return;
    }

    vertex* newVertex = new vertex;
    newVertex->data = data;
    newVertex->vNext = vertices;
    newVertex->aHead = nullptr;

    vertices = newVertex;

    numVertices++;
}

void Graph::addEdge(char src, char dest, int weight)
{
    vertex* srcVertex = vertices;
    vertex* destVertex = vertices;

    while(srcVertex && srcVertex->data != src)
    {
        srcVertex = srcVertex->vNext;
    }

    while(destVertex && destVertex->data != dest)
    {
        destVertex = destVertex->vNext;
    }

    if(!srcVertex || !destVertex)
    {
        cout << "Vertices not found - can't add an edge" << endl;
        return;
    }

    // Add edge from source to destination
    edge* newEdge = new edge;
    newEdge->dest = destVertex;
    newEdge->weight = weight;
    newEdge->eNext = srcVertex->aHead;
    srcVertex->aHead = newEdge;

    // Add reverse edge because graph is undirected
    newEdge = new edge;
    newEdge->dest = srcVertex;
    newEdge->weight = weight;
    newEdge->eNext = destVertex->aHead;
    destVertex->aHead = newEdge;

    // Add to adjacency matrix
    int srcIndex = srcVertex->data - 'A';
    int destIndex = destVertex->data - 'A';

    adjMatrix[srcIndex][destIndex] = weight;
    adjMatrix[destIndex][srcIndex] = weight;
}

void Graph::removeVertex(char data)
{
    vertex* temp = vertices;
    vertex* prev = nullptr;

    while(temp && temp->data != data)
    {
        prev = temp;
        temp = temp->vNext;
    }

    if(!temp)
    {
        cout << "Vertex not found - can't remove" << endl;
        return;
    }

    int index = temp->data - 'A';

    // Remove this vertex from all adjacency lists
    for(vertex* v = vertices; v != nullptr; v = v->vNext)
    {
        edge* e = v->aHead;
        edge* prevEdge = nullptr;

        while(e)
        {
            if(e->dest->data == data)
            {
                if(prevEdge)
                {
                    prevEdge->eNext = e->eNext;
                }
                else
                {
                    v->aHead = e->eNext;
                }

                edge* deleteEdge = e;
                e = e->eNext;
                delete deleteEdge;
            }
            else
            {
                prevEdge = e;
                e = e->eNext;
            }
        }
    }

    // Remove vertex from vertex list
    if(prev)
    {
        prev->vNext = temp->vNext;
    }
    else
    {
        vertices = temp->vNext;
    }

    // Delete this vertex's own edge list
    edge* currentEdge = temp->aHead;

    while(currentEdge)
    {
        edge* deleteEdge = currentEdge;
        currentEdge = currentEdge->eNext;
        delete deleteEdge;
    }

    delete temp;

    // Remove from adjacency matrix
    for(int i = 0; i < maxVertices; i++)
    {
        adjMatrix[i][index] = 0;
        adjMatrix[index][i] = 0;
    }

    numVertices--;
}

void Graph::removeEdge(char src, char dest)
{
    vertex* srcVertex = vertices;
    vertex* destVertex = vertices;

    while(srcVertex && srcVertex->data != src)
    {
        srcVertex = srcVertex->vNext;
    }

    while(destVertex && destVertex->data != dest)
    {
        destVertex = destVertex->vNext;
    }

    if(!srcVertex || !destVertex)
    {
        cout << "Vertices not found - can't remove edge" << endl;
        return;
    }

    // Remove edge from src to dest
    edge* current = srcVertex->aHead;
    edge* previous = nullptr;

    while(current)
    {
        if(current->dest->data == dest)
        {
            if(previous)
            {
                previous->eNext = current->eNext;
            }
            else
            {
                srcVertex->aHead = current->eNext;
            }

            delete current;
            break;
        }

        previous = current;
        current = current->eNext;
    }

    // Remove edge from dest to src
    current = destVertex->aHead;
    previous = nullptr;

    while(current)
    {
        if(current->dest->data == src)
        {
            if(previous)
            {
                previous->eNext = current->eNext;
            }
            else
            {
                destVertex->aHead = current->eNext;
            }

            delete current;
            break;
        }

        previous = current;
        current = current->eNext;
    }

    // Remove from adjacency matrix
    int srcIndex = src - 'A';
    int destIndex = dest - 'A';

    adjMatrix[srcIndex][destIndex] = 0;
    adjMatrix[destIndex][srcIndex] = 0;
}

void Graph::bfsL(char start)
{
    for(int i = 0; i < maxVertices; i++)
    {
        visited[i] = false;
    }

    vertex* startVertex = vertices;

    while(startVertex && startVertex->data != start)
    {
        startVertex = startVertex->vNext;
    }

    if(!startVertex)
    {
        cout << "Start vertex not found" << endl;
        return;
    }

    vertex** queue = new vertex*[maxVertices];
    int front = 0;
    int rear = 0;

    visited[startVertex->data - 'A'] = true;
    queue[rear] = startVertex;
    rear++;

    cout << "BFS List starting at " << start << ": ";

    while(front < rear)
    {
        vertex* current = queue[front];
        front++;

        cout << current->data << " ";

        edge* tempEdge = current->aHead;

        while(tempEdge)
        {
            int index = tempEdge->dest->data - 'A';

            if(!visited[index])
            {
                visited[index] = true;
                queue[rear] = tempEdge->dest;
                rear++;
            }

            tempEdge = tempEdge->eNext;
        }
    }

    cout << endl;

    delete[] queue;
}

void Graph::bfsM(char start)
{
    for(int i = 0; i < maxVertices; i++)
    {
        visited[i] = false;
    }

    int startIndex = start - 'A';

    if(startIndex < 0 || startIndex >= maxVertices)
    {
        cout << "Start vertex not found" << endl;
        return;
    }

    int* queue = new int[maxVertices];
    int front = 0;
    int rear = 0;

    visited[startIndex] = true;
    queue[rear] = startIndex;
    rear++;

    cout << "BFS Matrix starting at " << start << ": ";

    while(front < rear)
    {
        int current = queue[front];
        front++;

        cout << char('A' + current) << " ";

        for(int i = 0; i < maxVertices; i++)
        {
            if(adjMatrix[current][i] != 0 && !visited[i])
            {
                visited[i] = true;
                queue[rear] = i;
                rear++;
            }
        }
    }

    cout << endl;

    delete[] queue;
}

void Graph::dfsL(char start)
{
    for(int i = 0; i < maxVertices; i++)
    {
        visited[i] = false;
    }

    vertex* startVertex = vertices;

    while(startVertex && startVertex->data != start)
    {
        startVertex = startVertex->vNext;
    }

    if(!startVertex)
    {
        cout << "Start vertex not found" << endl;
        return;
    }

    vertex** stack = new vertex*[maxVertices];
    int top = 0;

    stack[top] = startVertex;
    top++;
    visited[startVertex->data - 'A'] = true;

    cout << "DFS List starting at " << start << ": ";

    while(top > 0)
    {
        top--;
        vertex* current = stack[top];

        cout << current->data << " ";

        edge* tempEdge = current->aHead;

        while(tempEdge)
        {
            int index = tempEdge->dest->data - 'A';

            if(!visited[index])
            {
                visited[index] = true;
                stack[top] = tempEdge->dest;
                top++;
            }

            tempEdge = tempEdge->eNext;
        }
    }

    cout << endl;

    delete[] stack;
}

void Graph::dfsM(char start)
{
    for(int i = 0; i < maxVertices; i++)
    {
        visited[i] = false;
    }

    int startIndex = start - 'A';

    if(startIndex < 0 || startIndex >= maxVertices)
    {
        cout << "Start vertex not found" << endl;
        return;
    }

    int* stack = new int[maxVertices];
    int top = 0;

    stack[top] = startIndex;
    top++;
    visited[startIndex] = true;

    cout << "DFS Matrix starting at " << start << ": ";

    while(top > 0)
    {
        top--;
        int current = stack[top];

        cout << char('A' + current) << " ";

        for(int i = maxVertices - 1; i >= 0; i--)
        {
            if(adjMatrix[current][i] != 0 && !visited[i])
            {
                visited[i] = true;
                stack[top] = i;
                top++;
            }
        }
    }

    cout << endl;

    delete[] stack;
}

bool Graph::detectCycle()
{
    for(int i = 0; i < maxVertices; i++)
    {
        visited[i] = false;
    }

    vertex** stack = new vertex*[maxVertices];
    vertex** parent = new vertex*[maxVertices];

    for(int i = 0; i < maxVertices; i++)
    {
        parent[i] = nullptr;
    }

    for(vertex* start = vertices; start != nullptr; start = start->vNext)
    {
        int startIndex = start->data - 'A';

        if(!visited[startIndex])
        {
            int top = 0;

            stack[top] = start;
            top++;
            visited[startIndex] = true;
            parent[startIndex] = nullptr;

            while(top > 0)
            {
                top--;
                vertex* current = stack[top];

                int currentIndex = current->data - 'A';
                edge* tempEdge = current->aHead;

                while(tempEdge)
                {
                    int neighborIndex = tempEdge->dest->data - 'A';

                    if(!visited[neighborIndex])
                    {
                        visited[neighborIndex] = true;
                        parent[neighborIndex] = current;

                        stack[top] = tempEdge->dest;
                        top++;
                    }
                    else if(parent[currentIndex] != tempEdge->dest)
                    {
                        delete[] stack;
                        delete[] parent;
                        return true;
                    }

                    tempEdge = tempEdge->eNext;
                }
            }
        }
    }

    delete[] stack;
    delete[] parent;

    return false;
}

void Graph::dijkstra(char start)
{
    int startIndex = start - 'A';

    if(startIndex < 0 || startIndex >= maxVertices)
    {
        cout << "Start vertex not found" << endl;
        return;
    }

    vertex** vertexArray = new vertex*[maxVertices];

    for(int i = 0; i < maxVertices; i++)
    {
        vertexArray[i] = nullptr;
    }

    for(vertex* v = vertices; v != nullptr; v = v->vNext)
    {
        int index = v->data - 'A';
        vertexArray[index] = v;
    }

    if(vertexArray[startIndex] == nullptr)
    {
        cout << "Start vertex not found" << endl;
        delete[] vertexArray;
        return;
    }

    int* distance = new int[maxVertices];
    bool* done = new bool[maxVertices];

    for(int i = 0; i < maxVertices; i++)
    {
        distance[i] = INT_MAX;
        done[i] = false;
    }

    priority_queue<DijkstraNode, vector<DijkstraNode>, CompareDijkstraNode> minHeap;

    distance[startIndex] = 0;

    DijkstraNode firstNode;
    firstNode.vertexIndex = startIndex;
    firstNode.distance = 0;

    minHeap.push(firstNode);

    while(!minHeap.empty())
    {
        DijkstraNode currentNode = minHeap.top();
        minHeap.pop();

        int currentIndex = currentNode.vertexIndex;

        if(done[currentIndex])
        {
            continue;
        }

        done[currentIndex] = true;

        edge* tempEdge = vertexArray[currentIndex]->aHead;

        while(tempEdge != nullptr)
        {
            int neighborIndex = tempEdge->dest->data - 'A';

            if(!done[neighborIndex])
            {
                int newDistance = distance[currentIndex] + tempEdge->weight;

                if(newDistance < distance[neighborIndex])
                {
                    distance[neighborIndex] = newDistance;

                    DijkstraNode newNode;
                    newNode.vertexIndex = neighborIndex;
                    newNode.distance = newDistance;

                    minHeap.push(newNode);
                }
            }

            tempEdge = tempEdge->eNext;
        }
    }

    cout << "Dijkstra Shortest Paths starting at " << start << ":" << endl;

    for(int i = 0; i < maxVertices; i++)
    {
        if(vertexArray[i] != nullptr)
        {
            cout << start << " to " << char('A' + i) << ": ";

            if(distance[i] == INT_MAX)
            {
                cout << "No path";
            }
            else
            {
                cout << distance[i];
            }

            cout << endl;
        }
    }

    cout << endl;

    delete[] vertexArray;
    delete[] distance;
    delete[] done;
}

void Graph::kruskal()
{
    MSTEdge* edges = new MSTEdge[maxVertices * maxVertices];
    int edgeCount = 0;

    for(vertex* v = vertices; v != nullptr; v = v->vNext)
    {
        int srcIndex = v->data - 'A';

        for(edge* e = v->aHead; e != nullptr; e = e->eNext)
        {
            int destIndex = e->dest->data - 'A';

            if(srcIndex < destIndex)
            {
                edges[edgeCount].src = srcIndex;
                edges[edgeCount].dest = destIndex;
                edges[edgeCount].weight = e->weight;
                edgeCount++;
            }
        }
    }

    // Sort edges by weight using selection sort
    for(int i = 0; i < edgeCount - 1; i++)
    {
        int minIndex = i;

        for(int j = i + 1; j < edgeCount; j++)
        {
            if(edges[j].weight < edges[minIndex].weight)
            {
                minIndex = j;
            }
        }

        MSTEdge temp = edges[i];
        edges[i] = edges[minIndex];
        edges[minIndex] = temp;
    }

    int* parent = new int[maxVertices];

    for(int i = 0; i < maxVertices; i++)
    {
        parent[i] = i;
    }

    int totalCost = 0;
    int mstEdgeCount = 0;

    cout << "Kruskal MST:" << endl;

    for(int i = 0; i < edgeCount && mstEdgeCount < numVertices - 1; i++)
    {
        int srcRoot = findParent(parent, edges[i].src);
        int destRoot = findParent(parent, edges[i].dest);

        if(srcRoot != destRoot)
        {
            cout << char('A' + edges[i].src) << " - "
                 << char('A' + edges[i].dest) << " : "
                 << edges[i].weight << endl;

            totalCost += edges[i].weight;
            mstEdgeCount++;

            unionSets(parent, edges[i].src, edges[i].dest);
        }
    }

    cout << "Total Cost: " << totalCost << endl;

    if(mstEdgeCount != numVertices - 1)
    {
        cout << "The graph is disconnected, so a full MST was not created." << endl;
    }

    cout << endl;

    delete[] edges;
    delete[] parent;
}

void Graph::prim(char start)
{
    int startIndex = start - 'A';

    if(startIndex < 0 || startIndex >= maxVertices)
    {
        cout << "Start vertex not found" << endl;
        return;
    }

    vertex** vertexArray = new vertex*[maxVertices];

    for(int i = 0; i < maxVertices; i++)
    {
        vertexArray[i] = nullptr;
    }

    for(vertex* v = vertices; v != nullptr; v = v->vNext)
    {
        int index = v->data - 'A';
        vertexArray[index] = v;
    }

    if(vertexArray[startIndex] == nullptr)
    {
        cout << "Start vertex not found" << endl;
        delete[] vertexArray;
        return;
    }

    bool* inMST = new bool[maxVertices];

    for(int i = 0; i < maxVertices; i++)
    {
        inMST[i] = false;
    }

    priority_queue<PrimEdge, vector<PrimEdge>, ComparePrimEdge> minHeap;

    inMST[startIndex] = true;

    edge* tempEdge = vertexArray[startIndex]->aHead;

    while(tempEdge != nullptr)
    {
        PrimEdge newEdge;
        newEdge.src = startIndex;
        newEdge.dest = tempEdge->dest->data - 'A';
        newEdge.weight = tempEdge->weight;

        minHeap.push(newEdge);

        tempEdge = tempEdge->eNext;
    }

    int totalCost = 0;
    int mstEdgeCount = 0;

    cout << "Prim MST starting at " << start << ":" << endl;

    while(!minHeap.empty() && mstEdgeCount < numVertices - 1)
    {
        PrimEdge current = minHeap.top();
        minHeap.pop();

        if(inMST[current.dest])
        {
            continue;
        }

        cout << char('A' + current.src) << " - "
             << char('A' + current.dest) << " : "
             << current.weight << endl;

        totalCost += current.weight;
        mstEdgeCount++;

        inMST[current.dest] = true;

        tempEdge = vertexArray[current.dest]->aHead;

        while(tempEdge != nullptr)
        {
            int neighborIndex = tempEdge->dest->data - 'A';

            if(!inMST[neighborIndex])
            {
                PrimEdge newEdge;
                newEdge.src = current.dest;
                newEdge.dest = neighborIndex;
                newEdge.weight = tempEdge->weight;

                minHeap.push(newEdge);
            }

            tempEdge = tempEdge->eNext;
        }
    }

    cout << "Total Cost: " << totalCost << endl;

    if(mstEdgeCount != numVertices - 1)
    {
        cout << "The graph is disconnected, so a full MST was not created." << endl;
    }

    cout << endl;

    delete[] vertexArray;
    delete[] inMST;
}

void Graph::displayList()
{
    for(vertex* v = vertices; v != nullptr; v = v->vNext)
    {
        cout << v->data << " -> ";

        for(edge* e = v->aHead; e != nullptr; e = e->eNext)
        {
            cout << e->dest->data << " (" << e->weight << ") -> ";
        }

        cout << "null" << endl;
    }

    cout << endl;
}

void Graph::displayMatrix()
{
    cout << "     ";

    for(int i = 0; i < maxVertices; i++)
    {
        cout << setw(3) << char('A' + i) << " ";
    }

    cout << endl;
    cout << "   ";

    for(int i = 0; i < maxVertices; i++)
    {
        cout << "-----";
    }

    cout << endl;

    for(int i = 0; i < maxVertices; i++)
    {
        cout << setw(3) << char('A' + i) << " |";

        for(int j = 0; j < maxVertices; j++)
        {
            cout << setw(3) << adjMatrix[i][j] << " ";
        }

        cout << endl;
    }
}