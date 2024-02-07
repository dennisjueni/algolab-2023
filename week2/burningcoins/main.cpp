#include <iostream>
#include <map>
#include <vector>

void testcase() {
  int n;
  std::cin >> n;

  std::vector<int> values(n, 0);

  for (int i = 0; i < n; i++) {
    std::cin >> values[i];
  }

  std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

  for (int i = 0; i < n - 1; i++) {
    dp[i][i] = values[i];
    dp[i][i + 1] = std::max(values[i], values[i + 1]);
  }
  dp[n - 1][n - 1] = values[n - 1];

  for (int i = n - 2; i >= 0; i--) {
    for (int j = i + 2; j < n; j++) {
      int val1 = values[i] + std::min(dp[i + 2][j], dp[i + 1][j - 1]);
      int val2 = values[j] + std::min(dp[i][j - 2], dp[i + 1][j - 1]);
      dp[i][j] = std::max(val1, val2);
    }
  }

  std::cout << dp[0][n - 1] << "\n";
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