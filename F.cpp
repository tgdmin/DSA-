#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <algorithm>

template <typename T>
class Graph {
 public:
  explicit Graph(int vertices)
      : total_vertices(vertices),
        adjacency_list(vertices),
        reverse_adjacency_list(vertices) {}

  void AddEdge(T source, T destination) {
    adjacency_list[source].push_back(destination);
    reverse_adjacency_list[destination].push_back(source);
  }

  std::vector<std::vector<T>> FindStronglyConnectedComponents() {
    std::vector<bool> visited(total_vertices, false);
    std::stack<T> finish_order;

    // Perform the first DFS pass to compute finish order
    for (T i = 0; i < total_vertices; ++i) {
      if (!visited[i]) {
        PerformDFS(i, visited, finish_order);
      }
    }

    // Reset visited for the second DFS pass
    visited.assign(total_vertices, false);
    std::vector<std::vector<T>> scc_list;

    // Process nodes in reverse finish order to find SCCs
    while (!finish_order.empty()) {
      T current = finish_order.top();
      finish_order.pop();

      if (!visited[current]) {
        std::vector<T> scc;
        PerformReverseDFS(current, visited, scc);
        scc_list.push_back(scc);
      }
    }

    return scc_list;
  }

  Graph<T> CondenseGraph(const std::vector<std::vector<T>>& scc_list) {
    int new_vertex_count = scc_list.size();
    Graph<T> condensed_graph(new_vertex_count);
    std::vector<int> vertex_to_component(total_vertices, -1);

    for (size_t i = 0; i < scc_list.size(); ++i) {
      for (T vertex : scc_list[i]) {
        vertex_to_component[vertex] = static_cast<int>(i);
      }
    }

    for (T u = 0; u < total_vertices; ++u) {
      for (T v : adjacency_list[u]) {
        int source_component = vertex_to_component[u];
        int dest_component = vertex_to_component[v];
        if (source_component != dest_component) {
          condensed_graph.AddEdge(source_component, dest_component);
        }
      }
    }

    return condensed_graph;
  }

  std::pair<int, int> CalculateDegrees() {
    std::vector<int> in_degree(total_vertices, 0);
    std::vector<int> out_degree(total_vertices, 0);

    for (T u = 0; u < total_vertices; ++u) {
      for (T v : adjacency_list[u]) {
        out_degree[u]++;
        in_degree[v]++;
      }
    }

    int zero_in_degree = std::count(in_degree.begin(), in_degree.end(), 0);
    int zero_out_degree = std::count(out_degree.begin(), out_degree.end(), 0);

    return {zero_in_degree, zero_out_degree};
  }

 private:
  int total_vertices;
  std::vector<std::vector<T>> adjacency_list;
  std::vector<std::vector<T>> reverse_adjacency_list;

  void PerformDFS(T vertex, std::vector<bool>& visited, std::stack<T>& finish_order) {
    visited[vertex] = true;
    for (T neighbor : adjacency_list[vertex]) {
      if (!visited[neighbor]) {
        PerformDFS(neighbor, visited, finish_order);
      }
    }
    finish_order.push(vertex);
  }

  void PerformReverseDFS(T vertex, std::vector<bool>& visited, std::vector<T>& scc) {
    visited[vertex] = true;
    scc.push_back(vertex);
    for (T neighbor : reverse_adjacency_list[vertex]) {
      if (!visited[neighbor]) {
        PerformReverseDFS(neighbor, visited, scc);
      }
    }
  }
};

int main() {
  int vertices, edges;
  std::cin >> vertices >> edges;

  Graph<int> city_graph(vertices);

  for (int i = 0; i < edges; ++i) {
    int from, to;
    std::cin >> from >> to;
    city_graph.AddEdge(from - 1, to - 1);  // Convert to 0-based indexing
  }

  // Find all strongly connected components (SCCs)
  auto sccs = city_graph.FindStronglyConnectedComponents();

  // If there's only one SCC or no edges in the graph
  if (sccs.size() <= 1) {
    std::cout << 0 << std::endl;
    return 0;
  }

  // Create the condensed graph
  Graph<int> condensed_graph = city_graph.CondenseGraph(sccs);

  // Calculate the in-degrees and out-degrees for the condensed graph
  auto degrees = condensed_graph.CalculateDegrees();

  // Output the maximum of zero in-degrees or zero out-degrees
  std::cout << std::max(degrees.first, degrees.second) << std::endl;

  return 0;
}
