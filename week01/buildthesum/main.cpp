#include <iostream>

void testcase() {
  int n;
  std::cin >> n;

  int sum = 0;
  for (int i = 0; i < n; i++) {
    int x;
    std::cin >> x;
    sum += x;
  }
  std::cout << sum << "\n";
}

int main() {
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
  return 0;
}