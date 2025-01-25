#include <iostream>
#include <vector>
#include <string>

const int BITS_IN_BLOCK = 32;

class BitMatrix {
 public:
  explicit BitMatrix(int num_elements)
      : total_bits_(num_elements),
        storage_((num_elements + BITS_IN_BLOCK - 1) / BITS_IN_BLOCK, 0) {}

  void Enable(int position) {
    storage_[position / BITS_IN_BLOCK] |= (1u << (position % BITS_IN_BLOCK));
  }

  bool IsEnabled(int position) const {
    return (storage_[position / BITS_IN_BLOCK] & (1u << (position % BITS_IN_BLOCK))) != 0;
  }

  void Merge(const BitMatrix& other) {
    for (size_t i = 0; i < storage_.size(); ++i) {
      storage_[i] |= other.storage_[i];
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const BitMatrix& matrix) {
    for (int i = 0; i < matrix.total_bits_; ++i) {
      os << matrix.IsEnabled(i);
    }
    return os;
  }

 private:
  int total_bits_;
  std::vector<unsigned int> storage_;
};

class ReachabilityMatrix {
 public:
  explicit ReachabilityMatrix(const std::vector<BitMatrix>& adjacency_list)
      : num_nodes_(adjacency_list.size()), paths_(adjacency_list) {}

  void ComputePaths() {
    for (int pivot = 0; pivot < num_nodes_; ++pivot) {
      for (int start = 0; start < num_nodes_; ++start) {
        if (paths_[start].IsEnabled(pivot)) {
          paths_[start].Merge(paths_[pivot]);
        }
      }
    }
  }

  void PrintMatrix() const {
    for (const auto& row : paths_) {
      std::cout << row << std::endl;
    }
  }

 private:
  int num_nodes_;
  std::vector<BitMatrix> paths_;
};

int main() {
  int node_count;
  std::cin >> node_count;

  std::vector<BitMatrix> adjacency_list(node_count, BitMatrix(node_count));

  for (int i = 0; i < node_count; ++i) {
    std::string input;
    std::cin >> input;
    for (int j = 0; j < node_count; ++j) {
      if (input[j] == '1') {
        adjacency_list[i].Enable(j);
      }
    }
  }

  ReachabilityMatrix reachability(adjacency_list);
  reachability.ComputePaths();
  reachability.PrintMatrix();

  return 0;
}
