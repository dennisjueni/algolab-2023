#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>
#include <iostream>
#include <map>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;

void testcase() {
  int n, m, k, T;
  std::cin >> n >> m >> k >> T;

  std::vector<bool> teleportations(n, false);
  weighted_graph G(2 * n);

  for (int i = 0; i < T; i++) {
    int t;
    std::cin >> t;
    teleportations[t] = true;
  }

  for (int i = 0; i < m; i++) {
    int u, v, c;
    std::cin >> u >> v >> c;
    boost::add_edge(v, u, c, G);
  }

  std::vector<int> componentMap(2 * n);

  int num = boost::strong_components(
      G, boost::make_iterator_property_map(componentMap.begin(),
                                           boost::get(boost::vertex_index, G)));

  std::map<int, std::vector<int>> componentVertices;

  for (int i = 0; i < n; i++) {
    int componentId = componentMap[i];

    if (teleportations[i]) {
      componentVertices[componentId].push_back(i);
    }
  }

  for (int i = 0; i < num; i++) {
    std::vector<int> vertices = componentVertices[i];
    int len = vertices.size();

    if (len > 1) {
      boost::add_edge(vertices[0], vertices[0] + n, len - 1, G);
      boost::add_edge(vertices[0] + n, vertices[0], 0, G);
      for (int j = 1; j < len; j++) {
        boost::add_edge(vertices[j - 1] + n, vertices[j] + n, 0, G);
        boost::add_edge(vertices[j] + n, vertices[j - 1] + n, 0, G);

        boost::add_edge(vertices[j], vertices[j] + n, len - 1, G);
        boost::add_edge(vertices[j] + n, vertices[j], 0, G);
      }
    }
  }

  // Keeps track of the distance to each vertex
  std::vector<int> dist(2 * n);

  boost::dijkstra_shortest_paths(
      G, n - 1,
      boost::distance_map(boost::make_iterator_property_map(
          dist.begin(), boost::get(boost::vertex_index, G))));

  int curr = 1'000'001;
  for (int i = 0; i < k; i++) {
    curr = std::min(curr, dist[i]);
  }

  if (curr <= 1'000'000) {
    std::cout << curr << "\n";
  } else {
    std::cout << "no\n";
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