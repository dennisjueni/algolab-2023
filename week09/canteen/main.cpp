#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<
            boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                            boost::property<boost::edge_weight_t, long>>>>>
    graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

// Custom edge adder class
class edge_adder {
  graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost; // new negative cost
  }
};

void testcase() {

  int n;
  std::cin >> n;

  graph G(n + 2);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  int source = n;
  int sink = n + 1;

  int totalNumStudents = 0;

  for (int i = 0; i < n; i++) {
    int a, c;
    std::cin >> a >> c;

    adder.add_edge(source, i, a, c);
  }

  for (int i = 0; i < n; i++) {
    int s, p;
    std::cin >> s >> p;

    adder.add_edge(i, sink, s, -p + 20);

    totalNumStudents += s;
  }

  for (int i = 0; i < n - 1; i++) {
    int v, e;
    std::cin >> v >> e;

    adder.add_edge(i, i + 1, v, e);
  }

  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);
  int flow = 0;

  out_edge_it e;
  out_edge_it eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G);
       e != eend; ++e) {
    flow += c_map[*e] - rc_map[*e];
  }

  if (flow == totalNumStudents) {
    std::cout << "possible ";
  } else {
    std::cout << "impossible ";
  }

  // revert making all edges positive
  cost = -cost + 20 * flow;

  std::cout << flow << " " << cost << "\n";
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