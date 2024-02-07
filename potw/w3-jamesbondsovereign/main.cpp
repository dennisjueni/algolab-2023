#include <cmath>
#include <iostream>
#include <map>
#include <vector>

void testcase() {
  int n, m, k;
  std::cin >> n >> m >> k;

  std::vector<int> values(n);

  for (int i = 0; i < n; i++) {
    std::cin >> values[i];
  }

  std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

  for (int i = n; i >= 0; i--) {
    for (int j = i; j < n; j++) {

      if ((n - 1 - (j - i)) % m == k) {

        if (i < n - 1 && j > 0) {
          dp[i][j] =
              std::max(values[i] + dp[i + 1][j], values[j] + dp[i][j - 1]);
        } else if (i < n - 1) {
          dp[i][j] = values[i] + dp[i + 1][j];
        } else {
          dp[i][j] = values[j] + dp[i][j - 1];
        }

      } else {

        if (i < n - 1 && j > 0) {
          dp[i][j] = std::min(dp[i + 1][j], dp[i][j - 1]);
        } else if (i < n - 1) {
          dp[i][j] = dp[i + 1][j];
        } else {
          dp[i][j] = dp[i][j - 1];
        }
      }
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