#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef K::Point_2 Point;

void testcase() {

  int numDementors, numFugitives;
  long d;
  std::cin >> numDementors >> numFugitives >> d;

  std::vector<Point> dementors = std::vector<Point>(numDementors);

  for (int i = 0; i < numDementors; i++) {
    long x, y;
    std::cin >> x >> y;

    dementors[i] = {x, y};
  }

  Delaunay t;
  t.insert(dementors.begin(), dementors.end());

  // 0 corresponds to infinite face
  int numFaces = 1;
  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
    f->info() = numFaces;
    numFaces++;
  }

  std::vector<std::vector<std::pair<int, K::FT>>> adjList =
      std::vector<std::vector<std::pair<int, K::FT>>>(numFaces);

  std::vector<K::FT> faceRadii = std::vector<K::FT>(numFaces);

  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {

    int currIdx = f->info();

    for (int i = 0; i < 3; i++) {

      auto neighboringFace = f->neighbor(i);
      int neighborIdx = 0;

      if (!t.is_infinite(neighboringFace)) {
        neighborIdx = neighboringFace->info();
      }

      Point v1 = f->vertex((i + 1) % 3)->point();
      Point v2 = f->vertex((i + 2) % 3)->point();

      K::FT dist = CGAL::squared_distance(v1, v2);

      adjList[currIdx].push_back({neighborIdx, dist});
      if (neighborIdx == 0) {
        adjList[neighborIdx].push_back({currIdx, dist});
      }
    }
    faceRadii[currIdx] = CGAL::squared_radius(
        f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
  }

  // Now do prioqueue approach
  auto comparator = [](std::pair<int, K::FT> a, std::pair<int, K::FT> b) {
    return a.second < b.second;
  };

  std::priority_queue<std::pair<int, K::FT>, std::vector<std::pair<int, K::FT>>,
                      decltype(comparator)>
      prioQueue(comparator);

  K::FT maximum = K::FT(std::numeric_limits<long>::max());
  prioQueue.push({0, 4 * (maximum + maximum) * (maximum + maximum)});

  for (int i = 0; i < numFaces; i++) {
    prioQueue.push({i, faceRadii[i]});
  }

  std::vector<K::FT> faceMax = std::vector<K::FT>(numFaces, -1);

  while (!prioQueue.empty()) {

    auto current = prioQueue.top();
    int idx = current.first;
    K::FT dist = current.second;

    prioQueue.pop();

    if (faceMax[idx] != -1) {
      continue;
    }
    faceMax[idx] = dist;

    for (auto neighbor : adjList[idx]) {
      if (faceMax[neighbor.first] == -1) {
        prioQueue.push({neighbor.first, std::min(dist, neighbor.second)});
      }
    }
  }

  for (int i = 0; i < numFugitives; i++) {
    long x, y, s;
    std::cin >> x >> y >> s;

    Point fugitive = {x, y};

    Point nearestPoint = t.nearest_vertex(fugitive)->point();

    if (CGAL::squared_distance(fugitive, nearestPoint) <
        K::FT(s + d) * K::FT(s + d)) {
      std::cout << "n";
      continue;
    }

    auto face = t.locate(fugitive);

    int faceIdx = 0;
    if (!t.is_infinite(face)) {
      faceIdx = face->info();
    }

    if (faceMax[faceIdx] >=
        K::FT(4) * K::FT(K::FT(s) + K::FT(d)) * K::FT(K::FT(s) + K::FT(d))) {
      std::cout << "y";
    } else {
      std::cout << "n";
    }
  }

  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}