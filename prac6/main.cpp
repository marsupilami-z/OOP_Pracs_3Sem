#include <iostream>
#include <vector>
#include <queue>

using std::cout;
using std::vector;
using std::queue;
using std::min;

const int N = 9;

vector<vector<int>> mat = {
  { 0, 7, 6, 4, 7, 0, 9, 2, 7 },
  { 7, 0, 7, 6, 8, 5, 7, 4, 6 },
  { 6, 7, 0, 6, 2, 1, 8, 6, 0 },
  { 4, 6, 6, 0, 5, 8, 4, 7, 1 },
  { 7, 8, 2, 5, 0, 0, 0, 2, 5 },
  { 0, 5, 1, 8, 0, 0, 2, 2, 3 },
  { 9, 7, 8, 4, 0, 2, 0, 6, 1 },
  { 2, 4, 6, 7, 2, 2, 6, 0, 3 },
  { 7, 6, 0, 1, 5, 3, 1, 3, 0 }
};

vector<int> path;
vector<int> used(N, 0);

bool bfs(vector<vector<int>>& rcap, vector<int>& parent, int s, int t)
{
  vector<int> visited(N, 0);
  queue<int> q;
  q.push(s);

  visited[s] = 1;
  parent[s] = -1;

  while (!q.empty())
  {
    int u = q.front();
    q.pop();

    for (int v = 0; v < N; v++)
    {
      if (visited[v] == 0 && rcap[u][v] > 0)
      {
        parent[v] = u;
        visited[v] = 1;
        q.push(v);

        if (v == t) return true;
      }
    }
  }

  return false;
}

int findFlow(int s, int t)
{
  vector<vector<int>> rcap = mat;
  vector<int> parent(N, -1);

  int maxFlow = 0;

  while (bfs(rcap, parent, s, t))
  {
    int pathFlow = 9999;

    for (int v = t; v != s; v = parent[v])
      pathFlow = min(pathFlow, rcap[parent[v]][v]);

    vector<int> route;

    for (int v = t; v != s; v = parent[v])
      route.push_back(v);

    route.push_back(s);

    for (int i = (int)route.size() - 1; i >= 0; i--)
    {
      cout << route[i];

      if (i > 0) cout << " -> ";
    }

    cout << " : flow " << pathFlow << "\n";

    for (int v = t; v != s; v = parent[v])
    {
      int u = parent[v];
      rcap[u][v] -= pathFlow;
      rcap[v][u] += pathFlow;
    }

    maxFlow += pathFlow;
  }

  return maxFlow;
}

bool hamilton(int u)
{
  path.push_back(u);
  used[u] = 1;

  if ((int)path.size() == N) return true;

  for (int v = 0; v < N; v++)
  {
    if (mat[u][v] > 0 && used[v] == 0)
    {
      if (hamilton(v)) return true;
    }
  }

  path.pop_back();
  used[u] = 0;
  return false;
}

void coloring()
{
  vector<int> color(N, -1);
  color[0] = 0;

  for (int u = 1; u < N; u++)
  {
    vector<int> busy(N, 0);

    for (int v = 0; v < N; v++)
    {
      if (mat[u][v] > 0 && color[v] != -1)
        busy[color[v]] = 1;
    }

    int c = 0;
    while (busy[c]) c++;

    color[u] = c;
  }

  for (int i = 0; i < N; i++)
    cout << "vertex " << i << " : color " << color[i] << "\n";

  int maxColor = 0;
  for (int i = 0; i < N; i++)
    if (color[i] > maxColor) maxColor = color[i];

  cout << "Colors used: " << maxColor + 1 << "\n";
}

int main()
{
  cout << "\n";
  int total = findFlow(0, 8);
  cout << "Total flow = " << total << "\n";

  cout << "\n\n";
  if (hamilton(0))
  {
    for (int i = 0; i < (int)path.size(); i++)
    {
      cout << path[i];

      if (i < (int)path.size() - 1) cout << " -> ";
    }
    cout << "\n";
  }
  else
  {
    cout << "No Hamiltonian path\n";
  }

  cout << "\n\n";
  coloring();

  return 0;
}