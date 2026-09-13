#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using std::cout;
using std::vector;
using std::priority_queue;
using std::queue;
using std::stack;
using std::pair;

const int N = 9;
const int INF = 9999;

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

vector<vector<int>> mstMat(N, vector<int>(N, 0));

void makeMST()
{
  vector<int> key(N, INF);
  vector<int> parent(N, -1);
  vector<int> used(N, 0);

  key[0] = 0;

  priority_queue<pair<int,int>> pq;
  pq.push({0, 0});

  while (!pq.empty())
  {
    pair<int,int> top = pq.top();
    pq.pop();

    int u = top.second;

    if (used[u] == 2) continue;
    used[u] = 2;

    for (int v = 0; v < N; v++)
    {
      if (mat[u][v] > 0 && used[v] != 2 && mat[u][v] < key[v])
      {
        key[v] = mat[u][v];
        parent[v] = u;
        pq.push({-key[v], v});
      }
    }
  }

  for (int v = 1; v < N; v++)
  {
    int p = parent[v];

    if (p != -1)
    {
      mstMat[p][v] = key[v];
      mstMat[v][p] = key[v];
    }
  }
}

vector<int> dfs(vector<vector<int>>& g, int start)
{
  vector<int> order;
  vector<int> used(N, 0);
  stack<int> Stack;

  Stack.push(start);

  while (!Stack.empty())
  {
    int node = Stack.top();
    Stack.pop();

    if (used[node] == 2) continue;
    used[node] = 2;

    order.push_back(node);

    for (int j = N - 1; j >= 0; j--)
    {
      if (g[node][j] > 0 && used[j] != 2)
      {
        Stack.push(j);
        used[j] = 1;
      }
    }
  }

  return order;
}

vector<int> dijkstra(int start)
{
  vector<int> dist(N, INF);
  dist[start] = 0;

  priority_queue<pair<int,int>> pq;
  pq.push({0, start});

  while (!pq.empty())
  {
    pair<int,int> top = pq.top();
    pq.pop();

    int d = -top.first;
    int u = top.second;

    if (d > dist[u]) continue;

    for (int v = 0; v < N; v++)
    {
      if (mat[u][v] > 0 && dist[u] + mat[u][v] < dist[v])
      {
        dist[v] = dist[u] + mat[u][v];
        pq.push({-dist[v], v});
      }
    }
  }

  return dist;
}

vector<int> degreesBFS(vector<vector<int>>& g)
{
  vector<int> used(N, 0);
  vector<int> degree(N, 0);
  queue<int> Queue;

  Queue.push(0);
  used[0] = 2;

  while (!Queue.empty())
  {
    int node = Queue.front();
    Queue.pop();

    for (int j = 0; j < N; j++)
    {
      if (g[node][j] > 0)
      {
        degree[node]++;

        if (used[j] != 2)
        {
          Queue.push(j);
          used[j] = 2;
        }
      }
    }
  }

  return degree;
}

void printVector(vector<int>& v)
{
  for (int i = 0; i < (int)v.size(); i++)
    cout << v[i] << " ";

  cout << "\n";
}

void printMST()
{
  for (int u = 0; u < N; u++)
  {
    for (int v = u + 1; v < N; v++)
    {
      if (mstMat[u][v] > 0)
        cout << u << " - " << v << " : " << mstMat[u][v] << "\n";
    }
  }
}

int main()
{
  makeMST();

  cout << "\n";
  printMST();

  cout << "\n\n";
  vector<int> dfsMST = dfs(mstMat, 0);
  printVector(dfsMST);

  cout << "\n\n";
  vector<int> dfsGraph = dfs(mat, 0);
  printVector(dfsGraph);

  cout << "\n\n";
  vector<int> dist = dijkstra(0);

  for (int i = 0; i < N; i++)
  {
    cout << "0 -> " << i << " = ";
    if (dist[i] == INF) cout << "INF\n";
    else cout << dist[i] << "\n";
  }

  cout << "\n\n";
  vector<int> deg = degreesBFS(mstMat);
  int sum = 0;

  for (int i = 0; i < N; i++)
  {
    cout << "vertex " << i << " : degree " << deg[i] << "\n";
    sum += deg[i];
  }

  cout << "\nAverage degree = " << (double)sum / N << "\n";

  return 0;
}