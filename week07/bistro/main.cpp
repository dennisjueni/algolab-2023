#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <boost/pending/disjoint_sets.hpp>
#include <unordered_map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef IK::Point_2 Point2;
typedef CGAL::Delaunay_triangulation_2<IK> Triangulation;

void testcase(int n) {

  std::vector<Point2> pts(n);

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    pts[i] = {x, y};
  }

  Triangulation t;
  t.insert(pts.begin(), pts.end());

  int m;
  std::cin >> m;

  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    Point2 p = Point2(x, y);

    Triangulation::Vertex_handle nearest_triang_pt = t.nearest_vertex(p);
    Point2 triang_pt = nearest_triang_pt->point();
    long squaredDist = CGAL::squared_distance(p, triang_pt);
    std::cout << squaredDist << "\n";
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