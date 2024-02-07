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
  int l, p;
  std::cin >> l >> p;

  // one vertex vor every location, one vertex for every path and source & sink
  graph G(l + 2);
  edge_adder adder(G);

  int source = l;
  int sink = l + 1;

  std::vector<int> supplyPerLocation(l);
  std::vector<int> demandPerLocation(l);

  int totalDemand = 0;

  for (int i = 0; i < l; i++) {
    int currentSoldiers, neededSoldiers;
    std::cin >> currentSoldiers >> neededSoldiers;

    supplyPerLocation[i] = currentSoldiers;
    demandPerLocation[i] = neededSoldiers;
  }

  for (int i = 0; i < p; i++) {
    int from, to, minNum, maxNum;
    std::cin >> from >> to >> minNum >> maxNum;

    adder.add_edge(from, to, maxNum - minNum);

    demandPerLocation[from] += minNum;
    supplyPerLocation[to] += minNum;
  }

  for (int i = 0; i < l; i++) {
    int supply = supplyPerLocation[i] - demandPerLocation[i];
    int demand = demandPerLocation[i] - supplyPerLocation[i];
    if (supply > 0) {
      adder.add_edge(source, i, supply);
    } else if (demand > 0) {
      adder.add_edge(i, sink, demand);

      totalDemand += demand;
    }
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);

  if (flow != totalDemand) {
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