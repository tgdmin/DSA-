#include <iostream>
#include <queue>
#include <vector>
#include <limits>

// Define a constant representing a very large value
const int kMax = 1e6;

// Struct representing an edge in the graph
struct Edge {
    int to;    // Destination vertex
    int cost;  // Cost of traveling along this edge
};

// Class representing a graph
class Graph {
private:
    int num_vertices_; // Number of vertices in the graph
    std::vector<std::vector<Edge>> adj_list_; // Adjacency list representation

public:
    // Constructor initializes the graph with the given number of vertices
    explicit Graph(int n) : num_vertices_(n), adj_list_(n + 1) {}

    // Adds a directed edge to the graph
    void AddEdge(int from, int to, int cost) {
        adj_list_[from].push_back({to, cost});
    }

    // Returns the adjacency list of the graph
    const std::vector<std::vector<Edge>>& GetAdjList() const {
        return adj_list_;
    }

    // Returns the number of vertices in the graph
    int GetNumVertices() const {
        return num_vertices_;
    }
};

// Function implementing Dijkstra's algorithm with a constraint on the number of flights
int LimitedFlightDijkstra(int n, const Graph& graph, int k, int start, int end) {
    // Create a 2D vector to track the shortest distances with up to k flights
    std::vector<std::vector<int>> distances(n + 1, std::vector<int>(k + 1, kMax));
    distances[start][0] = 0;

    // Min-heap priority queue to store {current cost, current vertex, flights used}
    using State = std::tuple<int, int, int>;
    std::priority_queue<State, std::vector<State>, std::greater<>> pq;

    // Start with the source vertex
    pq.push({0, start, 0});

    while (!pq.empty()) {
        auto [cost, current, flights_taken] = pq.top();
        pq.pop();

        // If the destination vertex is reached, return the cost
        if (current == end) {
            return cost;
        }

        // Explore all adjacent edges if flights limit allows
        for (const Edge& edge : graph.GetAdjList()[current]) {
            if (flights_taken < k) {
                int new_cost = cost + edge.cost;
                if (new_cost < distances[edge.to][flights_taken + 1]) {
                    distances[edge.to][flights_taken + 1] = new_cost;
                    pq.push({new_cost, edge.to, flights_taken + 1});
                }
            }
        }
    }

    // If no valid path is found, return -1
    return -1;
}

int main() {
    int n;     // Number of vertices
    int m;     // Number of edges
    int k;     // Maximum number of flights
    int start; // Start vertex
    int end;   // End vertex

    // Input the number of vertices, edges, max flights, start, and end vertices
    std::cin >> n >> m >> k >> start >> end;

    // Initialize the graph
    Graph graph(n);

    // Input the edges of the graph
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        std::cin >> from >> to >> cost;
        graph.AddEdge(from, to, cost);
    }

    // Run the limited flight Dijkstra algorithm and print the result
    int result = LimitedFlightDijkstra(n, graph, k, start, end);
    std::cout << result << std::endl;

    return 0;
}
