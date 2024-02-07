#include <iostream>
#include <map>
#include <vector>

struct MapElement {
  int waterway1;
  int max1;

  int waterway2;
  int max2;
};

void testcase() {
  int numIslands, numMen, numWays;
  std::cin >> numIslands >> numMen >> numWays;

  std::map<int, MapElement> maxIslandsPerMen;
  std::vector<std::vector<int>> waterWays =
      std::vector<std::vector<int>>(numWays, std::vector<int>());
  std::vector<int> menPerIsland = std::vector<int>(numIslands);

  for (int i = 0; i < numIslands; i++) {
    std::cin >> menPerIsland[i];
  }

  for (int i = 0; i < numWays; i++) {
    int l;
    std::cin >> l;

    int numMenUsed = 0;

    int pyke;
    std::cin >> pyke;
    waterWays[i].push_back(pyke);

    for (int j = 1; j < l; j++) {
      int currIsland;
      std::cin >> currIsland;

      waterWays[i].push_back(currIsland);

      numMenUsed += menPerIsland[currIsland];

      if (maxIslandsPerMen.find(numMenUsed) == maxIslandsPerMen.end()) {
        maxIslandsPerMen[numMenUsed] = {i, j, -1, -1};

      } else if (maxIslandsPerMen[numMenUsed].max1 < j) {

        maxIslandsPerMen[numMenUsed].max2 = maxIslandsPerMen[numMenUsed].max1;
        maxIslandsPerMen[numMenUsed].waterway2 =
            maxIslandsPerMen[numMenUsed].waterway1;

        maxIslandsPerMen[numMenUsed].max1 = j;
        maxIslandsPerMen[numMenUsed].waterway1 = i;

      } else if (maxIslandsPerMen[numMenUsed].max2 < j) {
        maxIslandsPerMen[numMenUsed].max2 = j;
        maxIslandsPerMen[numMenUsed].waterway2 = i;
      }
    }
  }

  int maxIslands = 0;

  // Starting 2-pointer approach
  for (int i = 0; i < numWays; i++) {

    int left = waterWays[i].size() - 1;
    int right = waterWays[i].size() - 1;

    int currMenUsed = menPerIsland[waterWays[i][left]];

    while (left != 0) {
      int menLeft = numMen - currMenUsed;

      if (right == 0 &&
          maxIslandsPerMen.find(menLeft) == maxIslandsPerMen.end()) {
        // This means it is not possible starting at x going over pyke
        currMenUsed -= menPerIsland[waterWays[i][left]];
        left--;
        continue;
      }

      if (right == 0 && maxIslandsPerMen[menLeft].waterway1 != i) {
        // We take the maximum possible men after pyke
        maxIslands = std::max(maxIslands, left - right + 1 +
                                              maxIslandsPerMen[menLeft].max1);

        currMenUsed -= menPerIsland[waterWays[i][left]];
        left--;
        continue;

      } else if (right == 0 && maxIslandsPerMen[menLeft].waterway2 != -1) {
        // We take the maximum possible men after pyke (which is not ourselves)
        maxIslands = std::max(maxIslands, left - right + 1 +
                                              maxIslandsPerMen[menLeft].max2);

        currMenUsed -= menPerIsland[waterWays[i][left]];
        left--;
        continue;
      }

      if (menLeft == 0) {
        // Have found a solution, check for other solutions
        maxIslands = std::max(maxIslands, left - right + 1);

        if (right == 0) {
          currMenUsed -= menPerIsland[waterWays[i][left]];
          left--;
        } else {
          right--;
          currMenUsed += menPerIsland[waterWays[i][right]];
        }
        continue;
      }

      if (menLeft < 0 || right == 0) {
        currMenUsed -= menPerIsland[waterWays[i][left]];
        left--;
      } else {
        right--;
        currMenUsed += menPerIsland[waterWays[i][right]];
      }
    }
  }

  // Check starting at pyke
  int menUsed = menPerIsland[0];
  int menLeft = numMen - menUsed;
  if (menLeft == 0 && maxIslands == 0) {
    maxIslands = 1;
  } else if (maxIslandsPerMen.find(menLeft) != maxIslandsPerMen.end()) {
    maxIslands = std::max(maxIslands, 1 + maxIslandsPerMen[menLeft].max1);
  }

  std::cout << maxIslands << "\n";
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