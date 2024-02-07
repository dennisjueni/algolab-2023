#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;

typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void testcase() {
  int n, m;
  std::cin >> n >> m;

  weighted_graph G(n);

  for (int i = 0; i < m; i++) {
    int s, t, w;
    std::cin >> s >> t >> w;
    boost::add_edge(s, t, w, G);
  }

  /* --- MST --- */
  std::vector<edge_desc> mst;
  int mstWeight = 0;

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end();
       it++) {
    mstWeight += get(boost::edge_weight_t(), G, *it);
  }

  /* --- Djikstra --- */
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(
      G, 0,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));

  std::vector<int>::iterator maxDist =
      std::max_element(dist_map.begin(), dist_map.end());

  std::cout << mstWeight << " " << *maxDist << "\n";
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