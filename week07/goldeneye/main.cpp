#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <tuple>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, IK> Vb;
typedef CGAL::Triangulation_face_base_2<IK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK, Tds> Delaunay;

typedef IK::Point_2 Point2;
typedef std::pair<Point2, int> IndexedPoint;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<int, int, IK::FT> Edge;

void testcase() {
  int n, m;
  double p;
  std::cin >> n >> m >> p;

  std::vector<IndexedPoint> points(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    points[i] = std::make_pair(Point2(x, y), i);
  }

  // O(n log(n)) if order of points is random (which they are)
  Delaunay t;
  t.insert(points.begin(), points.end());

  // extract edges and sort by (squared) length
  // This step takes O(n log n) time (for the sorting).
  std::vector<Edge> edges;
  edges.reserve(3 * n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    int idx1 = e->first->vertex((e->second + 1) % 3)->info();
    int idx2 = e->first->vertex((e->second + 2) % 3)->info();

    // ensure smaller index comes first
    if (idx1 > idx2) {
      std::swap(idx1, idx2);
    }
    edges.emplace_back(idx1, idx2, t.segment(e).squared_length());
  }

  std::sort(edges.begin(), edges.end(),
            [](const Edge &e1, const Edge &e2) -> bool {
              return std::get<2>(e1) < std::get<2>(e2);
            });

  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf_p(n);
  boost::disjoint_sets_with_storage<> uf_a(n);
  boost::disjoint_sets_with_storage<> uf_b(n);

  long a = 0;
  long b = 0;

  for (auto e = edges.begin(); e != edges.end(); e++) {
    // determine components of endpoints
    int c1 = uf_p.find_set(std::get<0>(*e));
    int c2 = uf_p.find_set(std::get<1>(*e));
    long dist = std::get<2>(*e);

    // ((dist/2)^2 = dist^2 / 4 <= p/4)
    if (dist > p) {
      // as soon as we are adding distances that are too large we stop merging
      // components
      break;
    }

    if (c1 != c2) {
      // this edge connects two different components => connect the two
      // components
      uf_p.link(c1, c2);
    }
  }

  std::string c = "";

  // Check if missions possible with current power consumption
  for (int i = 0; i < m; i++) {
    int x0, y0, x1, y1;
    std::cin >> x0 >> y0 >> x1 >> y1;

    Point2 p1 = Point2(x0, y0);
    Point2 p2 = Point2(x1, y1);

    auto nearest_pt1 = t.nearest_vertex(p1);
    auto nearest_pt2 = t.nearest_vertex(p2);

    long squaredDist1 = CGAL::squared_distance(p1, nearest_pt1->point());
    long squaredDist2 = CGAL::squared_distance(p2, nearest_pt2->point());

    int vertexInfo1 = nearest_pt1->info();
    int vertexInfo2 = nearest_pt2->info();

    int c1 = uf_p.find_set(vertexInfo1);
    int c2 = uf_p.find_set(vertexInfo2);

    long maxDistStart = 4.0 * std::max(squaredDist1, squaredDist2);

    if (maxDistStart <= p && c1 == c2) {
      c += "y";

      long distNeeded = 0;

      for (auto e = edges.begin(); e != edges.end(); e++) {

        // If the two vertices closest to s & t are in the same component we are
        // done
        int b1 = uf_b.find_set(vertexInfo1);
        int b2 = uf_b.find_set(vertexInfo2);
        if (b1 == b2) {
          break;
        }

        // Look at the components of the edge we are currently looking at
        int c1 = uf_b.find_set(std::get<0>(*e));
        int c2 = uf_b.find_set(std::get<1>(*e));
        distNeeded = std::get<2>(*e);

        if (c1 != c2) {
          // this edge connects two different components => connect the two
          // components
          uf_b.link(c1, c2);
        }
      }
      b = std::max({distNeeded, maxDistStart, b});
    } else {
      c += "n";
    }

    long distNeeded = 0;

    for (auto e = edges.begin(); e != edges.end(); e++) {

      // If the two vertices closest to s & t are in the same component we are
      // done
      int a1 = uf_a.find_set(vertexInfo1);
      int a2 = uf_a.find_set(vertexInfo2);
      if (a1 == a2) {
        break;
      }
      // determine components of endpoints
      int c1 = uf_a.find_set(std::get<0>(*e));
      int c2 = uf_a.find_set(std::get<1>(*e));
      distNeeded = std::get<2>(*e);

      if (c1 != c2) {
        // this edge connects two different components => connect the two
        // components
        uf_a.link(c1, c2);
      }
    }
    a = std::max({distNeeded, maxDistStart, a});
  }

  std::cout << c << "\n";
  std::cout << a << "\n";
  std::cout << b << "\n";
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