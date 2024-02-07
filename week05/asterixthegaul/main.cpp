#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

struct Movement {
  long d;
  long t;
};

struct Result {
  long totalTime;
  long totalDist;
};

std::vector<Result> listSubsets(std::vector<Movement> &moves) {

  int n = moves.size();
  std::vector<Result> results(1 << n);

  for (int i = 0; i < 1 << n; i++) {
    long totalTime = 0;
    long totalDist = 0;
    for (int j = 0; j < n; j++) {
      if (i & 1 << j) {
        totalTime += moves[j].t;
        totalDist += moves[j].d;
      }
    }

    results[i] = {totalTime, totalDist};
  }

  return results;
}

bool checkSolution(int n, std::vector<Movement> moves, long T, long D) {
  // Split the original moves vectors into two (roughly) equally sized vectors
  int half = n / 2;
  std::vector<Movement> firstHalf(moves.begin(), moves.begin() + half);
  std::vector<Movement> secondHalf(moves.begin() + half, moves.end());

  // Generate all subsets for both vectors
  std::vector<Result> firstResult = listSubsets(firstHalf);
  std::vector<Result> secondResult = listSubsets(secondHalf);

  // First sort the second vector by time
  // Then we additionally make sure that if a dist D can be done in time T, at
  // least dist D can also be in any time T' > T
  std::sort(secondResult.begin(), secondResult.end(),
            [](Result r1, Result r2) { return r1.totalTime < r2.totalTime; });

  for (unsigned int i = 1; i < secondResult.size(); i++) {
    if (secondResult[i - 1].totalDist > secondResult[i].totalDist) {
      secondResult[i].totalDist = secondResult[i - 1].totalDist;
    }
  }

  // We now iterate over all possible combinations of the first set and check if
  // we can cover the whole distance with some combination in the other set
  for (unsigned int i = 0; i < firstResult.size(); i++) {
    Result r1 = firstResult[i];

    long timeLeft = T - r1.totalTime;

    auto it = std::lower_bound(
        secondResult.begin(), secondResult.end(), timeLeft,
        [](const Result &a, long target) { return a.totalTime < target; });

    if (it == secondResult.begin()) {
      continue;
    }

    it--;
    Result r2 = *it;

    long totalDist = r1.totalDist + r2.totalDist;
    if (totalDist >= D) {
      return true;
    }
  }
  return false;
}

void testcase() {

  int n, m;
  long D, T;
  std::cin >> n >> m >> D >> T;

  std::vector<Movement> moves(n);
  std::vector<long> potion(m);

  for (int i = 0; i < n; i++) {
    long d, t;
    std::cin >> d >> t;
    moves[i] = {d, t};
  }

  for (int i = 0; i < m; i++) {
    std::cin >> potion[i];
  }

  if (checkSolution(n, moves, T, D)) {
    std::cout << "0\n";
    return;
  }

  int l = 0;
  int r = m;
  while (l < r) {
    int middle = (l + r) / 2;

    for (int i = 0; i < n; i++) {
      moves[i].d += potion[middle];
    }

    if (checkSolution(n, moves, T, D)) {
      // It might also work with a smaller potion => test it
      r = middle;
    } else {
      l = middle + 1;
    }

    for (int i = 0; i < n; i++) {
      moves[i].d -= potion[middle];
    }
  }

  if (l < m) {
    std::cout << l + 1 << std::endl;
  } else {
    std::cout << "Panoramix captured" << std::endl;
  }
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