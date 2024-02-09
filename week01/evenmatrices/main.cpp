#include <iostream>
#include <vector>

void testcase() {
  int n;
  std::cin >> n;

  std::vector<std::vector<int>> prefixSums(n + 1, std::vector<int>(n + 1, 0));

  // Start by calculating the prefixSums of the matrix, with the first row and
  // col being all zeroes for easier indexing
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      int newNum;
      std::cin >> newNum;
      prefixSums[i][j] = prefixSums[i - 1][j] + prefixSums[i][j - 1] -
                         prefixSums[i - 1][j - 1] + newNum;
    }
  }

  int result = 0;

  // Go through all pairs of rows and calculate how many numbers are of
  // same/diff parity. Then we can simply look at the number of possibilities of
  // choosing a pair of either same or diff parity, which willl lead us to our
  // result
  for (int i = 0; i <= n; i++) {
    for (int j = i + 1; j <= n; j++) {

      int numSameParity = 0;
      for (int k = 0; k <= n; k++) {
        if (prefixSums[i][k] % 2 == prefixSums[j][k] % 2) {
          numSameParity++;
        }
      }
      int numDiffParity = n + 1 - numSameParity;
      result += (numSameParity * (numSameParity - 1) / 2) +
                (numDiffParity * (numDiffParity - 1) / 2);
    }
  }

  std::cout << result << "\n";
}

int main() {
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}