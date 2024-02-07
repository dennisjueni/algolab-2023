#include <iostream>
#include <limits>
#include <vector>

std::vector<std::vector<int>> dp;
std::vector<std::vector<int>> adjList;
std::vector<int> costs;

// coveringOption = 0 => root not covered at all
// coveringOption = 1 => cover the root indirectly
// coveringOption = 2 => cover the root directly
int solve(int root, int coveringOption) {

  // Handle already calculated cases
  if (dp[root][coveringOption] != -1) {
    return dp[root][coveringOption];
  }

  // Handle base cases (leafs)
  if (adjList[root].size() == 0) {
    if (coveringOption == 0) {
      dp[root][coveringOption] = 0;
    } else {
      dp[root][coveringOption] = costs[root];
    }
    return dp[root][coveringOption];
  }

  if (coveringOption == 0) {
    // root not covered at all

    int totalCost = 0;
    for (int child : adjList[root]) {
      // Even though this means the root might be covered, if it is cheaper
      // we'll take it!
      totalCost += std::min(solve(child, 1), solve(child, 2));
    }
    dp[root][coveringOption] = totalCost;

  } else if (coveringOption == 2) {
    int totalCost = costs[root];

    for (int child : adjList[root]) {
      // Even though this means some vertices might be double covered, if it is
      // cheaper we'll take it
      int tempMin = std::min(solve(child, 0), solve(child, 1));
      totalCost += std::min(tempMin, solve(child, 2));
    }
    dp[root][coveringOption] = totalCost;

  } else {

    int totalCost = 0;
    int minDiff = std::numeric_limits<int>::max();
    int maxIdx = -1;

    for (int child : adjList[root]) {
      if (solve(child, 2) - solve(child, 1) < minDiff) {

        if (maxIdx != -1) {
          // replace what we previously added
          totalCost -= solve(maxIdx, 2);
          totalCost += std::min(solve(maxIdx, 1), solve(maxIdx, 2));
        }

        maxIdx = child;
        minDiff = solve(child, 2) - solve(child, 1);
        totalCost += solve(child, 2);
      } else {
        totalCost += std::min(solve(child, 1), solve(child, 2));
      }
    }

    dp[root][coveringOption] = totalCost;
  }

  return dp[root][coveringOption];
}

void testcase() {
  int n;
  std::cin >> n;

  dp = std::vector<std::vector<int>>(n, std::vector<int>(3, -1));
  adjList = std::vector<std::vector<int>>(n, std::vector<int>());
  costs = std::vector<int>(n, 0);

  for (int i = 0; i < n - 1; i++) {
    int u, v;
    std::cin >> u >> v;
    adjList[u].push_back(v);
  }

  for (int i = 0; i < n; i++) {
    std::cin >> costs[i];
  }

  int result = std::min(solve(0, 1), solve(0, 2));

  std::cout << result << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}