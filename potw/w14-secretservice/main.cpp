#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type for Djikstra computation
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    djikstra_graph;
typedef boost::property_map<djikstra_graph, boost::edge_weight_t>::type
    djikstra_weight_map;
typedef boost::graph_traits<djikstra_graph>::edge_descriptor djikstra_edge_desc;
typedef boost::graph_traits<djikstra_graph>::vertex_descriptor
    djisktra_vertex_desc;

// Graph Type with nested interior edge properties for flow algorithms
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

// Custom edge adder class, highly recommended
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

  int n, m, a, s, c, d;
  std::cin >> n >> m >> a >> s >> c >> d;

  djikstra_graph djikstraGraph(n);
  djikstra_weight_map weights = boost::get(boost::edge_weight, djikstraGraph);

  for (int i = 0; i < m; i++) {
    char w;
    int x, y, z;
    std::cin >> w >> x >> y >> z;

    djikstra_edge_desc e;
    e = boost::add_edge(x, y, djikstraGraph).first;
    weights[e] = z;

    if (w == 'L') {
      e = boost::add_edge(y, x, djikstraGraph).first;
      weights[e] = z;
    }
  }

  std::vector<int> agents = std::vector<int>(a);
  for (int i = 0; i < a; i++) {
    std::cin >> agents[i];
  }

  std::vector<int> shelters = std::vector<int>(s);
  for (int i = 0; i < s; i++) {
    std::cin >> shelters[i];
  }

  std::vector<std::vector<int>> agentToShelterDist =
      std::vector<std::vector<int>>(a, std::vector<int>(s, -1));
  int maxDist = 0;

  for (int i = 0; i < a; i++) {
    int startingVertex = agents[i];

    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(
        djikstraGraph, startingVertex,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, djikstraGraph))));

    for (int j = 0; j < s; j++) {
      int shelterVertex = shelters[j];
      if (dist_map[shelterVertex] != std::numeric_limits<int>::max()) {
        agentToShelterDist[i][j] = dist_map[shelterVertex] + d;

        maxDist = std::max(maxDist, agentToShelterDist[i][j]);
      }
    }
  }

  int bestTime = 0;

  // Binary search over potential max Distances
  int left = 0;
  int right = maxDist;

  while (left <= right) {
    int middle = (left + right) / 2;

    graph G(a + (c * s) + 2);
    edge_adder adder(G);
    int source = a + c * s;
    int sink = a + (c * s) + 1;

    for (int i = 0; i < a; i++) {
      for (int j = 0; j < s; j++) {
        if (agentToShelterDist[i][j] <= middle &&
            agentToShelterDist[i][j] != -1) {
          adder.add_edge(i, a + j, 1);

          if (c == 2 && agentToShelterDist[i][j] <= middle - d) {
            // This means the agent can go to the "first" signing in slot...
            adder.add_edge(i, a + s + j, 1);
          }
        }
      }
    }

    for (int i = 0; i < a; i++) {
      adder.add_edge(source, i, 1);
    }
    for (int i = 0; i < s; i++) {
      adder.add_edge(a + i, sink, 1);
      if (c == 2) {
        adder.add_edge(a + s + i, sink, 1);
      }
    }

    long flow = boost::push_relabel_max_flow(G, source, sink);

    if (flow == a) {
      // It is potentially possible with less time, so we decrease right
      bestTime = middle;
      right = middle - 1;
    } else {
      // It is not possible with this much time, so we increase left
      left = middle + 1;
    }
  }

  std::cout << bestTime << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}