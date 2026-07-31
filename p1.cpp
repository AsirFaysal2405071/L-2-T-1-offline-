#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;
class solution
{
public:
    int bfs(int s, int t, vector<int> &parent, vector<vector<int>> &cap, vector<vector<int>> &adj)
    {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, int>> q;
        q.push({s, INF});

        while (!q.empty())
        {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adj[cur])
            {
                if (parent[next] == -1 && cap[cur][next])
                {
                    parent[next] = cur;
                    int new_flow = min(flow, cap[cur][next]);
                    if (next == t)
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }

        return 0;
    }

    int maxflow(int s, int t, vector<vector<int>> &cap, int n, vector<vector<int>> &adj)
    {
        int flow = 0;
        vector<int> parent(n);
        int new_flow;

        while (new_flow = bfs(s, t, parent,cap,adj))
        {
            flow += new_flow;
            int cur = t;
            while (cur != s)
            {
                int prev = parent[cur];
                cap[prev][cur] -= new_flow;
                cap[cur][prev] += new_flow;
                cur = prev;
            }
        }

        return flow;
    }
};
int main()
{
    int n;
    vector<vector<int>> cap;
    vector<vector<int>> adj;

    cin >> n;
    int b;
    cin >> b;
    cap.assign(n, vector<int>(n, 0));
    adj.assign(n, vector<int>());
    for (int i = 0; i < b; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        cap[a][b] = c;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    solution obj;
    int result = obj.maxflow(0, n - 1,cap,n,adj);
    cout << result << endl;
    return 0;
}