#include <fstream>
#include <sstream>

#include <bits/stdc++.h>

#define pb push_back

using namespace std;

bool
dfs(int u, vector<vector<int>> &adj, vector<int> &state, stack<int> &st)
{
    state[u] = 1;

    for (int v : adj[u]) {
        if (state[v] == 1)
            return true;
        if (state[v] == 0)
            if (dfs(v, adj, state, st))
                return true;
    }

    state[u] = 2;
    st.push(u);
    return false;
}

vector<int>
topoSort(vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<int> state(n, 0);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        if (state[i] == 0) {
            if (dfs(i, adj, state, st)) {
                cout << "Error: cicle detected!\n";
                return {};
            }
        }
    }

    vector<int> res;
    while (!st.empty()) {
        res.push_back(st.top());
        st.pop();
    }
    return res;
}


void
addEdge(vector<vector<int>> &adj, int u, int v)
{
    adj[u].pb(v);
}

int
main()
{
    ifstream file("input.txt");
    string line;

    vector<string> modules;
    vector<vector<string>> parsed;

    int n = 0;
    while (getline(file, line)) {
        if (line.empty())
            continue;

        size_t colon = line.find(':');
        if (colon == string::npos) {
            cerr << "Error: invalid format at line " << n + 1
                 << " (expected 'module : dep1 dep2 ...')\n";
            return 1;
        }

        string name = "";
        for (int i = 0; i < (int)colon; i++)
            if (line[i] != ' ' && line[i] != '\t')
                name += line[i];
        if (name.empty()) {
            cerr << "Error: missing module name at line " << n + 1 << "\n";
            return 1;
        }

        string rest = line.substr(colon + 1);
        istringstream iss(rest);
        vector<string> parts = {name};
        string token;
        while (iss >> token)
            parts.pb(token);

        modules.pb(parts[0]);
        parsed.pb(parts);

        n++;
    }

    vector<vector<int>> adj(n);

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)parsed[i].size(); j++) {
            auto it = find(modules.begin(), modules.end(), parsed[i][j]);
            if (it != modules.end()) {
                int depIdx = it - modules.begin();
                addEdge(adj, depIdx, i);
            }
        }
    }

    vector<int> res = topoSort(adj);
    for (int v : res)
        cout << modules[v] << "\n";
}
