#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n, int m) {

  // lower bound of 0, no upper bound
  Program lp(CGAL::SMALLER, true, 0, false, 0);

  for (int i = 0; i < n; i++) {
    int minVal, maxVal;
    std::cin >> minVal >> maxVal;

    lp.set_b(2 * i, maxVal);
    lp.set_b(2 * i + 1, -1 * minVal);
  }

  for (int i = 0; i < m; i++) {

    int price;
    std::cin >> price;

    // want to minimize price * i-th product
    lp.set_c(i, price);

    for (int j = 0; j < n; j++) {
      int amount;
      std::cin >> amount;

      lp.set_a(i, 2 * j, amount);
      lp.set_a(i, 2 * j + 1, -amount);
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.status() == CGAL::QP_OPTIMAL) {
    int result = std::floor(CGAL::to_double(s.objective_value()));

    std::cout << result << "\n";
  } else if (s.status() == CGAL::QP_INFEASIBLE) {
    std::cout << "No such diet.\n";
  } else if (s.status() == CGAL::QP_UNBOUNDED) {
    std::cout << "No such diet???\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n, m;
  std::cin >> n >> m;

  while (!(n == 0 && m == 0)) {
    testcase(n, m);
    std::cin >> n >> m;
  }

  return 0;
}