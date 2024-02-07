#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <iostream>
#include <queue>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef IK::Point_2 Point2;
typedef IK::Circle_2 Circle2;

typedef CGAL::Triangulation_vertex_base_2<IK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, IK>
    Fb; // Each face gets an index of type int
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK, Tds> Triangulation;

void testcase(int n) {

  int left, bottom, right, top;
  std::cin >> left >> bottom >> right >> top;

  std::vector<Point2> pts(n);

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    pts[i] = {x, y};
  }

  Triangulation t;
  t.insert(pts.begin(), pts.end());

  std::vector<double> minDistances;

  for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
    double x = v->point().x();
    double y = v->point().y();

    double minDist = std::numeric_limits<double>::max();

    Triangulation::Edge_circulator c = t.incident_edges(v);
    // if we only have 1 germ, there are no incident edges, thus we need to
    // check for c == 0
    if (c != 0) {
      do {
        if (t.is_infinite(c)) {
          continue;
        }
        double dist = (std::sqrt(t.segment(c).squared_length()) - 1.0) / 2.0;
        minDist = std::min(minDist, dist);
      } while (++c != t.incident_edges(v));
    }

    minDist = std::min(minDist, std::abs(left - x) - 0.5);
    minDist = std::min(minDist, std::abs(right - x) - 0.5);
    minDist = std::min(minDist, std::abs(top - y) - 0.5);
    minDist = std::min(minDist, std::abs(bottom - y) - 0.5);

    minDistances.push_back(minDist);
  }

  std::sort(minDistances.begin(), minDistances.end());

  int firstDead = std::ceil(std::sqrt(minDistances[0]));
  int halfDead = std::ceil(std::sqrt(minDistances[n / 2]));
  int lastDead = std::ceil(std::sqrt(minDistances[n - 1]));

  std::cout << firstDead << " " << halfDead << " " << lastDead << "\n";
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