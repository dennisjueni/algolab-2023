#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

void testcase() {
  int n;
  std::cin >> n;

  std::vector<int> beachBars(n);

  for (int i = 0; i < n; i++) {
    std::cin >> beachBars[i];
  }

  std::sort(beachBars.begin(), beachBars.end());

  int i = 0, j = 0;

  int maxParasols = 1;
  int longestDist = 0;

  std::set<int> optimalPoints = {};

  while (j < n) {
    int diff = beachBars[j] - beachBars[i];
    if (diff <= 200) {
      // Update the max values
      int numParasols = j - i + 1;
      int currLongestDist = diff % 2 == 0 ? diff / 2 : (diff + 1) / 2;

      int newPoint1 = beachBars[j] - currLongestDist;
      int newPoint2 = beachBars[i] + currLongestDist;

      if (numParasols == maxParasols && currLongestDist == longestDist) {
        optimalPoints.insert(newPoint1);
        optimalPoints.insert(newPoint2);
      } else if (numParasols > maxParasols || (numParasols == maxParasols &&
                                               currLongestDist < longestDist)) {
        maxParasols = numParasols;
        longestDist = currLongestDist;
        optimalPoints = {newPoint1, newPoint2};
      }

      // Update sliding window
      j++;
    } else {
      i++;
    }
  }

  std::cout << maxParasols << " " << longestDist << "\n";
  for (int point : optimalPoints) {
    std::cout << point << " ";
  }
  std::cout << "\n";
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