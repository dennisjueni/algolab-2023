//
#include <iostream>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

typedef int IT;
typedef double ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Stall {
  int initialFilth;
  int maxFilth;
  int k;
  int l;
  int m;
};

int n;
std::vector<Stall> stalls;

bool solveLP(int a, int p) {
  Program lp(CGAL::SMALLER, true, 0, true, 1);
  const int h1 = 0;
  const int h2 = 1;
  const int h3 = 2;

  for (int i = 0; i < n; i++) {
    lp.set_a(h1, i, -1 * (stalls[i].k + a * a));
    lp.set_a(h2, i, -1 * (stalls[i].l + p * p));
    lp.set_a(h3, i, -1 * (stalls[i].m + a * p));

    lp.set_b(i, stalls[i].maxFilth - stalls[i].initialFilth);
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  return !s.is_infeasible();
}

void testcase() {
  std::cin >> n;

  stalls = std::vector<Stall>(n);

  for (int i = 0; i < n; i++) {
    int initialFilth, maxFilth;
    std::cin >> initialFilth >> maxFilth;

    int k, l, m;
    std::cin >> k >> l >> m;

    stalls[i] = {initialFilth, maxFilth, k, l, m};
  }

  std::vector<int> river1 = std::vector<int>(25, 0);
  std::vector<int> river2 = std::vector<int>(25, 0);

  for (int i = 1; i < 25; i++) {
    int additionalFlow;
    std::cin >> additionalFlow;
    river1[i] = river1[i - 1] + additionalFlow;
  }
  for (int i = 1; i < 25; i++) {
    int additionalFlow;
    std::cin >> additionalFlow;
    river2[i] = river2[i - 1] + additionalFlow;
  }

  if (solveLP(river1[0], river2[0])) {
    std::cout << "0\n";
    return;
  }

  std::vector<int> memo1 = std::vector<int>(25, -1);
  std::vector<int> memo2 = std::vector<int>(25, -1);

  int minHours = 49;
  int left = 0;
  int right = 48;

  while (left <= right) {
    int middle = (left + right) / 2;

    bool hasSolution = false;

    for (int i = 0; i <= std::min(middle, 24); i++) {
      if (middle - i > 24) {
        continue;
      }
      int j = middle - i;

      // If we know (i,j*) for j* > j does not solve it, (i,j) won't solve it
      // either If we know (i*,j) for i* > i does not solve it, (i,j) won't
      // solve it either
      if (memo1[i] >= j || memo2[j] >= i) {
        continue;
      }

      if (solveLP(river1[i], river2[j])) {
        hasSolution = true;
        minHours = middle;
        break;
      } else {
        memo1[i] = std::max(memo1[i], j);
        memo2[j] = std::max(memo2[j], i);
      }
    }

    if (hasSolution) {
      right = middle - 1;
    } else {
      left = middle + 1;
    }
  }

  if (minHours == 49) {
    std::cout << "Impossible!\n";
  } else {
    std::cout << minHours << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}