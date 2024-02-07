#include <iostream>
#include <limits>
#include <vector>

void testcase() {
  int n, k;
  std::cin >> n >> k;

  std::vector<int> prefixSums = std::vector<int>(n + 2, 0);

  for (int i = 1; i <= n; i++) {
    int curr;
    std::cin >> curr;

    prefixSums[i] = prefixSums[i - 1] + curr;
  }
  prefixSums[n + 1] = prefixSums[n];

  int left = 0;
  int right = 1;

  int bestLeft = 0;
  int bestRight = 0;
  int minDiff = std::numeric_limits<int>::max();

  while (left <= n && right <= n) {

    int currSum = prefixSums[right] - prefixSums[left];

    if (currSum == k) {
      // We found the lexicographically closest solution
      bestLeft = left;
      bestRight = right;
      break;
    }

    int diff = std::abs(currSum - k);

    // strictly smaller to keep lexicographic property
    if (diff < minDiff) {
      bestLeft = left;
      bestRight = right;
      minDiff = diff;
    }

    if (currSum > k) {
      left++;
    } else {
      right++;
    }
  }

  std::cout << bestLeft << " " << bestRight - 1 << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}