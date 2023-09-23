#include "graph.hh"
#include <iostream>
#include <queue>
#include <stack>


/**
 * Dentro de la componente conexa a la que pertence el vértice start_point
 * retorna cierto si se puede llegar al vértice target, falso en caso 
 * contrario, aplicando BFS. Se asume los vértices start_point y target 
 * son vértices válidos, osea están en el grafo y no son el mismo
 */
bool bfs_find(const Graph& _graph, vertex start_point, vertex target)
{
    queue<vertex> next_to_visit;
    
    //número total de vértices
    int LIMIT = _graph.num_vertex;

    //vector de vértices visitados
    vector<bool> visited_vertices(LIMIT, false);

    next_to_visit.push(start_point);
    visited_vertices[start_point-1] = true;

    //booleano indica si se ha llegado al
    //target vértice desde el vértice start_point
    bool reached = false;

    while (not next_to_visit.empty() and not reached) {
        //se trata el primero en la cola
        vertex _next = next_to_visit.front();
        next_to_visit.pop();
        if (not reached) {
            //explora todos los adyacentes a _next
            for (int current : _graph.adjList[_next-1]) {
                if (not visited_vertices[current-1]) {
                    next_to_visit.push(current);
                    visited_vertices[current-1] = true;
                    if (current == target) reached = true;
                }
            }
        }
    }
    return reached;
}

/**
 * Dentro de la componente conexa a la que pertence el vértice start_point
 * retorna cierto si se puede llegar al vértice target, falso en caso 
 * contrario, aplicando DFS. Se asume los vértices start_point y target son
 *  vértices válidos, osea están en el grafo y no son el mismo
 */
bool dfs_find(const Graph& _graph, vertex start_point, vertex target)
{
    stack<vertex> next_to_visit;
    
    //número total de vértices
    int LIMIT = _graph.num_vertex;

    //vector de vértices visitados
    vector<bool> visited_vertices(LIMIT, false);

    next_to_visit.push(start_point);

    //booleano indica si se ha llegado al
    //target vértice desde el vértice start_point
    bool reached = false;

    while (not next_to_visit.empty() and not reached) {
        vertex _next = next_to_visit.top();
        next_to_visit.pop();
        if (not visited_vertices[_next-1]) {
            visited_vertices[_next-1] = true;
            if (_next == target) reached = true;
            if (not reached) {
                for (vertex _y : _graph.adjList[_next-1])  
                    next_to_visit.push(_y);
            }
            
        }
    }
    return reached;
}
