#include <iostream>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Circle_2 Circle;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long floor_to_long(const CGAL::Quotient<CGAL::Gmpz> &x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x)
    a -= 1;
  while (a + 1 <= x)
    a += 1;
  return a;
}

void testcase() {

  int numWarehouses, numStadiums, numContourLines;
  std::cin >> numWarehouses >> numStadiums >> numContourLines;

  // numWarehouses * numStadiums variables
  Program lp(CGAL::SMALLER, false, 0, false, 0);
  int currConstraint = 0;

  std::vector<int> alcohol = std::vector<int>(numWarehouses, 0);
  std::vector<int> alcoholLimit = std::vector<int>(numStadiums, 0);
  std::vector<Point> points = std::vector<Point>(numWarehouses + numStadiums);

  for (int i = 0; i < numWarehouses; i++) {
    int x, y, s;
    std::cin >> x >> y >> s >> alcohol[i];
    points[i] = {x, y};

    // the amount shipped per warehouse cannot exceed the supply and needs to be
    // positive
    for (int j = 0; j < numStadiums; j++) {
      lp.set_a(i * numStadiums + j, currConstraint, 1);
      lp.set_l(i * numStadiums + j, true, 0);
    }

    lp.set_b(currConstraint, s);
    currConstraint++;
  }

  for (int i = 0; i < numStadiums; i++) {
    int x, y, d;
    std::cin >> x >> y >> d >> alcoholLimit[i];
    points[numWarehouses + i] = {x, y};

    for (int j = 0; j < numWarehouses; j++) {
      lp.set_a(j * numStadiums + i, currConstraint, 1);
      lp.set_a(j * numStadiums + i, currConstraint + 1, -1);
    }
    lp.set_b(currConstraint, d);
    lp.set_b(currConstraint + 1, -1 * d);
    currConstraint += 2;
  }

  for (int i = 0; i < numStadiums; i++) {
    for (int j = 0; j < numWarehouses; j++) {
      lp.set_a(j * numStadiums + i, currConstraint, alcohol[j]);
    }
    lp.set_b(currConstraint, 100 * alcoholLimit[i]);
    currConstraint++;
  }

  std::vector<std::vector<int>> rewards = std::vector<std::vector<int>>(
      numWarehouses, std::vector<int>(numStadiums));

  for (int i = 0; i < numWarehouses; i++) {
    for (int j = 0; j < numStadiums; j++) {
      int reward;
      std::cin >> reward;
      rewards[i][j] = 100 * reward;
    }
  }

  Triangulation t;
  t.insert(points.begin(), points.end());

  for (int i = 0; i < numContourLines; i++) {
    int x, y, r;
    std::cin >> x >> y >> r;
    Point center = {x, y};

    auto nearestPoint = t.nearest_vertex(center)->point();

    if (CGAL::squared_distance(center, nearestPoint) > r * r) {
      continue;
    }

    for (int j = 0; j < numWarehouses; j++) {
      for (int k = 0; k < numStadiums; k++) {
        auto dist1 = CGAL::squared_distance(points[j], center);
        auto dist2 = CGAL::squared_distance(points[numWarehouses + k], center);

        bool isWarehouseInside = dist1 < r * r;
        bool isStadiumInside = dist2 < r * r;

        if ((isWarehouseInside && !isStadiumInside) ||
            (!isWarehouseInside && isStadiumInside)) {
          rewards[j][k]--;
        }
      }
    }
  }

  for (int i = 0; i < numWarehouses; i++) {
    for (int j = 0; j < numStadiums; j++) {
      lp.set_c(i * numStadiums + j, -1 * rewards[i][j]);
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible()) {
    std::cout << "RIOT!\n";
    return;
  }

  std::cout << floor_to_long(s.objective_value() / -100) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
