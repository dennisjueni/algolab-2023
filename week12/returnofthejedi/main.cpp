#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <iostream>
#include <limits>
#include <stack>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::property<boost::vertex_distance_t, int>,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void testcase() {

  int n, i;
  std::cin >> n >> i;
  i--;

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  std::vector<std::vector<int>> edgeCosts =
      std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
  std::vector<std::vector<int>> mstAdjList =
      std::vector<std::vector<int>>(n, std::vector<int>());
  std::vector<std::vector<bool>> isInMST =
      std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      int c;
      std::cin >> c;

      edge_desc e;
      e = boost::add_edge(i, j, G).first;
      weights[e] = c;

      edgeCosts[i][j] = c;
      edgeCosts[j][i] = c;
    }
  }

  // This is Leia's constructed network
  std::vector<boost::graph_traits<weighted_graph>::vertex_descriptor> p(n);
  prim_minimum_spanning_tree(G, &p[0], boost::root_vertex(i));

  int leiasCost = 0;
  for (std::size_t i = 0; i != p.size(); i++) {
    if (p[i] == i) {
      // This only happens for the root
      continue;
    }

    leiasCost += edgeCosts[i][p[i]];

    isInMST[i][p[i]] = true;
    isInMST[p[i]][i] = true;

    mstAdjList[i].push_back(p[i]);
    mstAdjList[p[i]].push_back(i);
  }

  // Compute max edges for pairwise paths

  std::vector<std::vector<int>> maxEdgePerPair =
      std::vector<std::vector<int>>(n, std::vector<int>(n, 0));

  for (int i = 0; i < n; i++) {

    std::vector<bool> visited = std::vector<bool>(n, false);
    std::stack<std::pair<int, int>> dfsStack =
        std::stack<std::pair<int, int>>();
    // Max edge between i and i is "0"
    dfsStack.push({i, 0});

    while (!dfsStack.empty()) {
      auto currPair = dfsStack.top();
      dfsStack.pop();

      int curr = currPair.first;
      int maxEdge = currPair.second;

      if (!visited[curr]) {
        maxEdgePerPair[i][curr] = maxEdge;
        visited[curr] = true;
        for (int child : mstAdjList[curr]) {
          dfsStack.push({child, std::max(maxEdge, edgeCosts[curr][child])});
        }
      }
    }
  }

  int minDifference = std::numeric_limits<int>::max();

  // Check difference of adding the edge between two vertices i and j and
  // removing the max in the path in the MST
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (!isInMST[i][j]) {
        int diff = edgeCosts[i][j] - maxEdgePerPair[i][j];
        minDifference = std::min(minDifference, diff);
      }
    }
  }

  std::cout << leiasCost + minDifference << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}