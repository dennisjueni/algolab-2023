#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Ray_2 Ray;

struct SortableRay {
  int idx;
  long yIntercept;
  Ray ray;
};

struct IndexedRoutes {
  int idx;
  int oldIdx;
};

void testcase() {
  int n;
  std::cin >> n;

  std::vector<SortableRay> rays = std::vector<SortableRay>(n);

  for (int i = 0; i < n; i++) {
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;

    Point start = {0, y0};
    Point direction = {x1, y1};

    Ray ray = {start, direction};

    rays[i] = {i, y0, ray};
  }

  std::sort(rays.begin(), rays.end(), [](SortableRay r1, SortableRay r2) {
    return r1.yIntercept > r2.yIntercept;
  });

  std::vector<IndexedRoutes> validBikers = std::vector<IndexedRoutes>();
  validBikers.push_back({0, rays[0].idx});

  for (int i = 1; i < n; i++) {

    bool isPossible = false;

    while (!isPossible) {
      int lowestIndex = validBikers[validBikers.size() - 1].idx;
      Ray ray1 = rays[lowestIndex].ray;
      Ray ray2 = rays[i].ray;

      if (CGAL::do_intersect(ray1, ray2)) {
        auto o = CGAL::intersection(ray1, ray2);
        const Point *intersection = boost::get<Point>(&*o);

        auto dist1 = CGAL::squared_distance(
            Point(0, rays[lowestIndex].yIntercept), *intersection);
        auto dist2 =
            CGAL::squared_distance(Point(0, rays[i].yIntercept), *intersection);

        if (dist1 >= dist2) {
          // If either the lowestIndex Biker arrives later or at the same time
          // (he is the one from the left) we can remove it and are not done yet
          validBikers.pop_back();

          if (validBikers.empty()) {
            validBikers.push_back({i, rays[i].idx});
            isPossible = true;
          }
        } else {
          // This means we just don't add the new biker and keep the old one
          isPossible = true;
        }
      } else {
        validBikers.push_back({i, rays[i].idx});
        isPossible = true;
      }
    }
  }

  std::sort(
      validBikers.begin(), validBikers.end(),
      [](IndexedRoutes r1, IndexedRoutes r2) { return r1.oldIdx < r2.oldIdx; });

  for (auto x : validBikers) {
    std::cout << x.oldIdx << " ";
  }
  std::cout << "\n";

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}