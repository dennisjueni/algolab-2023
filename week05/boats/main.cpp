#include <algorithm>
#include <iostream>
#include <vector>

struct Boat {
  int len;
  int pos;
};

void testcase() {
  int n;
  std::cin >> n;

  std::vector<Boat> boats(n);
  for (int i = 0; i < n; i++) {
    int len, pos;
    std::cin >> len >> pos;
    boats[i] = {len, pos};
  }

  std::sort(boats.begin(), boats.end(),
            [](Boat b1, Boat b2) { return b1.pos <= b2.pos; });

  std::vector<Boat> result = {{boats[0].len, boats[0].pos}};

  for (int i = 1; i < n; i++) {
    int length = boats[i].len;
    int position = boats[i].pos;
    int largestOccupiedPlace = result.back().pos;

    if (position - length >= largestOccupiedPlace) {
      // No need to remove any boats, save the position where the boat ends
      result.push_back({length, position});
    } else if (largestOccupiedPlace <= position) {
      // Also no need ot remove any boats but we cannot place it minimally
      result.push_back({length, largestOccupiedPlace + length});
    } else {
      bool shouldPlaceNewOne = false;
      int newPos = -1;

      while (!result.empty()) {

        int newLargestOccupiedPlace = largestOccupiedPlace;
        // We might need to remove a boat
        int secondLargestOccupiedPlace = 0;
        if (result.size() > 1) {
          secondLargestOccupiedPlace = result[result.size() - 2].pos;
        }

        if (position - length >= secondLargestOccupiedPlace) {
          // This means we can place the new boat
          newLargestOccupiedPlace = position;
        } else if (secondLargestOccupiedPlace <= position) {
          // This also means we can place the new boat
          newLargestOccupiedPlace = secondLargestOccupiedPlace + length;
        }

        if (newLargestOccupiedPlace < largestOccupiedPlace) {
          result.pop_back();
          shouldPlaceNewOne = true;
          newPos = newLargestOccupiedPlace;

          // Update largestOccupiedPlace
          largestOccupiedPlace = secondLargestOccupiedPlace;
        } else {
          break;
        }
      }

      if (shouldPlaceNewOne) {
        result.push_back({length, newPos});
      }
    }
  }

  std::cout << result.size() << "\n";
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