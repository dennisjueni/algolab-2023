#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

struct Bomb {
  int detTime;
  int index;
};

void testcase() {
  int n;
  std::cin >> n;

  std::vector<Bomb> bombs(n);
  for (int i = 0; i < n; i++) {
    int detTime;
    std::cin >> detTime;
    bombs[i] = {detTime, i};
  }

  int firstGrounded = (n - 1) / 2;

  for (int i = 0; i < firstGrounded; i++) {
    bombs[2 * i + 1].detTime =
        std::min(bombs[i].detTime - 1, bombs[2 * i + 1].detTime);
    bombs[2 * i + 2].detTime =
        std::min(bombs[i].detTime - 1, bombs[2 * i + 2].detTime);
  }

  auto comparator = [](const Bomb &a, const Bomb &b) {
    return a.detTime > b.detTime;
  };
  std::priority_queue<Bomb, std::vector<Bomb>, decltype(comparator)> prioQueue(
      comparator);

  for (int i = firstGrounded; i < n; i++) {
    prioQueue.push(bombs[i]);
  }

  int currentTime = 1;
  std::set<int> popped;

  while (!prioQueue.empty()) {
    Bomb top = prioQueue.top();
    prioQueue.pop();

    if (top.detTime < currentTime) {
      std::cout << "no\n";
      return;
    }

    popped.insert(top.index);

    // Get the parent of the current node (if possible)
    int secondChild;
    int parentIndex;
    if (top.index % 2 == 0) {
      parentIndex = (top.index / 2) - 1;
      secondChild = 2 * parentIndex + 1;
    } else {
      parentIndex = ((top.index + 1) / 2) - 1;
      secondChild = 2 * parentIndex + 2;
    }

    if (popped.find(secondChild) != popped.end()) {
      prioQueue.push(bombs[parentIndex]);
    }

    currentTime++;
  }

  std::cout << "yes\n";
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