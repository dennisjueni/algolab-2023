/// 1
#include <iostream>
#include <limits>
#include <vector>

void testcase() {
  int n;
  std::cin >> n;

  std::vector<int> stackA(n + 1, 0);
  std::vector<int> stackB(n + 1, 0);

  for (int i = 1; i <= n; i++) {
    std::cin >> stackA[i];
  }

  for (int i = 1; i <= n; i++) {
    std::cin >> stackB[i];
  }

  std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, -1));

  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= n; j++) {

      if (i == 0 && j == 0) {
        dp[i][j] = 0;
        continue;
      }

      if (i == 0 || j == 0) {
        dp[i][j] = -1;
        continue;
      }

      dp[i][j] = std::numeric_limits<int>::max();

      // removing exactly one element from stack B
      int newCostA = 0;
      int numElementsA = 0;

      for (int k = i; k > 0; k--) {

        newCostA += stackA[k];
        numElementsA++;

        if (dp[k - 1][j - 1] == -1) {
          continue;
        }

        int totalCost = (newCostA - numElementsA) * (stackB[j] - 1);
        dp[i][j] = std::min(dp[i][j], totalCost + dp[k - 1][j - 1]);
      }

      // removing exactly one element from stack A
      int newCostB = 0;
      int numElementsB = 0;

      for (int k = j; k > 0; k--) {

        newCostB += stackB[k];
        numElementsB++;

        if (dp[i - 1][k - 1] == -1) {
          continue;
        }

        int totalCost = (newCostB - numElementsB) * (stackA[i] - 1);
        dp[i][j] = std::min(dp[i][j], totalCost + dp[i - 1][k - 1]);
      }

      if (dp[i][j] == std::numeric_limits<int>::max()) {
        dp[i][j] = -1;
      }
    }
  }

  std::cout << dp[n][n] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}