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

  void update_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::edge(from, to, G).first;
    const edge_desc rev_e = boost::edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;      // new assign cost
    w_map[rev_e] = -cost; // new negative cost
  }
};

void testcase() {
  int numCities, numGuides, budget, start, end;
  std::cin >> numCities >> numGuides >> budget >> start >> end;

  graph G(numCities + 1);
  edge_adder adder(G);
  int source = numCities;

  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  int totalCapacity = 0;

  for (int i = 0; i < numGuides; i++) {
    int x, y, cost, capacity;
    std::cin >> x >> y >> cost >> capacity;

    adder.add_edge(x, y, capacity, cost);

    if (x == start) {
      totalCapacity += capacity;
    }
  }

  adder.add_edge(source, start, 0, 0);

  int left = 0;
  int right = totalCapacity;
  int middle = (right + left) / 2;

  int maxLuggages = 0;

  while (left <= right) {
    adder.update_edge(source, start, middle, 0);

    boost::successive_shortest_path_nonnegative_weights(G, source, end);
    int cost = boost::find_flow_cost(G);

    if (cost > budget) {
      right = middle - 1;
    } else {
      // calculate flow
      int flow = 0;
      out_edge_it e, eend;
      for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G);
           e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];
      }

      maxLuggages = flow;
      left = middle + 1;
    }
    middle = (right + left) / 2;
  }

  std::cout << maxLuggages << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}
