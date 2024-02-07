#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

void testcase() {
  int n, m;
  std::cin >> n >> m;

  std::vector<int> strengths(n);
  for (int i = 0; i < n; i++) {
    std::cin >> strengths[i];
  }

  std::multiset<int> weightSet;
  for (int i = 0; i < m; i++) {
    int w;
    std::cin >> w;
    weightSet.insert(w);
  }

  std::sort(strengths.begin(), strengths.end(), std::greater<int>());

  if (strengths[0] < *weightSet.rbegin()) {
    std::cout << "impossible\n";
    return;
  }

  int totalTime = -1;

  while (!weightSet.empty()) {
    totalTime += 1;

    for (int i = 0; i < n; i++) {
      // Upper bound returns a pointer to the first element that is strictly
      // greater than its argument.
      auto it = weightSet.upper_bound(strengths[i]);

      if (it == weightSet.begin()) {
        // If the iterator points to the beginning that means all elements are
        // greater than its argument. In our case this means we cannot carry any
        // boxes. And no person with less strength can carry any boxes.
        break;
      }
      it--;
      weightSet.erase(it);
    }
    totalTime += 2;
  }

  std::cout << totalTime << "\n";
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