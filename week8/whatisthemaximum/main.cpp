#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void type1(int a, int b) {

  // lower bound of 0, no upper bound
  Program lp(CGAL::SMALLER, true, 0, false, 0);

  const int X = 0;
  const int Y = 1;

  lp.set_a(X, 0, 1);
  lp.set_a(Y, 0, 1);
  lp.set_b(0, 4); // x + y  <= 4
  lp.set_a(X, 1, 4);
  lp.set_a(Y, 1, 2);
  lp.set_b(1, a * b); // 4x + 2y <= ab
  lp.set_a(X, 2, -1);
  lp.set_a(Y, 2, 1);
  lp.set_b(2, 1); // −x+y<=1

  lp.set_c(X, a);
  lp.set_c(Y, (-1) * b);

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.status() == CGAL::QP_OPTIMAL) {
    int result = std::floor((-1.0) * CGAL::to_double(s.objective_value()));
    if (result == 0 && std::signbit(result)) {
      result = 0;
    }

    std::cout << result << "\n";
  } else if (s.status() == CGAL::QP_INFEASIBLE) {
    std::cout << "no\n";
  } else if (s.status() == CGAL::QP_UNBOUNDED) {
    std::cout << "unbounded\n";
  }
}

void type2(int a, int b) {

  // no lower bound, upper bound of 0
  Program lp(CGAL::LARGER, false, 0, true, 0);

  const int X = 0;
  const int Y = 1;
  const int Z = 2;

  lp.set_a(X, 0, 1);
  lp.set_a(Y, 0, 1);
  lp.set_a(Z, 0, 0);
  lp.set_b(0, -4); // x + y >= −4
  lp.set_a(X, 1, 4);
  lp.set_a(Y, 1, 2);
  lp.set_a(Z, 1, 1);
  lp.set_b(1, (-1) * a * b); // 4x + 2y + z >= −ab
  lp.set_a(X, 2, -1);
  lp.set_a(Y, 2, 1);
  lp.set_a(Z, 2, 0);
  lp.set_b(2, -1); // −x + y => −1

  lp.set_c(X, a);
  lp.set_c(Y, b);
  lp.set_c(Z, 1);

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.status() == CGAL::QP_OPTIMAL) {
    int result = std::ceil(CGAL::to_double(s.objective_value()));

    std::cout << result << "\n";
  } else if (s.status() == CGAL::QP_INFEASIBLE) {
    std::cout << "no\n";
  } else if (s.status() == CGAL::QP_UNBOUNDED) {
    std::cout << "unbounded\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int p;
  std::cin >> p;

  while (p != 0) {
    int a, b;
    std::cin >> a >> b;
    if (p == 1) {
      type1(a, b);
    } else {
      type2(a, b);
    }
    std::cin >> p;
  }

  return 0;
}