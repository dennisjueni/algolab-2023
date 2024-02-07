#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <cmath>
#include <iomanip>
#include <iostream>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt IK;
typedef CGAL::Min_circle_2_traits_2<IK> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

void testcase(int n) {

  std::vector<IK::Point_2> points(n);

  for (int i = 0; i < n; i++) {
    long x, y;
    std::cin >> x >> y;
    IK::Point_2 p(x, y);
    points[i] = p;
  }

  Min_circle mc(points.begin(), points.end(), true);

  Traits::Circle circle = mc.circle();
  IK::FT radius = CGAL::sqrt(circle.squared_radius());

  std::cout << (long)std::ceil(CGAL::to_double(radius)) << "\n";
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