#include "Dijkstra_Algo.h"
#include <iostream>

using namespace std;

int main()
{
    Graph_Dijkstra g;
    g.add_vertex('A', {{'B', 7}, {'C', 8}});
    g.add_vertex('B', {{'A', 7}, {'F', 2}});
    g.add_vertex('C', {{'A', 8}, {'F', 6}, {'G', 4}});
    g.add_vertex('D', {{'F', 8}});
    g.add_vertex('E', {{'H', 1}});
    g.add_vertex('F', {{'B', 2}, {'C', 6}, {'D', 8}, {'G', 9}, {'H', 3}});
    g.add_vertex('G', {{'C', 4}, {'F', 9}});
    g.add_vertex('H', {{'E', 1}, {'F', 3}});
    
    for (char vertex : g.shortest_path('A', 'H'))
    {
        cout << vertex << endl;
    }
    
    return 0;
}
