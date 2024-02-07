#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}

  void add(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int n;
  std::cin >> n;

  graph G(n * n + 2);
  edge_adder adder(G);
  int source = n * n;
  int sink = n * n + 1;

  int sumPresent = 0;

  std::vector<int> present(n * n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {

      int currField = i * n + j;
      std::cin >> present[currField];

      if (!present[currField]) {
        continue;
      }

      // since the graph is bipartite we first compute max matching = min VC.
      if ((i + j) % 2 == 0) {
        adder.add(source, currField, 1);
      } else {
        adder.add(currField, sink, 1);
      }

      sumPresent++;
    }
  }

  auto conditional_add = [&adder, &present, &n](int currField, int i, int j,
                                                int c) {
    if (i >= 0 && i < n && j >= 0 && j < n && present[i * n + j]) {
      adder.add(currField, i * n + j, c);
    }
  };

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {

      int currField = i * n + j;

      if (present[currField] && (i + j) % 2 == 0) {
        conditional_add(currField, i - 2, j + 1, 1);
        conditional_add(currField, i - 2, j - 1, 1);
        conditional_add(currField, i + 2, j + 1, 1);
        conditional_add(currField, i + 2, j - 1, 1);

        conditional_add(currField, i - 1, j + 2, 1);
        conditional_add(currField, i - 1, j - 2, 1);
        conditional_add(currField, i + 1, j + 2, 1);
        conditional_add(currField, i + 1, j - 2, 1);
      }
    }
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);

  // std::cout << flow << " " << sumPresent << "\n";

  std::cout << sumPresent - flow << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
  return 0;
}