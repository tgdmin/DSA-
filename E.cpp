#include <iostream>
#include <vector>
#include <algorithm>  

using namespace std;

template <typename T>
class TopologicalSort {
 public:
  TopologicalSort(int vertex_count)
      : vertex_count(vertex_count), adjacency_list(vertex_count), in_degrees(vertex_count, 0) {}

  void AddEdge(T start, T end) {
    adjacency_list[start].push_back(end);
    ++in_degrees[end];
  }

  bool Sort(vector<int>& result) {
    vector<bool> visited(vertex_count, false);
    vector<bool> recursion_stack(vertex_count, false);

    for (T i = 0; i < vertex_count; ++i) {
      if (!visited[i] && !DFS(i, visited, recursion_stack, result)) {
        return false;
      }
    }

    reverse(result.begin(), result.end());
    return true;
  }

 private:
  T vertex_count;
  vector<vector<T>> adjacency_list;
  vector<int> in_degrees;

  bool DFS(T node, vector<bool>& visited, vector<bool>& recursion_stack, vector<int>& result) {
    visited[node] = true;
    recursion_stack[node] = true;

    for (T neighbor : adjacency_list[node]) {
      if (!visited[neighbor] && !DFS(neighbor, visited, recursion_stack, result)) {
        return false;
      } else if (recursion_stack[neighbor]) {
        return false;
      }
    }

    recursion_stack[node] = false;
    result.push_back(node);
    return true;
  }
};

int main() {
  int vertex_count, edge_count;
  cin >> vertex_count >> edge_count;

  TopologicalSort<int> graph(vertex_count);
  vector<int> sorted_vertices;

  for (int i = 0; i < edge_count; ++i) {
    int source, destination;
    cin >> source >> destination;
    graph.AddEdge(source, destination);
  }

  if (graph.Sort(sorted_vertices)) {
    cout << "YES" << endl;
    for (int vertex : sorted_vertices) {
      cout << vertex << " ";
    }
  } else {
    cout << "NO";
  }

  return 0;
}
