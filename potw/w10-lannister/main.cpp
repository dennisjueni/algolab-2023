#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <iostream>
#include <vector>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct House {
  int x;
  int y;
};

void testcase() {

  int numNoble, numCommon;
  long maxSewageLength;
  std::cin >> numNoble >> numCommon >> maxSewageLength;

  std::vector<House> nobleHouses(numNoble);
  std::vector<House> commonHouses(numCommon);

  long sumNobleX = 0;
  long sumNobleY = 0;
  long sumCommonX = 0;
  long sumCommonY = 0;

  Program lp(CGAL::SMALLER, false, 0, false, 0);

  const int a = 0;
  const int b = 1;
  const int c = 2;
  const int d = 3;
  const int e = 4;

  lp.set_u(a, true, 1);
  lp.set_l(a, true, 1);

  int currConstraint = 0;

  for (int i = 0; i < numNoble; i++) {
    int x, y;
    std::cin >> x >> y;

    lp.set_a(a, currConstraint, x);
    lp.set_a(b, currConstraint, y);
    lp.set_a(c, currConstraint, 1);
    lp.set_b(currConstraint, 0);
    currConstraint++;

    nobleHouses[i] = {x, y};
    sumNobleX += x;
    sumNobleY += y;
  }

  for (int i = 0; i < numCommon; i++) {
    int x, y;
    std::cin >> x >> y;

    lp.set_a(a, currConstraint, -x);
    lp.set_a(b, currConstraint, -y);
    lp.set_a(c, currConstraint, -1);
    lp.set_b(currConstraint, 0);
    currConstraint++;

    commonHouses[i] = {x, y};
    sumCommonX += x;
    sumCommonY += y;
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) {
    std::cout << "Yuck!\n";
    return;
  }

  if (maxSewageLength != -1) {
    lp.set_a(b, currConstraint, sumCommonY - sumNobleY);
    lp.set_a(c, currConstraint, numCommon - numNoble);
    lp.set_b(currConstraint, maxSewageLength - sumCommonX + sumNobleX);
    currConstraint++;

    Solution s2 = CGAL::solve_linear_program(lp, ET());
    if (s2.is_infeasible()) {
      std::cout << "Bankrupt!\n";
      return;
    }
  }

  for (int i = 0; i < numNoble; i++) {
    lp.set_a(b, currConstraint, -nobleHouses[i].x);
    lp.set_a(d, currConstraint, -1);
    lp.set_a(e, currConstraint, -1);
    lp.set_b(currConstraint, -nobleHouses[i].y);
    currConstraint++;

    lp.set_a(b, currConstraint, nobleHouses[i].x);
    lp.set_a(d, currConstraint, 1);
    lp.set_a(e, currConstraint, -1);
    lp.set_b(currConstraint, nobleHouses[i].y);
    currConstraint++;
  }

  for (int i = 0; i < numCommon; i++) {
    lp.set_a(b, currConstraint, -commonHouses[i].x);
    lp.set_a(d, currConstraint, -1);
    lp.set_a(e, currConstraint, -1);
    lp.set_b(currConstraint, -commonHouses[i].y);
    currConstraint++;

    lp.set_a(b, currConstraint, commonHouses[i].x);
    lp.set_a(d, currConstraint, 1);
    lp.set_a(e, currConstraint, -1);
    lp.set_b(currConstraint, commonHouses[i].y);
    currConstraint++;
  }

  lp.set_c(e, 1);

  Solution s3 = CGAL::solve_linear_program(lp, ET());

  std::cout << (long)std::ceil(CGAL::to_double(s3.objective_value())) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}