#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <iostream>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;

void testcase() {

  int n, m;
  std::cin >> n >> m;

  graph G(n);

  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    boost::add_edge(u, v, G);
  }

  std::map<graph::edge_descriptor, int> component_map;
  auto c = boost::make_assoc_property_map(component_map);

  biconnected_components(G, c);

  std::vector<int> edgesPerComponent = std::vector<int>(m, 0);
  std::map<int, std::pair<int, int>> reverseComponentMap;

  for (auto p : component_map) {
    edgesPerComponent[p.second] += 1;
    int v1 = boost::source(p.first, G);
    int v2 = boost::target(p.first, G);
    if (v1 > v2) {
      std::swap(v1, v2);
    }
    reverseComponentMap[p.second] = {v1, v2};
  }

  std::vector<std::pair<int, int>> edges = std::vector<std::pair<int, int>>();
  int numBridges = 0;

  for (int i = 0; i < m; i++) {
    if (edgesPerComponent[i] == 1) {
      numBridges++;
      edges.push_back(reverseComponentMap[i]);
    }
  }

  std::cout << numBridges << "\n";

  std::sort(edges.begin(), edges.end());

  for (unsigned int i = 0; i < edges.size(); i++) {
    std::cout << edges[i].first << " " << edges[i].second << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}