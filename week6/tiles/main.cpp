/// 2
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
  int w, h;
  std::cin >> w >> h;

  // two vertices for every potential position and two for source and sink
  graph G(2 * w * h + 2);
  edge_adder adder(G);

  std::vector<std::vector<bool>> vertices(h, std::vector<bool>(w));

  int totalPos = 0;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      char c;
      std::cin >> c;

      if (c == 'x') {
        continue;
      }

      vertices[i][j] = true;
      totalPos++;

      int currIndex = (i * w) + j;

      // since we are going through the grid top-down, left-right we only check
      // for previous edges
      if (i > 0 && vertices[i - 1][j]) {
        int newIndex = (i - 1) * w + j;
        // add edges such that flow can go from here to other, and from other to
        // here
        adder.add_edge(currIndex, newIndex + (w * h), 1);
        adder.add_edge(newIndex, currIndex + (w * h), 1);
      }
      if (j > 0 && vertices[i][j - 1]) {
        int newIndex = i * w + j - 1;
        adder.add_edge(currIndex, newIndex + (w * h), 1);
        adder.add_edge(newIndex, currIndex + (w * h), 1);
      }
    }
  }

  int v_source = 2 * w * h;
  int v_sink = 2 * w * h + 1;

  for (int i = 0; i < w * h; i++) {
    adder.add_edge(v_source, i, 1);
  }
  for (int i = w * h; i < 2 * w * h; i++) {
    adder.add_edge(i, v_sink, 1);
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

  if (flow != totalPos) {
    std::cout << "no\n";
  } else {
    std::cout << "yes\n";
  }
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