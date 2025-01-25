#include <iostream>
#include <vector>
#include <queue>
#include <cstdint>
#include <limits>

const uint64_t kInf = std::numeric_limits<uint64_t>::max();

class Graph {
 private:
  uint64_t num_vertices_;
  std::vector<std::vector<std::pair<uint64_t, uint64_t>>> adj_list_;

 public:
  explicit Graph(uint64_t vertices)
      : num_vertices_(vertices), adj_list_(vertices) {}

  void AddEdge(uint64_t u, uint64_t v, uint64_t weight) {
    adj_list_[u].emplace_back(v, weight);
  }

  const std::vector<std::vector<std::pair<uint64_t, uint64_t>>>& GetAdjList() const {
    return adj_list_;
  }

  uint64_t GetNumVertices() const {
    return num_vertices_;
  }
};

uint64_t MinBottlesLemonade(const Graph& graph, uint64_t start, uint64_t target) {
  uint64_t num_vertices = graph.GetNumVertices();
  if (start == target) return 0;

  std::vector<uint64_t> min_cost(num_vertices, kInf);
  min_cost[start] = 0;

  std::priority_queue<std::pair<uint64_t, uint64_t>, std::vector<std::pair<uint64_t, uint64_t>>, std::greater<>> pq;
  pq.emplace(0, start);

  while (!pq.empty()) {
    auto [current_cost, current_vertex] = pq.top();
    pq.pop();

    if (current_cost > min_cost[current_vertex]) continue;

    for (const auto& [next_vertex, weight] : graph.GetAdjList()[current_vertex]) {
      if (min_cost[next_vertex] > current_cost + weight) {
        min_cost[next_vertex] = current_cost + weight;
        pq.emplace(min_cost[next_vertex], next_vertex);
      }
    }
  }

  return min_cost[target] == kInf ? -1 : min_cost[target];
}

int main() {
  uint64_t a, b, m, x, y;
  std::cin >> a >> b >> m >> x >> y;

  Graph graph(m);

  for (uint64_t i = 0; i < m; ++i) {
    graph.AddEdge(i, (i + 1) % m, a);
    graph.AddEdge(i, (i * i + 1) % m, b);
  }

  uint64_t result = MinBottlesLemonade(graph, x, y);
  std::cout << result << std::endl;

  return 0;
}
