#include <iostream>
#include <map>
#include <vector>

void testcase() {
  int n, m, maxMoves;
  long claimedScore;
  std::cin >> n >> m >> claimedScore >> maxMoves;

  std::map<int, std::vector<std::pair<int, int>>> outEdges;
  for (int i = 0; i < m; i++) {
    int u, v, p;
    std::cin >> u >> v >> p;
    outEdges[u].push_back(std::make_pair(v, p));
  }

  std::vector<std::vector<long>> dp(n, std::vector<long>(2, 0));

  for (int i = 1; i <= maxMoves; i++) {
    for (int v = 0; v < n; v++) {

      long max = -1;
      for (auto pair : outEdges[v]) {
        // If we have a new maximum it's the score of the canal + the score at
        // the resp. hole
        max = std::max(max, pair.second + dp[pair.first][(i + 1) % 2]);
      }

      if (v == 0 && max >= claimedScore) {
        std::cout << i << "\n";
        return;
      }

      if (max == -1) {
        // This means we are at a sink
        dp[v][i % 2] = dp[0][i % 2];
      } else {
        dp[v][i % 2] = max;
      }
    }
  }

  std::cout << "Impossible\n";
  return;
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