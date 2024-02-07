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
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int m, n, k, C;
  std::cin >> m >> n >> k >> C;

  // 2 vertices for every crossing plus 2 for source and sink
  graph G(2 * n * m + 2);
  edge_adder adder(G);
  int source = 2 * n * m;
  int sink = 2 * n * m + 1;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {

      int inVertex = i * m + j;
      int outVertex = n * m + inVertex;
      adder.add_edge(inVertex, outVertex, C);

      if (i == 0 && (j == 0 || j == m - 1)) {
        // this means from the crossing we can get to the outside in two ways
        adder.add_edge(outVertex, sink, 2);
      } else if (i == n - 1 && (j == 0 || j == m - 1)) {
        adder.add_edge(outVertex, sink, 2);
      } else if (i == 0 || j == 0 || i == n - 1 || j == m - 1) {
        // This means we can directly go from the outVertex to the outside
        // through a pathway
        adder.add_edge(outVertex, sink, 1);
      }

      // We again only look up and to the left to add pathways, since we "add"
      // vertices in that order
      if (i != 0) {
        int topInCrossing = inVertex - m;
        int topOutCrossing = outVertex - m;
        adder.add_edge(outVertex, topInCrossing, 1);
        adder.add_edge(topOutCrossing, inVertex, 1);
      }

      if (j != 0) {
        int leftInCrossing = inVertex - 1;
        int leftOutCrossing = outVertex - 1;
        adder.add_edge(outVertex, leftInCrossing, 1);
        adder.add_edge(leftOutCrossing, inVertex, 1);
      }
    }
  }

  // Read in and place the knights
  for (int i = 0; i < k; i++) {
    int mPos, nPos;
    std::cin >> mPos >> nPos;

    int knightInVertex = nPos * m + mPos;

    adder.add_edge(source, knightInVertex, 1);
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);

  std::cout << flow << "\n";
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