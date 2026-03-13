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

vector<string>
split(const string &s, char delim)
{
    istringstream iss(s);
    vector<string> parts;
    string part;
    while (getline(iss, part, delim))
        parts.pb(part);

    return parts;
}

int
main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    // input size (num of lines)
    int n = 2;

    vector<string> modules;
    vector<vector<string>> parsed;
    vector<vector<int>> adj(n);

    for (int i = 0; i < n; i++) {
        string line;
        cin >> line;
        vector<string> parts = split(line, ',');
        modules.pb(parts[0]);
        parsed.pb(parts);
    }

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
