#include <vector>
#include <unordered_map>

using namespace std;

void sort_comp(vector<int>& comp, const vector<vector<int> >& graph) {
    int n = comp.size();

    unordered_map<int, int> num;
    for (int i = 0; i < n; ++i) {
        num[comp[i]] = i;
    }

    vector<vector<int> > dp((1 << n), vector<int>(n));
    vector<vector<int> > prev((1 << n), vector<int>(n, -1));

    dp[1 << 0][0] = 1;

    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int v = 0; v < n; ++v) {
            if (!dp[mask][v]) continue;
            for (auto u : graph[comp[v]]) {
                if (num.find(u) != num.end() && 
                    (mask | (1 << num[u])) != u) {
                    dp[mask | (1 << num[u])][u] = 1;
                    prev[mask | (1 << num[u])][u] = v;
                }
            }
        }
    }

    int v = -1;
    int mask = (1 << n) - 1;
    for (int i = 0; i < n; ++i) {
        if (dp[mask][i]) {
            bool f = false;
            for (auto u : graph[i]) {
                if (u == 0) {
                    f = true;
                    break;
                }
            }

            if (!f) continue;

            v = i;
            break;
        }
    }

    if (v == -1) {
        return;
    }

    vector<int> tmp;
    while (prev[mask][v] != -1) {
        tmp.push_back(v);
        auto tmp2 = mask ^ (1 << v);
        v = prev[mask][v];
        mask = tmp2;
    }
    tmp.push_back(v);
    comp = tmp;
}

void sort_by_hamilthonian_path(vector<vector<int> >& comp, const vector<vector<int> >& graph) {
    for (int i = 0; i < comp.size(); ++i) {
        sort_comp(comp[i], graph);
    }
}