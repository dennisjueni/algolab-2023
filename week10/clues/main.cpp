#include <iostream>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

typedef boost::default_color_type color;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

typedef K::Point_2 Point;
typedef std::pair<K::Point_2, int> IPoint;

void testcase() {

  int numStations, numClues;
  long radius;
  std::cin >> numStations >> numClues >> radius;

  std::vector<IPoint> points = std::vector<IPoint>(numStations);

  for (int i = 0; i < numStations; i++) {
    int x, y;
    std::cin >> x >> y;

    points[i] = {{x, y}, i};
  }

  Delaunay t;
  t.insert(points.begin(), points.end());

  graph G(numStations);

  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    int i1 = e->first->vertex((e->second + 1) % 3)->info();
    int i2 = e->first->vertex((e->second + 2) % 3)->info();

    if (t.segment(e).squared_length() <= radius * radius) {
      boost::add_edge(i1, i2, G);
    }
  }

  std::vector<color> vertex_color(numStations);

  bool isBipartite = is_bipartite(
      G, boost::get(boost::vertex_index, G),
      boost::make_iterator_property_map(vertex_color.begin(),
                                        boost::get(boost::vertex_index, G)));

  if (isBipartite) {

    std::vector<IPoint> whitePoints;
    std::vector<IPoint> blackPoints;

    // Create two new Delaunay triangulations
    for (int i = 0; i < numStations; i++) {
      if (vertex_color[i] == boost::color_traits<color>::white()) {
        whitePoints.push_back(points[i]);
      } else {
        blackPoints.push_back(points[i]);
      }
    }

    Delaunay tWhite;
    tWhite.insert(whitePoints.begin(), whitePoints.end());

    for (auto e = tWhite.finite_edges_begin(); e != tWhite.finite_edges_end();
         ++e) {
      if (tWhite.segment(e).squared_length() <= radius * radius) {
        isBipartite = false;
        break;
      }
    }

    if (isBipartite) {
      Delaunay tBlack;
      tBlack.insert(blackPoints.begin(), blackPoints.end());

      for (auto e = tBlack.finite_edges_begin(); e != tBlack.finite_edges_end();
           ++e) {
        if (tBlack.segment(e).squared_length() <= radius * radius) {
          isBipartite = false;
          break;
        }
      }
    }
  }

  if (!isBipartite) {
    for (int i = 0; i < numClues; i++) {
      int x1, y1, x2, y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      std::cout << "n";
    }
    std::cout << "\n";
    return;
  }

  // We now know that there is no interference -> can now use connected
  // components

  std::vector<int> component_map(numStations);
  boost::connected_components(
      G, boost::make_iterator_property_map(component_map.begin(),
                                           boost::get(boost::vertex_index, G)));

  for (int i = 0; i < numClues; i++) {
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;

    Point clue1 = {x1, y1};
    Point clue2 = {x2, y2};

    if (CGAL::squared_distance(clue1, clue2) <= radius * radius) {
      std::cout << "y";
      continue;
    }

    auto nearestVertex1 = t.nearest_vertex(clue1);
    auto nearestVertex2 = t.nearest_vertex(clue2);

    auto nearestPoint1 = nearestVertex1->point();
    auto nearestPoint2 = nearestVertex2->point();

    if ((CGAL::squared_distance(clue1, nearestPoint1) > (radius * radius)) ||
        (CGAL::squared_distance(clue2, nearestPoint2) > (radius * radius))) {
      std::cout << "n";
      continue;
    }

    int idx1 = nearestVertex1->info();
    int idx2 = nearestVertex2->info();

    if (component_map[idx1] == component_map[idx2]) {
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