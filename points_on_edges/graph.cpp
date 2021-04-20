#include "graph.hpp"


template <typename T>
Graph<T>::Graph(int n, int m, int start): n(n), m(m), start(start) {
    g.resize(n);
}

template <>
void Graph<int>::addEdge(int v, int u, int w, int edgeNum) {
    g[v].push_back(Edge<int>(v, u, w, w, edgeNum));
}


template <>
void Graph<double>::addEdge(int v, int u, int w, int edgeNum) {
    g[v].push_back(Edge<double>(v, u, sqrt(w), w, edgeNum));
}

template <typename T>
void dfs(Graph<T> &g, int v, std::vector<int> &used, std::vector<int> &order) {
    used[v] = 1;
    for (int i = 0; i < g.g[v].size(); ++i) {
        int u = g.g[v][i].u;
        if (!used[u]) {
            dfs(g, u, used, order);
        }
    }
    order.push_back(v);
}


template <typename T>
void dfs2(Graph<T> &g, int v, std::vector<int> &used) {
    used[v] = 1;
    for (int i = 0; i < g.g[v].size(); ++i) {
        int u = g.g[v][i].u;
        if (!used[u]) {
            dfs2(g, u, used);
        }
    }
}

/*
input:
    A:  N vectors in matrix A that are placed as rows.
        M size of each vector
    ids:
        vector of size N with ids
    EPS:
        number is considered to be 0 iff its absolute value is less than EPS

output:
        ids of vectors that span the same subspace as A^t
*/

std::vector<int> GaussianElimination(std::vector<std::vector<double>> A, std::vector<int> ids, double EPS) {
    int N = A.size();
    int M = A[0].size();
    // std::cerr << "EDGES START:\n";
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < M; ++j) {
    //         std::cerr << A[i][j] << " ";
    //     }
    //     std::cerr << "\n";
    // }
    // std::cerr << "EDGES END\n";
    // std::cerr << std::endl;
    int pivC = 0;
    int pivR = 0;
    while (pivC < M && pivR < N) {
        int mx = pivR;
        for (int i = pivR; i < N; ++i) {
            if (std::abs(A[i][pivC]) > std::abs(A[mx][pivC])) {
                mx = i;
            }
        }
        if (std::abs(A[mx][pivC]) < EPS) {
            pivC++;
            continue;
        }
        std::swap(ids[pivR], ids[mx]);
        for (int i = 0; i < M; ++i) {
            // std::cerr << std::max(mx, pivR) << " " << i << " FROM " << N << " " << M << "\n";
            std::swap(A[mx][i], A[pivR][i]);
        }
        for (int i = pivR + 1; i < N; ++i) {
            double c = -A[i][pivC] / A[pivR][pivC];
            for (int j = pivC; j < M; ++j) {
                A[i][j] += c * A[pivR][j];
            }
        }
        pivR++, pivC++;
    }

    // std::cerr << "EDGES AFTER START:\n";
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < M; ++j) {
    //         std::cerr << A[i][j] << " ";
    //     }
    //     std::cerr << "\n";
    // }
    // std::cerr << "EDGES AFTER END\n";

    std::vector<int> res;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (std::abs(A[i][j]) > EPS) {
                res.push_back(i);
                break;
            }
        }
    }
    return res;
}


template <typename T>
 bool cmpCycleLength(const Cycle<T> &a, const Cycle<T> &b) {
    return a.tr.size() < b.tr.size();
}


template <typename T>
void dfsFindAny(int v, Graph<T> &g, std::vector<int> &used, std::vector<int> &goalVertices, std::vector<int> &edgesPref, int &done) {
    used[v] = 1;
    for (auto i: goalVertices) {
        if (v == i) {
            done = 1;
            return;
        }
    }
    for (auto e: g.g[v]) {
        if (!used[e.u]) {
            edgesPref.push_back(e.edgeNum);
            dfsFindAny(e.u, g, used, goalVertices, edgesPref, done);
            if (done) {
                return;
            }
            edgesPref.pop_back();
        }
    }
}

template <typename T>
void dfsFindRestricted(int v, int goalV, Graph<T> &g, std::vector<int> &used, std::vector<int> &allowedVertices, std::vector<int> &edgesPref, int &done) {
    used[v] = 1;
    if (goalV == v) {
        done = 1;
        return;
    }
    for (auto e: g.g[v]) {
        if (!used[e.u] && allowedVertices[e.u]) {
            edgesPref.push_back(e.edgeNum);
            dfsFindRestricted(e.u, goalV, g, used, allowedVertices, edgesPref, done);
            if (done) {
                return;
            }
            edgesPref.pop_back();
        }
    }
}


template <typename T>
std::vector<Cycle<T>> getCycleBasis(Graph<T> &g) {
    int n = g.n;
    int m = g.m;
    std::vector<Cycle<T>> basis;
    std::vector<int> usedVertices(n);
    std::vector<int> usedEdges(m);
    std::vector<int> used(n);
    std::vector<Edge<T>> edges(m);
    for (int i = 0; i < n; ++i) {
        for (auto j: g.g[i]) {
            edges[j.edgeNum] = j;
        }
    }
    usedVertices[g.start] = 1;
    int edgesLeft = m;
    int verticesLeft = n - 1;
    while (edgesLeft > 0) {
        Cycle<T> res;
        res.edgeCount.resize(m);
        if (verticesLeft == 0) {
            int takeEdge = -1;
            for (int i = 0; i < m; ++i) {
                if (!usedEdges[i]) {
                    takeEdge = i;
                    break;
                }
            }
            assert(takeEdge != -1);
            std::vector<int> used(n);
            std::vector<int> edgesPref;
            int done = 0;
            dfsFindRestricted(edges[takeEdge].u, edges[takeEdge].v, g, used, usedVertices, edgesPref, done);
            assert(done == 1);
            usedEdges[takeEdge] = 1;
            edgesLeft--;
            res.tr.push_back(edges[takeEdge]);
            res.edgeCount[takeEdge] = 1;
            for (auto i: edgesPref) {
                res.tr.push_back(edges[i]);
                res.edgeCount[i] = 1;
            }
            basis.push_back(res);
        } else {
            int takeEdge = -1;
            for (int i = 0; i < n; ++i) {
                if (!usedVertices[i]) {
                    continue;
                }
                for (auto j: g.g[i]) {
                    if (!usedVertices[j.u]) {
                        takeEdge = j.edgeNum;
                        break;
                    }
                }
                if (takeEdge != -1) {
                    break;
                }
            }
            assert(takeEdge != -1);
            int v = edges[takeEdge].v;
            int u = edges[takeEdge].u;
            std::vector<int> edgesPref;
            std::vector<int> goalVertices;
            int done = 0;
            for (int i = 0; i < n; ++i) {
                if (usedVertices[i]) {
                    goalVertices.push_back(i);
                }
            }
            std::vector<int> used(n);
            dfsFindAny(u, g, used, goalVertices, edgesPref, done);
            assert(done == 1);
            fill(used.begin(), used.end(), 0);
            assert(edgesPref.size() > 0);
            int lastv = edges[edgesPref.back()].u;
            std::vector<int> edgesPref2;
            done = 0;
            dfsFindRestricted(lastv, v, g, used, usedVertices, edgesPref2, done);

            res.tr.push_back(edges[takeEdge]);
            res.edgeCount[takeEdge] = 1;
            usedEdges[takeEdge] = 1;
            edgesLeft--;
            for (auto i: edgesPref) {
                res.tr.push_back(edges[i]);
                res.edgeCount[i] = 1;
                usedEdges[i] = 1;
                edgesLeft--;
                usedVertices[edges[i].v] = 1;
                verticesLeft--;
            }
            for (auto i: edgesPref2) {
                res.tr.push_back(edges[i]);
                res.edgeCount[i] = 1;
            }
            basis.push_back(res);
        }
    }
    return basis;
}


// template <typename T>
// std::vector<Cycle<T>> getCycleBasis(Graph<T> &g, std::vector<Cycle<T>> &allCycles, double EPS) {
//     int n = g.n;
//     int m = g.m;
//     std::vector<Cycle<T>> basis;
//     std::vector<Cycle<T>> allSorted(allCycles.begin(), allCycles.end());
//     std::sort(allSorted.begin(), allSorted.end(), [] (const Cycle<T> &a, const Cycle<T> &b) {
//             return a.tr.size() < b.tr.size();
//         });
//     std::vector<int> usedEdgeNums(m);
//     for (int j = 0; j < allSorted.size(); ++j) {
//         int take = 0;
//         for (int k = 0; k < m; ++k) {
//             if (allSorted[j].edgeCount[k] > usedEdgeNums[k]) {
//                 take = 1;
//                 break;
//             }
//         }
//         if (take) {
//             basis.push_back(allSorted[j]);
//             for (int k = 0; k < m; ++k) {
//                 usedEdgeNums[k] |= allSorted[j].edgeCount[k];
//             }
//         }
//     }
//     return basis;
// }




// shifts the traversal so that the edge with the minimal id is in the beginning
template <typename T>
void normalizeCycle(Cycle<T> &c) {
    std::vector<Edge<T>> nw(c.tr.size());
    int mni = 0;
    for (int i = 0; i < c.tr.size(); ++i) {
        if (c.tr[i].edgeNum < c.tr[mni].edgeNum) {
            mni = i;
        }
    }
    // shift left by mni
    for (int i = 0; i < c.tr.size(); ++i) {
        nw[i] = c.tr[(i + mni) % c.tr.size()];
    }
    c.tr = nw;
}


template <typename T>
bool operator <(const Cycle<T> &a, const Cycle<T> &b) {
    for (int i = 0; i < std::min(a.tr.size(), b.tr.size()); ++i) {
        if (a.tr[i].edgeNum != b.tr[i].edgeNum) {
            return a.tr[i].edgeNum < b.tr[i].edgeNum;
        }
    }
    return a.tr.size() < b.tr.size();
}

template <typename T>
void dfsCycles(int v, Graph<T> &g, std::vector<int> &used, std::vector<Edge<T>> &currentTraversal, std::set<Cycle<T>> &allCyclesSet) {
    // std::cerr << v << "\n";
    if (currentTraversal.size() >= 2) {
        if (currentTraversal[0].v == currentTraversal.back().u) {
            // std::cerr << "Found a Cycle\n";
            // not sure if the eq is by value
            std::vector<Edge<T>> nwCycle(currentTraversal.begin(), currentTraversal.end()); 
            Cycle nwNorm = Cycle<T>();
            nwNorm.tr = nwCycle;
            nwNorm.edgeCount = std::vector<int>(g.m);
            normalizeCycle(nwNorm);
            // std::cerr << "Cycle Candidate\n";
            // for (int i = 0; i < nwNorm.tr.size(); ++i) {
            //     std::cerr << nwNorm.tr[i].edgeNum << " ";
            // }
            // std::cerr << "\n";
            allCyclesSet.insert(nwNorm); 
        }
    }
    for (int i = 0; i < g.g[v].size(); ++i) {
        int eNum = g.g[v][i].edgeNum;
        int u = g.g[v][i].u;
        if (!used[eNum]) {
            used[eNum] = 1;
            currentTraversal.push_back(g.g[v][i]);
            dfsCycles(u, g, used, currentTraversal, allCyclesSet);
            currentTraversal.pop_back();
            used[eNum] = 0;
        }
    }
}


template <typename T>
std::vector<Cycle<T>> getAllCycles(Graph<T> &g) {
    int n = g.n;
    int m = g.m;
    std::vector<int> used(m);
    std::set<Cycle<T>> allCyclesSet;
    std::vector<Edge<T>> currentTraversal;
    for (int i = 0; i < n; ++i) {
        currentTraversal.clear();
        std::fill(used.begin(), used.end(), 0);
        dfsCycles(i, g, used, currentTraversal, allCyclesSet);
    }
    std::vector<Cycle<T>> allCycles;
    for (auto i: allCyclesSet) {
        allCycles.push_back(i);
    }
    for (int i = 0; i < allCycles.size(); ++i) {
        allCycles[i].edgeCount.resize(m);
        for (int j = 0; j < allCycles[i].tr.size(); ++j) {
            allCycles[i].edgeCount[allCycles[i].tr[j].edgeNum]++;
        }
    }
    return allCycles;
}


template <typename T>
int isStronglyConnected(Graph<T> &g) {
    std::vector<int> used(g.n);
    std::vector<int> order;
    Graph<T> gr(g.n, g.m, g.start);
    for (int i = 0; i < g.n; ++i) {
        for (int j = 0; j < g.g[i].size(); ++j) {
            gr.addEdge(g.g[i][j].u, g.g[i][j].v, g.g[i][j].w, g.g[i][j].edgeNum);
        }
    }
    for (int i = 0; i < g.n; ++i) {
        if (!used[i]) {
            dfs(g, i, used, order);
        }
    }
    std::fill(used.begin(), used.end(), 0);
    dfs2(gr, order.back(), used);
    for (int i = 0; i < g.n; ++i) {
        if (!used[i]) {
            return 0;
        }
    }
    return 1;
}

template <typename T>
void clearGraph(Graph<T> &g) {
    g.g.clear();
    g.g.resize(g.n);
}

// Prime number MAXPRIME (0-indexed) <= MAXN
const int MAXN = 1000;
const int MAXPRIME = 100;


int isprime(int n) {
    if (n == 1) {
        return 0;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

// 0-indexed
int nthprime(int n) {
    int cur = 0;
    for (int i = 2; i < MAXN; ++i) {
        if (isprime(i)) {
            if (cur == n) {
                return i;
            }
            cur++;
        }
    }
}

// true iff successfuly generated
int genRandGraph(int maxIter, Graph<double> &g) {
    for (int iter = 0; iter < maxIter; ++iter) {
        // std::cout << iter;
        std::vector<int> usedPrimes(MAXPRIME); 
        std::vector<std::pair<int, int>> potEdges;
        for (int i = 0; i < g.n; ++i) {
            for (int j = 0; j < g.n; ++j) {
                if (i == j) {
                    continue;
                }
                potEdges.push_back({i, j});
            }
        }
        if (potEdges.size() < g.m) {
            // Edge goal isn't reachable
            return 0;
        }
        for (int i = 0; i < g.m;) {
            int edgeId = rand() % potEdges.size();
            int v = potEdges[edgeId].first;
            int u = potEdges[edgeId].second;
            potEdges.erase(potEdges.begin() + edgeId);
            int takeprime = -1;
            for (int i = 0; i < MAXPRIME; ++i) {
                if (!usedPrimes[i]) {
                    takeprime = i;
                    break;
                }
            }
            assert(takeprime != -1);
            usedPrimes[takeprime] = 1;
            int w = nthprime(takeprime);
            g.addEdge(v, u, w, i);
            ++i;
        }
        if (isStronglyConnected(g)) {
            return 1;
        }
        clearGraph(g);
    }
    return 0;
}

template <typename T>
int Graph<T>::getN(int time) {
    if (memo.count(time)) {
        return memo[time];
    } else {
        throw std::string("The value at the given moment of time is not precalculated");
    }
}

template <>
void Graph<int>::precalc(int time) {
    std::set<std::pair<int, int>> q;
    std::vector<int> cnt(time + 1);
    memo[0] = 1;
    q.insert({0, start});
    while (!q.empty()) {
        auto cur = *q.begin();
        q.erase(q.begin());
        // (cur.first; cur.first + u.w]
        for (int i = 0; i < g[cur.second].size(); ++i) {
            auto u = g[cur.second][i];
            if (cur.first + 1 <= time) {
                cnt[cur.first + 1]++;
            }
            if (cur.first + u.w + 1 <= time) {
                cnt[cur.first + u.w + 1]--;
            }
            int u_time = cur.first + u.w;
            if (u_time < time) {
                q.insert({u_time, u.u});
            }
        }
    }
    int sv = 0;
    for (int i = 0; i <= time; ++i) {
        sv += cnt[i];
        memo[i] = sv;
    }
}

struct Path {
    short v;
    double time;
    std::vector<short> took;
};

template <>
void Graph<double>::precalc(int time) {
    std::vector<int> cnt(time + 1);
    std::set <std::vector<short>> used;
    std::deque<Path> q;
    Path first;
    first.v = start;
    first.time = 0;
    int mx_edge = 1;
    for (int i = 0; i < n; ++i) {
        for (auto u: g[i]) {
            mx_edge = std::max(mx_edge, u.edgeNum + 1);
        }
    }
    first.took = std::vector <short> (mx_edge);
    q.push_back(first);
    memo[0] = 1;
    while(!q.empty()) {
        // std::cerr << q.size() << "\n";
        Path cur = q.front();
        // std::cerr << "Time " << cur.time << "\n";
        q.pop_front();
        if (used.find(cur.took) != used.end()) {
            continue;
        }
        used.insert(cur.took); 
        for (auto u: g[cur.v]) {
            int l = ceil(cur.time);
            int r = floor(cur.time + u.w) + 1;
            if (l <= time) {
                cnt[l]++;
            }
            if (r <= time) {
                cnt[r]--;
            }
            if (cur.time + u.w < time) {
                Path nw;
                nw.v = u.u;
                nw.time = cur.time + u.w;
                nw.took = cur.took;
                // if (rand() % 1000 == 0) {
                //     for (int i = 0; i < nw.took.size(); ++i) {
                //         std::cerr << "Took " << i << ": " << nw.took[i] << "\n";
                //     }
                // }
                nw.took[u.edgeNum]++;
                q.push_back(nw);      
            } 
        }
    }
    int sv = 0;
    for (int i = 0; i <= time; ++i) {
        sv += cnt[i];
        memo[i] = sv;
    }
}


template <>
void Graph<double>::precalcPoints(int time) {
    std::vector<int> cnt(time + 1);
    std::set <std::vector<short>> used;
    std::deque<Path> q;
    Path first;
    first.v = start;
    first.time = 0;
    int mx_edge = 1;
    for (int i = 0; i < n; ++i) {
        for (auto u: g[i]) {
            mx_edge = std::max(mx_edge, u.edgeNum + 1);
        }
    }
    first.took = std::vector <short> (mx_edge);
    q.push_back(first);
    while(!q.empty()) {
        Path cur = q.front();
        q.pop_front();
        if (used.find(cur.took) != used.end()) {
            continue;
        }
        used.insert(cur.took); 
        for (auto u: g[cur.v]) {
            int l = ceil(cur.time);
            int r = floor(cur.time + u.w);
            for (int i = std::min(l, time); i <= std::min(r, time); ++i) {
                memoPoints[i].points.push_back({u.edgeNum, cur.time});
            }
            if (cur.time + u.w < time) {
                Path nw;
                nw.v = u.u;
                nw.time = cur.time + u.w;
                nw.took = cur.took;
                nw.took[u.edgeNum]++;
                q.push_back(nw);      
            } 
        }
    }
}

template <typename T>
PointsSet Graph<T>::getPoints(int time) {
    if (memoPoints.count(time)) {
        return memoPoints[time];
    } else {
        throw std::string("The value hasn't been precalculated");
    }
}


template class Graph<int>;
template class Graph<double>;

