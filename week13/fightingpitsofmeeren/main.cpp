#include <cmath>
#include <iostream>
#include <set>
#include <vector>

typedef std::vector<int> VI;
typedef std::vector<VI> VII;
typedef std::vector<VII> VIII;
typedef std::vector<VIII> VIIII;

struct Entrance {
  int fighter1;
  int fighter2;
  int fighter3;
};

int numFighters, numDistinctFighters, m;
std::vector<int> queue;
VIIII dp;

int numDistinct(Entrance entry) {
  std::set<int> lastFighters;
  if (m == 2) {
    lastFighters = {0, entry.fighter1, entry.fighter2};
  } else {
    lastFighters = {0, entry.fighter1, entry.fighter2, entry.fighter3};
  }
  return lastFighters.size() - 1;
}

int excitement(Entrance entry, int diff) {
  return 1000 * numDistinct(entry) - std::pow(2, std::abs(diff));
}

int solve(int fighterIdx, Entrance north, Entrance south, int diff) {

  if (fighterIdx == -1) {
    return 0;
  }

  int northIdx = north.fighter1 * 5 + north.fighter2;
  int southIdx = south.fighter1 * 5 + south.fighter2;
  int diffIdx = diff + 12;

  if (std::abs(diff) > 11) {
    dp[fighterIdx][northIdx][southIdx][diffIdx] = -1;
    return -1;
  }

  if (dp[fighterIdx][northIdx][southIdx][diffIdx] != -2) {
    return dp[fighterIdx][northIdx][southIdx][diffIdx];
  }

  Entrance newNorth, newSouth;
  newNorth = {queue[fighterIdx], north.fighter1, north.fighter2};
  newSouth = {queue[fighterIdx], south.fighter1, south.fighter2};

  int excitementNorth = excitement(newNorth, diff + 1);
  int excitementSouth = excitement(newSouth, diff - 1);

  int sendNorth = solve(fighterIdx - 1, newNorth, south, diff + 1);
  int sendSouth = solve(fighterIdx - 1, north, newSouth, diff - 1);

  dp[fighterIdx][northIdx][southIdx][diffIdx] = -1;

  if (excitementNorth >= 0 && sendNorth != -1) {
    dp[fighterIdx][northIdx][southIdx][diffIdx] =
        std::max(dp[fighterIdx][northIdx][southIdx][diffIdx],
                 excitementNorth + sendNorth);
  }

  if (excitementSouth >= 0 && sendSouth != -1) {
    dp[fighterIdx][northIdx][southIdx][diffIdx] =
        std::max(dp[fighterIdx][northIdx][southIdx][diffIdx],
                 excitementSouth + sendSouth);
  }

  return dp[fighterIdx][northIdx][southIdx][diffIdx];
}

void testcase() {

  std::cin >> numFighters >> numDistinctFighters >> m;

  queue = std::vector<int>(numFighters);

  for (int i = numFighters - 1; i >= 0; i--) {
    int fighterType;
    std::cin >> fighterType;

    queue[i] = fighterType + 1;
  }

  dp = VIIII(numFighters, VIII(25, VII(25, VI(24, -2))));

  Entrance north = {0, 0, 0};
  Entrance south = {0, 0, 0};

  int maxExcitement = solve(numFighters - 1, north, south, 0);

  std::cout << maxExcitement << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}