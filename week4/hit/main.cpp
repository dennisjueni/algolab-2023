#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iomanip>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;

void testcase(int n) {
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;

  IK::Point_2 start(x, y);
  IK::Point_2 intermediate(a, b);

  IK::Ray_2 ray(start, intermediate);

  bool intersects = false;

  for (int i = 0; i < n; i++) {
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;

    if (!intersects) {
      IK::Point_2 start(r, s);
      IK::Point_2 end(t, u);
      IK::Segment_2 seg(start, end);

      if (CGAL::do_intersect(ray, seg)) {
        intersects = true;
      }
    }
  }

  if (intersects) {
    std::cout << "yes\n";
  } else {
    std::cout << "no\n";
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