#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <tuple>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

typedef K::Point_2 Point2;
typedef std::pair<Point2, int> IndexedPoint;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<int, int, K::FT> Edge;

void testcase() {
  int n, m, k;
  double s; // = 4*r^2
  std::cin >> n >> m >> s >> k;

  std::vector<IndexedPoint> trees;
  trees.reserve(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    trees.emplace_back(Point2(x, y), i);
  }

  std::vector<Point2> bones;
  bones.reserve(m);
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    bones.emplace_back(Point2(x, y));
  }

  // By giving the points paired with the indices, these indices are used to
  // initialize the vertex info accordingly.
  Delaunay t;
  t.insert(trees.begin(), trees.end());

  std::vector<Edge> edges;
  edges.reserve(3 * n + m); // there can be no more in a planar graph
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

  // For first part: Calculate the number of disjoint sets that are found with
  // radius r Then go through the bones and look if in radius of closest tree
  // and if so, add it to the respective set

  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);

  for (auto e = edges.begin(); e != edges.end(); e++) {
    // determine components of endpoints
    int c1 = uf.find_set(std::get<0>(*e));
    int c2 = uf.find_set(std::get<1>(*e));
    long dist = std::get<2>(*e);

    if (dist > s) {
      // as soon as we are adding distances that are too large we stop and check
      // other stuff first
      break;
    }

    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
    }
  }

  std::vector<int> bonesPerComponent(n, 0);

  for (Point2 bone : bones) {

    Delaunay::Vertex_handle nearest_triang_pt = t.nearest_vertex(bone);
    Point2 triang_pt = nearest_triang_pt->point();
    double squaredDist = CGAL::squared_distance(bone, triang_pt);

    if (4.0 * squaredDist <= s) {
      int c1 = uf.find_set(nearest_triang_pt->info());
      bonesPerComponent[c1]++;
    }
  }

  int a = 0;
  for (int num : bonesPerComponent) {
    a = std::max(a, num);
  }

  //------PART 2------

  std::vector<int> bonesPerComponent_b(n + m, 0);

  for (unsigned int i = 0; i < bones.size(); i++) {

    Point2 bone = bones[i];

    Delaunay::Vertex_handle nearest_triang_pt = t.nearest_vertex(bone);
    Point2 triang_pt = nearest_triang_pt->point();
    K::FT squaredDist = 4.0 * CGAL::squared_distance(bone, triang_pt);

    edges.emplace_back(nearest_triang_pt->info(), n + i, squaredDist);
    bonesPerComponent_b[n + i] = 1;
  }

  std::sort(edges.begin(), edges.end(),
            [](const Edge &e1, const Edge &e2) -> bool {
              return std::get<2>(e1) < std::get<2>(e2);
            });

  boost::disjoint_sets_with_storage<> uf_b(n + m);
  K::FT b = 0;

  for (auto e = edges.begin(); e != edges.end(); e++) {
    // determine components of endpoints
    int c1 = uf_b.find_set(std::get<0>(*e));
    int c2 = uf_b.find_set(std::get<1>(*e));
    double dist = std::get<2>(*e);

    if (c1 != c2) {
      // this edge connects two different components => part of the emst

      int numBones1 = bonesPerComponent_b[c1];
      bonesPerComponent_b[c1] = 0;
      int numBones2 = bonesPerComponent_b[c2];
      bonesPerComponent_b[c2] = 0;

      uf_b.link(c1, c2);
      int c3 = uf_b.find_set(std::get<0>(*e));
      bonesPerComponent_b[c3] = numBones1 + numBones2;

      if (bonesPerComponent_b[c3] >= k) {
        b = dist;
        break;
      }
    }
  }

  std::cout << a << " " << b << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
  return 0;
}