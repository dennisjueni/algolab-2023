#include <algorithm>
#include <iostream>

void testcase() {
  int n;
  std::cin >> n;

  int currentMax = 1;

  for (int i = 0; i < n; i++) {
    int height;
    std::cin >> height;

    if (currentMax > i) {
      currentMax = std::max(currentMax, i + height);
    }
  }
  std::cout << std::min(currentMax, n) << "\n";
  return;
}

int main() {
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}