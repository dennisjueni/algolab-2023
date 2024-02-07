#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <iostream>
#include <limits>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;

void testcase() {
  int n, c, f;
  std::cin >> n >> c >> f;

  std::vector<std::set<std::string>> studentInterests(n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      std::string interest;
      std::cin >> interest;
      studentInterests[i].insert(interest);
    }
  }

  graph G(n);

  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      int count = 0;
      auto it1 = studentInterests[i].begin(), it2 = studentInterests[j].begin();
      while (it1 != studentInterests[i].end() &&
             it2 != studentInterests[j].end()) {
        if (*it1 < *it2) {
          ++it1;
        } else if (*it2 < *it1) {
          ++it2;
        } else {
          ++count;
          ++it1;
          ++it2;
        }
      }
      if (count > f) {
        boost::add_edge(i, j, G);
      }
    }
  }
  std::vector<boost::graph_traits<graph>::vertex_descriptor> mate(n);
  checked_edmonds_maximum_cardinality_matching(
      G, boost::make_iterator_property_map(mate.begin(),
                                           boost::get(boost::vertex_index, G)));

  int matchingSize =
      matching_size(G, boost::make_iterator_property_map(
                           mate.begin(), boost::get(boost::vertex_index, G)));

  if (matchingSize == n / 2) {
    std::cout << "not optimal\n";
  } else {
    std::cout << "optimal\n";
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