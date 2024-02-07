/// 1
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>
#include <vector>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    Graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class EdgeAdder {
  Graph &G;

public:
  explicit EdgeAdder(Graph &G) : G(G) {}

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

void solve() {

  int numIntersections, numStreets, numStations, numPhotos;
  std::cin >> numIntersections >> numStreets >> numStations >> numPhotos;

  Graph G(2 * numIntersections + 2);
  EdgeAdder adder(G);

  int source = 2 * numIntersections;
  int sink = 2 * numIntersections + 1;

  std::vector<int> stationsAtIntersection(numIntersections, 0);
  std::vector<int> photosAtIntersection(numIntersections, 0);

  for (int i = 0; i < numStations; i++) {
    int location;
    std::cin >> location;

    stationsAtIntersection[location] += 1;
  }

  for (int i = 0; i < numPhotos; i++) {
    int location;
    std::cin >> location;

    photosAtIntersection[location] += 1;
  }

  for (int i = 0; i < numIntersections; i++) {
    // From source to the station we add as many policemen as there are stations
    // at this intersection
    adder.add_edge(source, i, stationsAtIntersection[i]);

    // From every intersection with n photos we can get to the same intersection
    // one layer above with n connections
    adder.add_edge(i, numIntersections + i, photosAtIntersection[i]);

    // From every intersection with a station we can get to the sink
    adder.add_edge(numIntersections + i, sink, stationsAtIntersection[i]);
  }

  for (int i = 0; i < numStreets; i++) {
    int x, y;
    std::cin >> x >> y;

    adder.add_edge(x, y, std::numeric_limits<long>::max());
    adder.add_edge(numIntersections + x, numIntersections + y, 1);
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << flow << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    solve();
  }
}