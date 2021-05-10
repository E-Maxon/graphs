#include <vector>

using namespace std;

int maxcnt;

void dfs3(int v, int d, vector<vector<int> >& graph, vector<int>& used, vector<int>& depth, vector<int>& add) {
    used[v] = 1;
    depth[v] = d;
    for (auto u : graph[v]) {
        if (!used[u]) {
            dfs3(u, d + 1, graph, used, depth, add);
        }
        else {
            add[u] = max(add[u], depth[v] + 1 - depth[u]);
        }
    }
}

void dfs4(int v, int delta, vector<vector<int> >& graph, vector<vector<int> >& levels, vector<int>& used, vector<int>& depth, vector<int>& add) {
    used[v] = 2;
    delta = max(delta, add[v]);
    depth[v] += delta;

    if (levels.size() <= depth[v]) {
        levels.resize(depth[v] + 1);
    }

    levels[depth[v]].push_back(v);
    for (auto u : graph[v]) {
        if (used[u] != 2) {
            dfs4(u, delta, graph, levels, used, depth, add);
        }
    }
}

vector<vector<int> > topsort(vector<vector<int> >& comp, vector<vector<int> >& graph, vector<int>& num) {
    vector<int> used;

    int n = comp.size();
    vector<vector<int> > tree(n);
    for (int i = 0; i < graph.size(); ++i) {
        for (auto u : graph[i]) {
            if (num[i] != num[u]) {
                tree[num[i]].push_back(num[u]);
            }
        }
    }

    vector<int> depth(n), add(n);
    vector<vector<int> > levels;
    used.resize(n);
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs3(i, 0, tree, used, depth, add);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (used[i] != 2) {
            dfs4(i, 0, tree, levels, used, depth, add);
        }
    }

    for (int i = 0; i < levels.size(); ++i) {
        if (levels[i].size() > maxcnt) {
            maxcnt = levels[i].size();
        }
    }

    return levels;
}