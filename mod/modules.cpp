#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

#define pb push_back

using namespace std;

vector<int>
topoSort(vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<int> indegree(n, 0);
    queue<int> q;
    vector<int> list;

    for (int i = 0; i < n; i++) {
        for (int next : adj[i])
            indegree[next]++;
    }

    for (int i = 0; i < n; i++)
        if (indegree[i] == 0)
            q.push(i);

    while (!q.empty()) {
        int top = q.front();
        q.pop();
        list.push_back(top);
        for (int next : adj[top]) {
            indegree[next]--;
            if (indegree[next] == 0)
                q.push(next);
        }
    }

    if ((int)list.size() != n) {
        cerr << "Error: cicle detected\n";
        return {};
    }

    return list;
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
        for (char& c : line) if (c == ':') c = ' ';

        istringstream iss(line);
        vector<string> parts;
        string token;

        while (iss >> token) parts.pb(token);

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
