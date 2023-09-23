#ifndef GRAPH_HH
#define GRAPH_HH

#include <vector>
#include <list>
#include <set>
#include <map>
using namespace std;

/**
 * Each vertex of the graph is represented by an integer
 */
typedef int vertex;
/**
 * Each edge/arc of the graph is represented by a pair of integers
 */
typedef pair<vertex, vertex> edge;

/**
 * Overload operator == for objects of type edge
 (Does not really matter since edge contains basic types)
 */
bool operator==(const edge& a, const edge& b) 
{
    return a.first == b.first and a.second == b.second;
}

/**
 * Definition of  graph class
 * (V, A) graph, where V is set of vertex and A is set of edges. 
 * With V = {1, ..., n}
 */
struct Graph {

    /**
     * Create en empty graph (no vertices and no edges)
     */
    Graph() {num_vertex = num_edges = 0;}

    /**
     * Create en empty graph with n vertices (but no edges)
     */
    Graph(int n) 
    {
        num_vertex = n;
        num_edges = 0;
        for (int i = 0; i < n; ++i) V.insert(V.end(), i+1);

        //create adjancecy list where each vertex has no adjacent vertex
        //therefore and empty list 
        adjList = vector<list<int>> (n, list<int> ());
    }
    /**
     * adds a new vertex to the graph; the new vertex will have identifier
     * "n+1" where n is the number of vertices in the graph, before adding the
     * the new vertex
     */
    void add_vertex() 
    {
        num_vertex = num_vertex + 1;
        V.insert(V.end(), num_vertex);
        adjList.push_back(list<vertex> ());
    }
    /**
     * adds edge with extremes u and vthat go from u to v in the graph
     */
    void add_edge(vertex u, vertex v) 
    {
        //Make sure both vertex exist. Since vertices go from 1 to n
        //we just try if u and v are smaller or equal to num_vertex
        if (u <= num_vertex and v <= num_vertex) {
            edge ins = make_pair(u,v);
            bool exist = false;
            auto it = A.begin();
            while (it != A.end() and not exist) {
                if (*it == ins) exist = true;
                else ++it;
            }


            //avoid adding already existing edges
            if (not exist) {
                num_edges = num_edges + 1;
                A.insert(A.end(), ins);
                adjList[u-1].insert(adjList[u-1].end(), v);
            }
        }
    }

    /**
     * Returns the density of the graph
     */
    double density() const 
    {   
        double _A = A.size();
        double _V = V.size();
        return _A / (_V * (_V - 1));
    }
    /**
     * Returns the degree of the graph
     */
    int grade() const{
        return int(V.size());
    }

    typedef vector<list<vertex>> graph_container;

    /**
     * Number of vertices
     */
    int num_vertex;
    /**
     * Number of edges
     */
    int num_edges;

    /**
     * Adjancecy list of the graph
     */
    graph_container adjList; 
    /**
     * List of vertex in the graph
     */
    list<vertex> V;
    /**
     * List of edges in the graph
     */
    list<edge> A;
};

#endif
