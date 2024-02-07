/// 4
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <iostream>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET> &x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x)
    a -= 1;
  while (a + 1 <= x)
    a += 1;
  return a;
}

struct NailPos {
  int x;
  int y;
};

void testcase() {

  int n, m, h, w;

  std::cin >> n >> m >> h >> w;

  // lower bound of 1, no upper bound
  Program lp(CGAL::SMALLER, true, 1, false, 0);

  std::vector<NailPos> freePos(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;

    freePos[i] = {x, y};

    lp.set_c(i, -2 * (h + w));
  }

  std::vector<NailPos> occupiedPos(m);
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;

    occupiedPos[i] = {x, y};
  }

  int constraintNum = 0;

  // We first add constraints for every pair of new posters
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      NailPos pos1 = freePos[i];
      NailPos pos2 = freePos[j];

      int widthDiff = std::abs(pos1.x - pos2.x);
      int heightDiff = std::abs(pos1.y - pos2.y);
      double b = std::max(2.0 * widthDiff / w, 2.0 * heightDiff / h);

      lp.set_a(i, constraintNum, 1);
      lp.set_a(j, constraintNum, 1);
      lp.set_b(constraintNum, b);

      constraintNum++;
    }
  }

  for (int i = 0; i < n; i++) {

    double minValue = std::numeric_limits<double>::max();
    for (int j = 0; j < m; j++) {
      NailPos pos1 = freePos[i];
      NailPos pos2 = occupiedPos[j];

      int widthDiff = std::abs(pos1.x - pos2.x);
      int heightDiff = std::abs(pos1.y - pos2.y);

      double maxDiff =
          std::max((2.0 * widthDiff / w) - 1, (2.0 * heightDiff / h) - 1);
      minValue = std::min(minValue, maxDiff);
    }

    lp.set_u(i, true, minValue);
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_unbounded()) {
    std::cout << "unbounded" << std::endl;
  } else {
    auto objValue = s.objective_value();
    std::cout << long(-1 * floor_to_double(objValue)) << std::endl;
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