#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Graph class with adjacency list representation
template <typename T>
class Graph {
 public:
  explicit Graph(int vertices) { adjacency_list.resize(vertices); }

  void AddEdge(T source, T destination) {
    adjacency_list[source].push_back(destination);
    adjacency_list[destination].push_back(source);
  }

  T FindShortestCombinedPath(T src1, T src2, T target, int total_nodes) {
    vector<T> dist_src1 = PerformBFS(src1, total_nodes);
    vector<T> dist_src2 = PerformBFS(src2, total_nodes);
    vector<T> dist_target = PerformBFS(target, total_nodes);

    return ComputeMinDistance(total_nodes, dist_src1, dist_src2, dist_target);
  }

 private:
  vector<vector<T>> adjacency_list;

  // BFS to calculate shortest distance from a source node
  vector<T> PerformBFS(T start, int total_nodes) {
    vector<T> distances(total_nodes, INT_MAX);
    queue<T> q;

    distances[start] = 0;
    q.push(start);

    while (!q.empty()) {
      T current = q.front();
      q.pop();

      for (const T& neighbor : adjacency_list[current]) {
        if (distances[neighbor] == INT_MAX) {
          distances[neighbor] = distances[current] + 1;
          q.push(neighbor);
        }
      }
    }

    return distances;
  }

  // Compute the minimum combined distance for all nodes
  static T ComputeMinDistance(int total_nodes, const vector<T>& dist1, const vector<T>& dist2, const vector<T>& dist3) {
    T min_distance = INT_MAX;

    for (int i = 0; i < total_nodes; ++i) {
      if (dist1[i] != INT_MAX && dist2[i] != INT_MAX && dist3[i] != INT_MAX) {
        min_distance = min(min_distance, dist1[i] + dist2[i] + dist3[i]);
      }
    }

    return min_distance;
  }
};

int main() {
  int nodes, edges, leon, matilda, destination;
  cin >> nodes >> edges >> leon >> matilda >> destination;

  Graph<int> city_graph(nodes + 1);

  for (int i = 0; i < edges; ++i) {
    int u, v;
    cin >> u >> v;
    city_graph.AddEdge(u, v);
  }

  cout << city_graph.FindShortestCombinedPath(leon, matilda, destination, nodes + 1);
  return 0;
}