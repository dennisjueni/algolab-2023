/// 4
#include <algorithm>
#include <iostream>
#include <vector>

struct Vertex {
  int index;
  int galleons;
  int galleonSum;
  int numNodes;
  int timeSum;
};

std::vector<Vertex> vertices;
std::vector<std::vector<std::pair<int, int>>> outEdges;

void dfs(int vertex) {

  int galleonSum = vertices[vertex].galleons;
  int numNodes = 1;
  int timeSum = 0;

  for (auto child : outEdges[vertex]) {

    dfs(child.first);

    numNodes += vertices[child.first].numNodes;
    galleonSum += vertices[child.first].galleonSum;
    timeSum += 2 * child.second + vertices[child.first].timeSum;
  }

  vertices[vertex].numNodes = numNodes;
  vertices[vertex].galleonSum = galleonSum;
  vertices[vertex].timeSum = timeSum;
}

long getMaxGold(int vertexId, int startTime) {

  std::sort(outEdges[vertexId].begin(), outEdges[vertexId].end(),
            [](std::pair<int, int> a, std::pair<int, int> b) {
              long numNodesA = vertices[a.first].numNodes;
              long numNodesB = vertices[b.first].numNodes;

              long timeA = 2 * a.second + vertices[a.first].timeSum;
              long timeB = 2 * b.second + vertices[b.first].timeSum;
              ;

              return timeA * numNodesB < timeB * numNodesA;
            });

  long totalGold = vertices[vertexId].galleons - startTime;

  for (auto child : outEdges[vertexId]) {
    // We only start when we reach the child
    startTime += child.second;

    totalGold += getMaxGold(child.first, startTime);

    startTime += child.second + vertices[child.first].timeSum;
  }

  return totalGold;
}

void testcase() {
  int n;
  std::cin >> n;

  vertices.clear();
  vertices.resize(n + 1);
  vertices[0] = {0, 0, -1, 1, -1};

  for (int i = 0; i < n; i++) {
    int temp;
    std::cin >> temp;
    vertices[i + 1] = {i + 1, temp, -1, 1, -1};
  }

  outEdges.clear();
  outEdges.resize(n + 1);

  for (int i = 0; i < n; i++) {
    int u, v, l;
    std::cin >> u >> v >> l;
    outEdges[u].push_back(std::make_pair(v, l));
  }

  dfs(0);

  std::cout << getMaxGold(0, 0) << "\n";
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