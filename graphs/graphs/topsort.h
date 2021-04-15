#include <vector>

using namespace std;

int maxcnt;
vector<int> order;
int cnt_c;

void dfs3(int v, vector<vector<int> >& graph, vector<int>& used) {
    used[v] = 1;
    for (auto u : graph[v]) {
        if (!used[u]) {
            dfs3(u, graph, used);
        }
    }
    order[v] = cnt_c--;
}

void dfs4(int v, int depth, vector<vector<int> >& graph, vector<vector<int> >& levels, vector<int>& used) {
    used[v] = 2;
    if (levels.size() <= depth) {
        levels.resize(levels.size() + 1);
    }
    levels[depth].push_back(v);
    for (auto u : graph[v]) {
        if (used[u] != 2) {
            dfs4(u, depth + 1, graph, levels, used);
        }
    }
}

vector<vector<int> > topsort(vector<vector<int> >& comp, vector<vector<int> >& graph, vector<int>& num) {
    vector<int> used;

    vector<vector<int> > tree(comp.size());
    for (int i = 0; i < graph.size(); ++i) {
        for (auto u : graph[i]) {
            if (num[i] != num[u]) {
                tree[num[i]].push_back(num[u]);
            }
        }
    }

    vector<vector<int> > levels;
    used.resize(comp.size());
    cnt_c = comp.size() - 1;
    order.resize(comp.size());
    for (int i = 0; i < comp.size(); ++i) {
        if (!used[i]) {
            dfs3(i, tree, used);
        }
    }

    int root = 0;
    for (int i = 0; i < comp.size(); ++i) {
        if (order[i] == 0) {
            root = i;
            break;
        }
    }

    dfs4(root, 0, tree, levels, used);

    for (int i = 0; i < levels.size(); ++i) {
        if (levels[i].size() > maxcnt) {
            maxcnt = levels[i].size();
        }
    }

    return levels;
}