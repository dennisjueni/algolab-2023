/// 1
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef IK::Point_2 Point2;

typedef CGAL::Triangulation_vertex_base_2<IK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, IK>
    Fb; // Each face gets an index of type int
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK, Tds> Triangulation;

struct Face {
  int id;
  double dist;
};

void testcase(int n) {

  std::vector<Point2> pts(n);

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    pts[i] = {x, y};
  }

  Triangulation t;
  t.insert(pts.begin(), pts.end());

  // assign an index to every (finite) face we have in our triangulation
  // start with one, so that infinite faces can get index 0
  int current_face_index = 1;
  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
    f->info() = current_face_index;
    current_face_index++;
  }

  std::vector<std::vector<std::pair<int, double>>> adjList(current_face_index);

  // we can now create an adjacency list where vertices are faces in the
  // triangulation and edges exist if two faces are immediate neighbors. The
  // weight of an edge is set to the distance between the two points that
  // connect two edges

  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {

    int index1 = f->info();

    for (int i = 0; i < 3; i++) {
      auto neighbor_face = f->neighbor(i);
      auto point1 = f->vertex((i + 1) % 3)->point();
      auto point2 = f->vertex((i + 2) % 3)->point();

      int index2;
      if (t.is_infinite(neighbor_face)) {
        index2 = 0;
      } else {
        index2 = neighbor_face->info();
      }

      double dist = CGAL::squared_distance(point1, point2);

      adjList[index1].push_back({index2, dist});
      // In this case we can do it as follow, however it is actually not that
      // necessary for all faces except for the infinite face. We need to do it
      // for the infinite face!
      adjList[index2].push_back({index1, dist});
    }
  }

  // we now use a prioQueue like approach to store the max distance a disk in a
  // face can be and still get out if the initial starting point is in the
  // infinite face, it will always get out, so this is our starting point

  std::vector<double> maxSize(current_face_index, -1.0);

  auto comp = [](Face a, Face b) { return a.dist < b.dist; };
  std::priority_queue<Face, std::vector<Face>, decltype(comp)> prioQueue(comp);

  // the infinite face has no limit on the distance of a disk
  prioQueue.push({0, std::numeric_limits<double>::max()});

  while (!prioQueue.empty()) {

    Face curr = prioQueue.top();
    prioQueue.pop();

    if (maxSize[curr.id] != -1.0) {
      // this means we have already checked and updated this face
      continue;
    }

    maxSize[curr.id] = curr.dist;

    for (auto neighbor : adjList[curr.id]) {
      prioQueue.push({neighbor.first, std::min(neighbor.second, curr.dist)});
    }
  }

  int m;
  std::cin >> m;

  std::string result = "";

  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    Point2 p = Point2(x, y);

    double d;
    std::cin >> d;

    Triangulation::Vertex_handle nearest_triang_pt = t.nearest_vertex(p);
    Point2 triang_pt = nearest_triang_pt->point();
    double squaredDist = CGAL::squared_distance(p, triang_pt);

    if (d > squaredDist) {
      result += "n";
      continue;
    }

    auto f = t.locate(p);
    int index = t.is_infinite(f) ? 0 : f->info();

    if (maxSize[index] >= 4 * d) {
      result += "y";
    } else {
      result += "n";
    }
  }
  std::cout << result << "\n";
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