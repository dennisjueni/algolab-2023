#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

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
    w_map[e] = cost;      // new assign cost
    w_map[rev_e] = -cost; // new negative cost
  }
};

void testcase() {

  int numStops, numMissions, maxFlow;
  std::cin >> numStops >> numMissions >> maxFlow;

  graph G(numStops + 2);
  edge_adder adder(G);

  int source = numStops;
  int sink = numStops + 1;

  for (int i = 0; i < numStops - 1; i++) {
    adder.add_edge(i, i + 1, maxFlow, 128);
  }

  for (int i = 0; i < numMissions; i++) {
    int x, y, prio;
    std::cin >> x >> y >> prio;

    int adjustedPrio = (-1 * prio) + (128 * (y - x));
    adder.add_edge(x, y, 1, adjustedPrio);
  }

  adder.add_edge(source, 0, maxFlow, 0);
  adder.add_edge(numStops - 1, sink, maxFlow, 0);

  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);

  std::cout << ((numStops - 1) * maxFlow * 128) - cost << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}