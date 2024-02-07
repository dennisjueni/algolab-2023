#include <iostream>
#include <vector>

void testcase() {

  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<std::pair<int, int>>> forwardAdjList(
      n, std::vector<std::pair<int, int>>());
  std::vector<std::vector<std::pair<int, int>>> backwardAdjList(
      n, std::vector<std::pair<int, int>>());

  for (int i = 0; i < m; i++) {
    int u, v, f;
    std::cin >> u >> v >> f;

    if (u < v) {
      forwardAdjList[u].push_back({v, f});
    } else {
      backwardAdjList[v].push_back({u, f});
    }
  }
  std::vector<std::vector<int>> dp =
      std::vector<std::vector<int>>(n, std::vector<int>(n, -1));

  for (int j = n - 1; j >= 0; j--) {
    for (int i = n - 1; i >= 0; i--) {

      // Base Case
      if (i == n - 1 && j == n - 1) {
        dp[i][j] = 0;
        continue;
      }

      // Cannot visit same node twice
      if (i == j && i != 0) {
        dp[i][j] = -1;
        continue;
      }

      if (i < j) {
        int currMax = -1;
        for (auto street : forwardAdjList[i]) {
          int dest = street.first;
          int rats = street.second;

          if (dp[dest][j] != -1) {
            currMax = std::max(currMax, rats + dp[dest][j]);
          }
        }

        dp[i][j] = currMax;
        continue;
      }

      if (i >= j) {
        int currMax = -1;
        for (auto street : backwardAdjList[j]) {
          int dest = street.first;
          int rats = street.second;
          if (dp[i][dest] != -1) {
            currMax = std::max(currMax, rats + dp[i][dest]);
          }
        }
        dp[i][j] = currMax;
      }
    }
  }

  std::cout << dp[0][0] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}