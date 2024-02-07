#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

int tripLength, maxRisk;

std::vector<int> temperatures;
std::vector<std::vector<int>> adjList;

std::vector<std::pair<int, int>> dfsStack;
std::map<int, int> tempCount;
std::set<int> validStarts;

int getTemperatureDifference() {
  int min = tempCount.begin()->first;
  int max = tempCount.rbegin()->first;
  return max - min;
}

void decreaseTemperatureCount(int temp) {
  tempCount[temp]--;

  // If the temperature does not occur anymore we can remove it from the map
  if (tempCount[temp] == 0) {
    tempCount.erase(temp);
  }
}

void dfs(int root, int depth) {

  int currentTemperature = temperatures[root];

  // First we add the current node to the dfsStack
  dfsStack.push_back({root, currentTemperature});

  // Update the temperatures
  tempCount[currentTemperature]++;

  int oldTemperature = -1;

  if (depth >= tripLength) {
    oldTemperature = dfsStack[depth - tripLength].second;
    decreaseTemperatureCount(oldTemperature);
  }

  // Check if path ending at root is valid path
  if (depth >= tripLength - 1 && getTemperatureDifference() <= maxRisk) {
    validStarts.insert(dfsStack[depth - tripLength + 1].first);
  }

  for (int child : adjList[root]) {
    dfs(child, depth + 1);
  }

  // Now we remove it again from the stack since we continue with a parent
  dfsStack.pop_back();
  decreaseTemperatureCount(currentTemperature);

  if (oldTemperature != -1) {
    tempCount[oldTemperature]++;
  }
}

void testcase() {
  int n, m, k;
  std::cin >> n >> m >> k;

  tripLength = m;
  maxRisk = k;

  dfsStack = std::vector<std::pair<int, int>>();
  tempCount = std::map<int, int>();
  validStarts = std::set<int>();

  temperatures = std::vector<int>(n);
  for (int i = 0; i < n; i++) {
    std::cin >> temperatures[i];
  }

  adjList = std::vector<std::vector<int>>(n, std::vector<int>());
  for (int i = 0; i < n - 1; i++) {
    int u, v;
    std::cin >> u >> v;

    adjList[u].push_back(v);
  }

  dfs(0, 0);

  if (validStarts.empty()) {
    std::cout << "Abort mission\n";
    return;
  }

  for (int validStart : validStarts) {
    std::cout << validStart << " ";
  }
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}