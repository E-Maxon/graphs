#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> comps;

void dfs1(int v, vector<vector<int> >& graph, vector<int>& top_sort, vector<bool>& used) {
    used[v] = true;
    for (auto u : graph[v]) {
        if (!used[u]) {
            dfs1(u, graph, top_sort, used);
        }
    }
    top_sort.push_back(v);
    return;
}

void dfs2(int v, int k, vector<vector<int> >& grcomp, vector<vector<int> >& Tgr) {
    comps[v] = k;
    grcomp.back().push_back(v);
    for (auto u : Tgr[v]) {
        if (comps[u] == -1) {
            dfs2(u, k, grcomp, Tgr);
        }
    }
    return;
}

vector<vector<int> > condense(vector<vector<int> >& graph) {
    vector<vector<int> > Tgr, grcomp;
    vector<bool> used;
    vector<int> top_sort;

    int n = graph.size();
    Tgr.resize(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < graph[i].size(); ++j) {
            Tgr[graph[i][j]].push_back(i);
        }
    }
    used.assign(n, false);
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs1(i, graph, top_sort, used);
        }
    }
    reverse(top_sort.begin(), top_sort.end());
    used.assign(n, false);
    comps.assign(n, -1);
    int n_comp = 0;
    for (int i = 0; i < n; ++i) {
        if (comps[top_sort[i]] == -1) {
            grcomp.resize(grcomp.size() + 1);
            dfs2(top_sort[i], n_comp, grcomp, Tgr);
            n_comp++;
        }
    }

    return grcomp;
}