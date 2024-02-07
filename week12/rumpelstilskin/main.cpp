#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
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
    graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

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
    w_map[e] = cost;      // new assign cost
    w_map[rev_e] = -cost; // new negative cost
  }
};

void testcase() {

  int numIslands;
  int numCitiesA, numCitiesB;
  int numInternalEdgesA, numInternalEdgesB;
  int numExternalEdgesA, numExternalEdgesB;
  int numStrawAvailableA, numStrawAvailableB;
  std::cin >> numIslands >> numCitiesA >> numCitiesB >> numInternalEdgesA >>
      numInternalEdgesB;
  std::cin >> numExternalEdgesA >> numExternalEdgesB >> numStrawAvailableA >>
      numStrawAvailableB;

  int maxPreference = 0;
  std::vector<int> preferenceScores = std::vector<int>(numIslands, 0);
  for (int i = 0; i < numIslands; i++) {
    std::cin >> preferenceScores[i];
    maxPreference = std::max(maxPreference, preferenceScores[i]);
  }

  // 2 * numIslands + numCitiesA + numCitiesB + source + sink
  graph G(2 * numIslands + numCitiesA + numCitiesB + 2);
  edge_adder adder(G);
  int source = 2 * numIslands + numCitiesA + numCitiesB;
  int sink = 2 * numIslands + numCitiesA + numCitiesB + 1;

  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  // Add internal edges A
  for (int i = 0; i < numInternalEdgesA; i++) {
    int u, v, c;
    std::cin >> u >> v >> c;

    adder.add_edge(2 * numIslands + u, 2 * numIslands + v, c, 0);
  }

  // Add internal edges B
  for (int i = 0; i < numInternalEdgesB; i++) {
    int u, v, c;
    std::cin >> u >> v >> c;

    // Maybe need to change the cost here!
    adder.add_edge(2 * numIslands + numCitiesA + v,
                   2 * numIslands + numCitiesA + u, c, 0);
  }

  // Add island connections
  for (int i = 0; i < numIslands; i++) {
    int preference = -1 * preferenceScores[i] + maxPreference;
    adder.add_edge(i, numIslands + i, 1, preference);
  }

  for (int i = 0; i < numExternalEdgesA; i++) {
    int u, v, c;
    std::cin >> u >> v >> c;

    adder.add_edge(2 * numIslands + u, v, c, 0);
  }

  for (int i = 0; i < numExternalEdgesB; i++) {
    int u, v, c;
    std::cin >> u >> v >> c;

    adder.add_edge(numIslands + v, 2 * numIslands + numCitiesA + u, c, 0);
  }

  adder.add_edge(source, 2 * numIslands, numStrawAvailableA, 0);
  adder.add_edge(2 * numIslands + numCitiesA, sink, numStrawAvailableB, 0);

  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int preference = boost::find_flow_cost(G);

  int flow = 0;
  out_edge_it e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G);
       e != eend; ++e) {
    flow += c_map[*e] - rc_map[*e];
  }

  preference = -1 * preference + flow * maxPreference;

  std::cout << flow << " " << preference << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}