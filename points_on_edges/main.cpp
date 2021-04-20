#include <iostream>
#include <fstream>
#include <iomanip>

#include "graph.cpp"


int main() {
    // (n, m, start)
    Graph<double> g(3, 4, 0);
    // (u, v, edgeWeight^2 (prime number), edgeNum \in \{0, \cdots, m - 1\})
    g.addEdge(0, 1, 2, 0);
    g.addEdge(1, 2, 3, 1);
    g.addEdge(2, 0, 5, 2);
    g.addEdge(2, 0, 7, 3);


    int time = 100;

    g.precalcPoints(time);

    for (int i = 1; i <= time; ++i) {
        PointsSet cur = g.getPoints(i);
        std::cout << "Time " << i << "\n";
        for (int j = 0; j < cur.size(); ++j) {
            std::cout << cur[j].start << " " << cur[j].edgeNum << "\n";
        }
        std::cout << "\n";
    }
}