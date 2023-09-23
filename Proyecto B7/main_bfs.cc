//MAIN PROGRAM
#include "procedures.hh"
#include "graph.hh"
#include <iostream>
#include <chrono>
using namespace std;

//definición de un grafo: grafo no dirigido donde los vertices
//se representan con enteros naturales
vector< vector<int> > Grafo;

//funcion para imprimir informacion respecto valor de result
//_str indica si se ha usado bfs ("_bfs_") o dfs ("_dfs_")
void printInfo(vertex start, vertex end, bool result, const string& _str) 
{
    if (_str == "_bfs_") {
        if (result) {
            cout << "BFS: could reach target: " << end;
            cout << ", from start point: " << start << endl;
        }
        else {
            cout << "BFS: could not reach target: " << end;
            cout << ", from start point: " << start << endl;
        }
    }
    else {
         if (result) {
            cout << "DFS: could reach target: " << end;
            cout << ", from start point: " << start << endl;
        }
        else {
            cout << "DFS: could not reach target: " << end;
            cout << ", from start point: " << start << endl;
        }
    }
}

//main entry point
int main(void)
{
    cout.setf(ios::fixed);
    cout.precision(5);
    
    cout << "----- | PROGRAMA DE TEST EMPIEZA AQUÍ | -----" << endl << endl;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

    std::chrono::time_point<std::chrono::high_resolution_clock> bfs_start, bfs_end;
    
    //start computation point
    start = std::chrono::high_resolution_clock::now();

    int graph_degree;
    cin >> graph_degree;
    
    //Grafo dirigido con graph_degree vértices
    Graph G(graph_degree);

    int _x, _y;

    bool reading;
    cin >> _x >> _y;
    while (_x != 0 and _y != 0) {
        G.add_edge(_x, _y);
        cin >> _x >> _y;
    }

    //vértice donde se empieza y
    //vértice al que se va, respectivamente
    int _ini, _end;
    cin >> _ini >> _end;

    cout << "Graph with degree: " << G.grade();
    cout << " node(s) and density: " << G.density() << endl;

    //bfs procedure start point
    bfs_start = std::chrono::high_resolution_clock::now();
    bool case1 = bfs_find(G, _ini, _end);
    //bfs procedure end point
    bfs_end = std::chrono::high_resolution_clock::now();

    printInfo(_ini, _end, case1, "_bfs_");
    
    //finish computation point
    end = std::chrono::high_resolution_clock::now();

    cout << endl;
    double elaps_time = double(std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count());
    double elaps_time_bfs = double(std::chrono::duration_cast<std::chrono::nanoseconds> (bfs_end - bfs_start).count());
    std::time_t end_time = std::chrono::high_resolution_clock::to_time_t(end);
  
    cout << "finished computation at " << std::ctime(&end_time);
    cout << "elapsed time program: " << elaps_time/1e3 << " ms\n";
    cout << "elapsed time bfs_find(): " << elaps_time_bfs/1e3 << " ms\n";
    
}