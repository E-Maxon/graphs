#include <vector>
#include <map>
#include <utility>
#include <set>
#include <string>
#include <deque>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <cassert>

template <typename T>
struct Edge_p {
    int v, u, id, edgeNum;
    T w;
    Edge_p(){}
    Edge_p(int v, int u, T w, int id, int edgeNum) : v(v), u(u), w(w), id(id), edgeNum(edgeNum) {}
};

template <typename T>
struct Cycle {
    std::vector<Edge_p<T>> tr;
    std::vector<int> edgeCount;
    Cycle(){}
};

struct Point {
    int edgeNum;
    // the point of time at which the point has appeared
    double start;
};

struct PointsSet {
    int time;
    std::vector<Point> points;
    PointsSet() {}
    int size() {
        return points.size();
    }
    Point &operator[](int i) {
        if (i < 0 || i >= points.size()) {
            throw std::string("Index out of bounds");
        }
        return points[i];
    }
};

template <typename T>
class Graph {
public:
    Graph() {}
    Graph(int n, int m, int start);
    // Graph(const Graph<T>&){}
    void addEdge(int v, int u, int w, int edgeNum);
    
    int getN(int time);
    void precalc(int time);

    PointsSet getPoints(int time);
    int getPointsNumber(int time);
    void precalcPoints(int time);
    int n, m, start;
    std::vector<std::vector<Edge_p<T>>> g;
private:
    int lastEdgeId = 0;
    std::map<int, int> memo;
    std::map<int, PointsSet> memoPoints;
};