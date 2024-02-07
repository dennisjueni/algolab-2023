#include <algorithm>
#include <iostream>

void testcase() {
  int n;
  std::cin >> n;

  // We initialize even to 1 because: To calculate x_i + ... + x_j, we calculate
  // S_j - S_{i-1}. Thus for i = 0, we need S_{-1} to be defined. This is by
  // definition even, thus we already have an even pair. E.g. if x_0 = 0 and we
  // want it to be counted, we need S_1 - S_0 to be of same parity and since S_1
  // is even we need S_0 to be even as well. Thus we start with 1 even cumsum.
  // The same argument can be made if we look at x_1 + x_2 + x_3 + ...
  int sum = 0, even = 1;

  for (int i = 0; i < n; i++) {
    int x;
    std::cin >> x;
    sum += x;
    if (sum % 2 == 0) {
      even++;
    }
  }

  int odd = n + 1 - even;

  int pairs = (even * (even - 1) / 2) + (odd * (odd - 1) / 2);
  std::cout << pairs << "\n";
}

int main() {
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}