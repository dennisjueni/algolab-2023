/// 1
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <algorithm>
#include <iomanip>
#include <iostream>

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef EK::Point_2 P;
typedef EK::Ray_2 R;
typedef EK::Segment_2 S;

long floor_to_double(const EK::FT &x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x)
    a -= 1;
  while (a + 1 <= x)
    a += 1;
  return static_cast<long>(a);
}

void testcase(int n) {
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;

  P start(x, y);
  P intermediate(a, b);

  R ray(start, intermediate);
  std::vector<S> segments(n);

  for (int i = 0; i < n; i++) {
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;

    P p1 = P(r, s);
    P p2 = P(t, u);
    segments[i] = S(p1, p2);
  }

  std::random_shuffle(segments.begin(), segments.end());

  bool hits = false;
  P firstHit;
  S newSegment;

  for (int i = 0; i < n; i++) {
    if (!hits) {
      // Check if the ray intersects with the segment
      if (CGAL::do_intersect(ray, segments[i])) {
        hits = true;
        const auto result = CGAL::intersection(ray, segments[i]);
        if (const S *s = boost::get<S>(&*result)) {
          firstHit = s->source();
        } else {
          firstHit = *boost::get<P>(&*result);
        }
        newSegment = S(start, firstHit);
      }
    } else {
      // Check if the segment intersects with the segment
      if (CGAL::do_intersect(newSegment, segments[i])) {
        const auto result = CGAL::intersection(newSegment, segments[i]);
        if (const S *s = boost::get<S>(&*result)) {
          firstHit = s->source();
        } else {
          firstHit = *boost::get<P>(&*result);
        }
        newSegment = S(start, firstHit);
      }
    }
  }

  if (hits) {
    std::cout << floor_to_double(firstHit.x()) << " "
              << floor_to_double(firstHit.y()) << "\n";
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