#ifndef INFO_HH
#define INFO_HH

#include <iostream>
#include "graph.hh"

/**
 * Para mostrar contenido del grafo
 */
void info(const Graph& G)
{
    auto it = G.A.begin();
    while (it != G.A.end()) {
        std::cout << '(' << it->first << ',';
        std::cout << it->second << ')' << std::endl;
        it++;
    }

    std::cout << std::endl;

    for (int i = 0; i < G.num_vertex; ++i) {
        std::cout << "adjacents of " << i+1 << ':';
        auto it1 = G.adjList[i].begin();
        while (it1 != G.adjList[i].end()) {
            std::cout << *it1 << ' ';
            ++it1;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}

#endif