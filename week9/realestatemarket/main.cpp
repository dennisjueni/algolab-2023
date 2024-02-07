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

  int n, m, s;
  std::cin >> n >> m >> s;

  graph G(s + m + n + 2);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  int source = s + m + n;
  int sink = s + m + n + 1;

  for (int i = 0; i < s; i++) {
    int stateLimit;
    std::cin >> stateLimit;

    adder.add_edge(source, i, stateLimit, 0);
  }

  for (int i = 0; i < m; i++) {
    int statePerSite;
    std::cin >> statePerSite;

    adder.add_edge(statePerSite - 1, s + i, 1, 0);
  }

  std::vector<std::vector<int>> offers(n, std::vector<int>(m, 0));
  int maxOffer = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cin >> offers[i][j];
      maxOffer = std::max(maxOffer, offers[i][j]);
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {

      int offerPerHouse = maxOffer - offers[i][j];

      adder.add_edge(s + j, s + m + i, 1, offerPerHouse);
    }
    adder.add_edge(s + m + i, sink, 1, 0);
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

  cost = flow * maxOffer - cost;
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