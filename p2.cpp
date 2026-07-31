#include <bits/stdc++.h>
using namespace std;
struct flight
{
    int id;
    string d_c, a_c, d_t, a_t;
    flight() {}
    flight(int id, string d_c, string a_c, string d_t, string a_t)
        : id(id), d_c(d_c), a_c(a_c), d_t(d_t), a_t(a_t) {}
};
float diff(flight &x, flight &y)
{
    int t1 = stoi(x.a_t.substr(0, 2)) * 60 + stoi(x.a_t.substr(3));
    int t2 = stoi(y.d_t.substr(0, 2)) * 60 + stoi(y.d_t.substr(3));
    return (t2 - t1);
}

class solution
{

public:
    vector<int> par;
    int bfs(int n, vector<vector<int>> &adj, vector<vector<int>> &cap)
    {
        queue<pair<int, int>> q;
        fill(par.begin(), par.end(), -1);
        par[0] = -2;
        q.push({0, INT_MAX});
        while (!q.empty())
        {
            int x = q.front().first;
            int y = q.front().second;
            q.pop();
            for (auto &e : adj[x])
            {
                if (par[e] == -1 && cap[x][e])
                {
                    par[e] = x;
                    if (e == 2 * n + 1)
                        return min(y, cap[x][e]);

                    q.push({e, min(y, cap[x][e])});
                }
            }
        }
        return 0;
    }
    void maxflow(int n, vector<vector<int>> &cap, vector<vector<int>> &adj)
    {
        vector<vector<int>> result;
        par.resize(2 * n + 2);
        int flow = 0, newflow = 0;
        while (newflow = bfs(n, adj, cap))
        {
            flow += newflow;
            int curr = 2 * n + 1;
            vector<int> p;
            while (curr != 0)
            {
                if (curr != 0 && curr != 2 * n + 1)
                    p.push_back(curr > n ? curr - n : curr);
                int prev = par[curr];
                cap[prev][curr]--;
                cap[curr][prev]++;
                curr = prev;
            }
            reverse(p.begin(), p.end());
            result.push_back(p);
        }
        cout << "Number of Aircraft: " << n - flow << endl;
        vector<vector<int>> path = result;
        vector<bool> removed(path.size(), false);

        bool changed = true;

        while (changed)
        {
            changed = false;

            for (int i = 0; i < path.size(); i++)
            {
                if (removed[i])
                    continue;

                for (int j = 0; j < path.size(); j++)
                {
                    if (i == j || removed[j])
                        continue;

                    if (path[i].back() == path[j].front())
                    {
                        copy(path[j].begin() + 1,
                             path[j].end(),
                             back_inserter(path[i]));

                        removed[j] = true;
                        changed = true;
                    }
                }
            }
        }

        int aircraft = 1;
        vector<bool> seen(n + 1, false);

        for (int i = 0; i < path.size(); i++)
        {
            if (removed[i])
                continue;

            cout << "Aircraft " << aircraft++ << ": ";

            for (int j = 0; j < path[i].size(); j++)
            {
                seen[path[i][j]] = true;

                cout << "F" << path[i][j];

                if (j + 1 != path[i].size())
                    cout << " -> ";
            }

            cout <<endl;
        }

        for (int id = 1; id <= n; id++)
        {
            if (!seen[id])
            {
                cout << "Aircraft " << aircraft++ << ": F" << id << '\n';
            }
        }
    }
};
int main()
{
    int n;
    cin >> n;
    vector<flight> f(n + 1);
    vector<vector<int>> adj(2 * n + 2);
    for (int i = 0; i < n; i++)
    {
        string x, d_c, a_c, d_t, a_t;

        cin >> x >> d_c >> a_c >> d_t >> a_t;

        int p = stoi(x.substr(1));

        f[p] = flight(p, d_c, a_c, d_t, a_t);
    }
    vector<vector<int>> cap(2 * n + 2, vector<int>(2 * n + 2, 0));

    sort(f.begin() + 1, f.end(), [](flight &x, flight &y)
         {
         int t1 = stoi(x.a_t.substr(0, 2)) * 60 + stoi(x.a_t.substr(3));
    int t2 = stoi(y.a_t.substr(0, 2)) * 60 + stoi(y.a_t.substr(3));
    return t1<t2; });

    for (int i = 1; i <= n; i++)
    {
        cap[0][f[i].id] = 1;
        adj[0].push_back(f[i].id);

        cap[f[i].id + n][2 * n + 1] = 1;
        adj[f[i].id + n].push_back(2 * n + 1);
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (i == j)
                continue;

            if (f[i].a_c == f[j].d_c && diff(f[i], f[j]) >= 180)
            {
                cap[f[i].id][f[j].id + n] = 1;
                adj[f[i].id].push_back(f[j].id + n);
            }
        }
    }
    solution obj;
    obj.maxflow(n, cap, adj);
    return 0;
}
