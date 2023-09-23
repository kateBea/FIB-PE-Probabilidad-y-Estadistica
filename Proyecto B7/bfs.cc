#include <iostream>
#include <map>
#include <list>
#include <stdio.h>
#include <chrono>
#include <time.h>

using namespace std;

struct Graph
{
    int V;    // No. of vertices
 
    // Pointer to an array containing adjacency
    // lists
    list<int> *adj;  

    Graph(int V);  // Constructor
 
    // function to add an edge to graph
    void addEdge(int v, int w);
 
    // prints BFS traversal from a given source s
    bool BFS(int s, int buscar); 
};
 
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}
 
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}

// s es el vertice desde donde se empiece, k es el elemento a buscar
bool Graph::BFS(int s, int k)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
        visited[i] = false;
    // Create a queue for BFS
    list<int> queue;

    bool found = false;
    
    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);
 
    // 'i' will be used to get all adjacent
    // vertices of a vertex
    list<int>::iterator i;
    
    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        cout << s << " ";
        queue.pop_front();
 
        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
                if (*i == k && !found) {
                    found = true;
                }
            }
        }
        
    }
    
    return found;
}
 
// Driver program to test methods of graph class
int main()
{
    // Create a graph given in the above diagram
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);

 
    cout << "Following is Breadth First Traversal "
         << "(starting from vertex 0) \n";

    std::chrono::steady_clock::time_point end, end1, start;

    int k;
    cin >> k;
    start = chrono::steady_clock::now();
    bool _res = g.BFS(0,k);
    end = chrono::steady_clock::now();

    // tiempo que tarda en encontrar el elemento a buscar "k"
    cout << endl;
    cout << "Tiempo que se ha tardado en dar resultado: "
        << double(chrono::duration_cast<chrono::nanoseconds>(end - start).count()/1e3)
        << " ms" << endl;
    cout << endl;
    if (_res) cout << "se ha encotrado el elemnto k..." << endl;
    else cout << "no se ha encontrado el elemento k..." << endl;

    end1 = chrono::steady_clock::now();
    //tiempo que el programa esta en marcha
    cout << "El programa ha estado en marcha: "
        << double(chrono::duration_cast<chrono::nanoseconds>(end1 - start).count()/1e9)
        << " s" << endl;
    return 0;
}