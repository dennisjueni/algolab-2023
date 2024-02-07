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
  int n, m;
  std::cin >> n >> m;

  // one vertex per game, one vertex per player and two for source and sink
  graph G(n + m + 2);
  edge_adder adder(G);
  int source = n + m;
  int sink = n + m + 1;

  for (int i = 0; i < m; i++) {

    // First always add an edge between source and the game with capacity 1
    adder.add_edge(source, i, 1);

    // Check who won and played the game
    int a, b, c;
    std::cin >> a >> b >> c;

    int player1Index = m + a;
    int player2Index = m + b;

    if (c == 0) {
      // If we do not know the outcome, the point could go to either one of the
      // two players
      adder.add_edge(i, player1Index, 1);
      adder.add_edge(i, player2Index, 1);
    } else if (c == 1) {
      adder.add_edge(i, player1Index, 1);
    } else if (c == 2) {
      adder.add_edge(i, player2Index, 1);
    }
  }

  int totalScore = 0;

  for (int i = 0; i < n; i++) {
    int score;
    std::cin >> score;
    totalScore += score;
    adder.add_edge(m + i, sink, score);
  }

  if (totalScore != m) {
    std::cout << "no\n";
    return;
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);

  if (flow != m) {
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