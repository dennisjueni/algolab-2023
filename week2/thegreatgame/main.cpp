#include <iostream>
#include <map>
#include <vector>

void testcase() {
  int n, m;
  std::cin >> n >> m;

  int r, b;
  std::cin >> r >> b;
  r--;
  b--;

  std::map<int, std::vector<int>> outEdges;
  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    u--;
    v--;
    outEdges[u].push_back(v);
  }

  std::vector<std::vector<int>> dp(n, std::vector<int>(2, 0));
  dp[n - 1][0] = 0;
  dp[n - 1][1] = 0;

  for (int i = n - 2; i >= 0; i--) {
    // We can initialize min to n+1, since the solution will always be at most n
    int min = n + 1, max = -1;
    for (int out : outEdges[i]) {
      min = std::min(min, dp[out][1]);
      max = std::max(max, dp[out][0]);
    }
    dp[i][0] = 1 + min;
    dp[i][1] = 1 + max;
  }

  for (auto vec : dp) {
    for (int x : vec) {
      std::cout << x << ", ";
    }
    std::cout << "\n";
  }

  if (dp[r][0] < dp[b][0]) {
    // Holmes wins
    std::cout << 0 << "\n";
  } else if (dp[r][0] > dp[b][0]) {
    // Moriarty wins
    std::cout << 1 << "\n";
  } else {
    if (dp[r][0] % 2 == 1) {
      // Holmes wins
      std::cout << 0 << "\n";
    } else {
      // Moriarty wins
      std::cout << 1 << "\n";
    }
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