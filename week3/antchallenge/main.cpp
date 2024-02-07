#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
#include <queue>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;

typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void testcase() {
  int n, e, s, a, b;
  std::cin >> n >> e >> s >> a >> b;

  // The graph that we will use for storing the min edges of all species
  weighted_graph G(n);
  // Initially create one graph per species
  std::vector<weighted_graph> graphs(s, weighted_graph(n));

  for (int i = 0; i < e; i++) {
    int t1, t2;
    std::cin >> t1 >> t2;

    for (int j = 0; j < s; j++) {
      int w;
      std::cin >> w;
      boost::add_edge(t1, t2, w, graphs[j]);
    }
  }

  std::vector<int> hives(s);
  for (int i = 0; i < s; i++) {
    std::cin >> hives[i];
  }

  // Find private network per species and put edges in G if they are smaller
  for (int i = 0; i < s; i++) {
    int startingHive = hives[i];
    std::vector<bool> visited(n, false);
    std::priority_queue<std::tuple<int, int, int>,
                        std::vector<std::tuple<int, int, int>>,
                        std::greater<std::tuple<int, int, int>>>
        minHeap;

    visited[startingHive] = true;

    // Add all neighbors of startingHive to the prioqueue
    boost::graph_traits<weighted_graph>::out_edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(startingHive, graphs[i]);
         ei != ei_end; ++ei) {
      int edgeWeight = get(boost::edge_weight_t(), graphs[i], *ei);
      minHeap.push(std::make_tuple(edgeWeight, boost::source(*ei, graphs[i]),
                                   boost::target(*ei, graphs[i])));
    }

    while (!minHeap.empty()) {
      std::tuple<int, int, int> minEl = minHeap.top();
      minHeap.pop();
      int weight = std::get<0>(minEl);
      int source = std::get<1>(minEl);
      int target = std::get<2>(minEl);
      if (visited[target]) {
        continue;
      }

      visited[target] = true;

      boost::graph_traits<weighted_graph>::edge_descriptor e;
      bool exists;
      tie(e, exists) = edge(source, target, G);

      // if the weight was previously higher we update it
      if (exists && weight < get(boost::edge_weight_t(), G, e)) {
        boost::put(boost::get(boost::edge_weight, G), e, weight);
      } else if (!exists) {
        boost::add_edge(source, target, weight, G);
      }

      boost::graph_traits<weighted_graph>::out_edge_iterator ei, ei_end;
      for (boost::tie(ei, ei_end) = out_edges(target, graphs[i]); ei != ei_end;
           ++ei) {
        int edgeWeight = get(boost::edge_weight_t(), graphs[i], *ei);
        if (!visited[boost::target(*ei, graphs[i])]) {
          minHeap.push(std::make_tuple(edgeWeight,
                                       boost::source(*ei, graphs[i]),
                                       boost::target(*ei, graphs[i])));
        }
      }
    }
  }

  // Assume for now that the graph we have now is a perfect representation of
  // our problem
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(
      G, a,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));

  std::cout << dist_map[b] << "\n";
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
