#include <algorithm>
#include <iostream>
#include <vector>

const int MAX_NODES = 53;
int graph[MAX_NODES][MAX_NODES];

// Perform a Depth-First Search to explore connected components
void performDFS(int node, std::vector<bool>& visited, std::vector<int>& currentComponent) {
    visited[node] = true;
    currentComponent.push_back(node);

    for (int neighbor = 0; neighbor < MAX_NODES; ++neighbor) {
        if (graph[node][neighbor] == 1 && !visited[neighbor]) {
            performDFS(neighbor, visited, currentComponent);
        }
    }
}

// Find the largest group of fully connected nodes using bitmasking
std::vector<int> findMaxFullyConnectedGroup(int n) {
    std::vector<bool> validSubsets(1 << n, false); // Valid subsets of nodes
    std::vector<int> adjacencyMask(n, 0);         // Adjacency masks for nodes
    std::vector<int> largestGroup;                // Resultant largest group

    // Build adjacency masks for each node
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph[i][j] == 1) {
                adjacencyMask[i] |= (1 << j);
            }
        }
    }

    // Determine valid subsets using dynamic programming
    for (int subset = 1; subset < (1 << n); ++subset) {
        if ((subset & (subset - 1)) == 0) { // Single-node subsets are valid
            validSubsets[subset] = true;
        } else {
            int lowestBitNode = __builtin_ctz(subset); // Rightmost set bit
            int subsetWithoutNode = subset ^ (1 << lowestBitNode);

            if (validSubsets[subsetWithoutNode] &&
                (subsetWithoutNode & adjacencyMask[lowestBitNode]) == subsetWithoutNode) {
                validSubsets[subset] = true;
            }
        }
    }

    // Identify the largest valid subset
    for (int subset = 1; subset < (1 << n); ++subset) {
        if (validSubsets[subset] && __builtin_popcount(subset) > largestGroup.size()) {
            largestGroup.clear();
            for (int i = 0; i < n; ++i) {
                if (subset & (1 << i)) {
                    largestGroup.push_back(i);
                }
            }
        }
    }

    return largestGroup;
}

// Wrapper to handle complete graphs or fallback to general solutions
std::vector<int> findLargestGroup(int n) {
    bool isCompleteGraph = true;

    // Check if the graph is fully connected
    for (int i = 0; i < n && isCompleteGraph; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && graph[i][j] != 1) {
                isCompleteGraph = false;
                break;
            }
        }
    }

    if (isCompleteGraph) {
        // Use DFS to find the largest connected component for a complete graph
        std::vector<bool> visited(MAX_NODES, false);
        std::vector<int> largestGroup;

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                std::vector<int> currentComponent;
                performDFS(i, visited, currentComponent);
                if (currentComponent.size() > largestGroup.size()) {
                    largestGroup = std::move(currentComponent);
                }
            }
        }
        return largestGroup;
    } else {
        // Use bitmasking for other cases
        return findMaxFullyConnectedGroup(n);
    }
}

int main() {
    int n;
    std::cin >> n;

    // Read adjacency matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> graph[i][j];
        }
    }

    // Determine the largest group and print the result
    std::vector<int> largestGroup = findLargestGroup(n);
    std::sort(largestGroup.begin(), largestGroup.end());
    
    for (size_t i = 0; i < largestGroup.size(); ++i) {
        std::cout << largestGroup[i];
        if (i < largestGroup.size() - 1) {
            std::cout << " ";
        }
    }

    return 0;
}
