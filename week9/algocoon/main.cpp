/// 1
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

  void add_edge(int from, int to, long capacity) {
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
  int n, m;
  std::cin >> n >> m;

  graph G(n);
  edge_adder adder(G);

  for (int i = 0; i < m; i++) {
    int a, b, c;
    std::cin >> a >> b >> c;

    adder.add_edge(a, b, c);
  }

  long minCut = std::numeric_limits<long>::max();

  // Since we know S & T must both contain at least 1 vertex,
  // we know we can fix one specific vertex and check the maxflow to all other
  // pairs.
  for (int i = 0; i < n; i++) {
    long flow1 = boost::push_relabel_max_flow(G, i, 0);
    long flow2 = boost::push_relabel_max_flow(G, 0, i);

    minCut = std::min({minCut, flow1, flow2});
  }

  std::cout << minCut << "\n";
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