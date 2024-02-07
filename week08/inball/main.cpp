#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n) {

  int d;
  std::cin >> d;

  // no lower bound, no upper bound
  Program lp(CGAL::SMALLER, false, 0, false, 0);

  for (int i = 0; i < n; i++) {

    long norm = 0;
    for (int j = 0; j < d; j++) {

      int a;
      std::cin >> a;

      // set j-th variable in i-th equality to a
      lp.set_a(j, i, a);
      norm += a * a;
    }

    norm = sqrt(norm);
    // Set radius in i-th equation
    lp.set_a(d, i, norm);

    int b;
    std::cin >> b;

    lp.set_b(i, b);
  }

  // dont want negative radius
  lp.set_l(d, true, 0);
  // want to maximize d == minimize -d
  lp.set_c(d, -1);

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.status() == CGAL::QP_OPTIMAL) {
    int result = std::floor((-1.0) * CGAL::to_double(s.objective_value()));
    std::cout << result << "\n";
  } else if (s.status() == CGAL::QP_INFEASIBLE) {
    std::cout << "none\n";
  } else if (s.status() == CGAL::QP_UNBOUNDED) {
    std::cout << "inf\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }

  return 0;
}