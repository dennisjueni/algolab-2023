#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

void testcase() {
  int n, m, k;
  std::cin >> n >> m >> k;

  std::vector<int> prefixSums(n + 1);
  prefixSums[0] = 0;
  for (int i = 1; i <= n; i++) {
    int temp;
    std::cin >> temp;
    prefixSums[i] = prefixSums[i - 1] + temp;
  }

  std::vector<int> sw(n, -1);

  // Sliding Window (i exclusive, j inclusive)
  int i = n - 1, j = n;
  while (i >= 0) {
    int diff = prefixSums[j] - prefixSums[i];
    if (diff == k) {
      sw[i] = j - i;
      i--;
    } else if (diff < k) {
      sw[i] = -1;
      i--;
    } else if (diff > k) {
      sw[i] = -1;
      j--;
    }
  }

  // Even though we do not need the n-th index, it is nice so we do not need to
  // do OOB-checking
  std::vector<std::vector<int>> dp(m, std::vector<int>(n + 1, -1));

  // Initialization
  dp[0][n - 1] = sw[n - 1];
  for (int j = n - 2; j >= 0; j--) {
    dp[0][j] = std::max(sw[j], dp[0][j + 1]);
  }

  for (int i = 1; i < m; i++) {
    for (int j = n - 1; j >= 0; j--) {

      // This represents the max we get when starting with m=i at index j
      int currMax = sw[j];
      if (currMax == -1 || dp[i - 1][j + currMax] == -1) {
        dp[i][j] = dp[i][j + 1];
      } else {
        dp[i][j] = std::max(dp[i][j + 1], currMax + dp[i - 1][j + currMax]);
      }
    }
  }

  if (dp[m - 1][0] == -1) {
    std::cout << "fail\n";
  } else {
    std::cout << dp[m - 1][0] << "\n";
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