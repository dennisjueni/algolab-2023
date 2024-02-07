/// 4
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
  // create an LP with Ax <= b and no lower/upper bounds
  Program lp(CGAL::SMALLER, false, 0, false, 0);
  const int x = 0;
  const int y = 1;
  const int z = 2;

  int initial_x, initial_y, numLegions;
  std::cin >> initial_x >> initial_y >> numLegions;

  for (int i = 0; i < numLegions; i++) {
    long a, b, c;
    int v;
    std::cin >> a >> b >> c >> v;

    long initialLocation = a * initial_x + b * initial_y + c;
    long coeff = v * std::sqrt(a * a + b * b);

    if (initialLocation < 0) {
      lp.set_a(x, i, a);
      lp.set_a(y, i, b);
      lp.set_a(z, i, coeff);

      lp.set_b(i, -c);
    } else if (initialLocation > 0) {
      lp.set_a(x, i, -a);
      lp.set_a(y, i, -b);
      lp.set_a(z, i, coeff);

      lp.set_b(i, c);
    } else {
      std::cout << "REACHED A POINT THAT SHOULD NEVER HAPPEN\n";
    }
  }

  lp.set_c(z, -1);

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << -1 * (long)CGAL::to_double(s.objective_value()) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
