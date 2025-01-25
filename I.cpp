#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

struct Link {
  int target_node; 
  double success_rate; 
};

class NetworkGraph {
 private:
  std::vector<std::vector<Link>> adjacency_list_; 
 public:
  explicit NetworkGraph(int num_nodes) : adjacency_list_(num_nodes + 1) {}

  void AddEdge(int source, int destination, double success_rate) {
    adjacency_list_[source].push_back({destination, success_rate});
    adjacency_list_[destination].push_back({source, success_rate});
  }

  const std::vector<std::vector<Link>>& GetAdjacencyList() const {
    return adjacency_list_;
  }
};

double FindMinimumRisk(const std::vector<std::vector<Link>>& graph, int start_node,
                        int end_node) {
  std::vector<double> risk(graph.size(), 1.0); 
  risk[start_node] = 0.0;

  std::priority_queue<std::pair<double, int>,
                      std::vector<std::pair<double, int>>,
                      std::greater<std::pair<double, int>>>
      pq;
  pq.push({0.0, start_node});

  while (!pq.empty()) {
    double current_risk = pq.top().first; 
    int current_node = pq.top().second; 
    pq.pop();

    for (const Link& edge : graph[current_node]) {
      double new_risk = current_risk + edge.success_rate -
                        current_risk * edge.success_rate;
      if (new_risk < risk[edge.target_node]) {
        risk[edge.target_node] = new_risk;
        pq.push({new_risk, edge.target_node});
      }
    }
  }

  return risk[end_node];
}

int main() {
  int total_nodes; 
  int total_edges; 
  int start; 
  int finish; 
  std::cin >> total_nodes >> total_edges >> start >> finish;

  NetworkGraph graph(total_nodes);

  for (int i = 0; i < total_edges; ++i) {
    int from_node; 
    int to_node; 
    double probability;
    std::cin >> from_node >> to_node >> probability;
    graph.AddEdge(from_node, to_node, probability / 100.0);
  }

  const auto& adjacency_list = graph.GetAdjacencyList();
  double minimum_risk = FindMinimumRisk(adjacency_list, start, finish);

  std::cout << std::setprecision(2) << minimum_risk << std::endl;

  return 0;
}
