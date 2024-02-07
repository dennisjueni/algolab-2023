/// 1
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/enum.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef EK::Point_2 P;
typedef EK::Triangle_2 T;
typedef EK::Line_2 L;

bool isInside(P c1, P c2, P c3, P p) {
  auto or0 = CGAL::orientation(c1, c2, p);
  auto or1 = CGAL::orientation(c2, c3, p);
  auto or2 = CGAL::orientation(c3, c1, p);
  return (or0 <= 0 && or1 <= 0 && or2 <= 0) ||
         (or0 >= 0 && or1 >= 0 && or2 >= 0);
}

void testcase() {
  int m, n;
  std::cin >> m >> n;
  int numLegs = m - 1;

  std::vector<P> points(m);

  // Read in points
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    points[i] = P(x, y);
  }

  std::vector<std::vector<int>> triangCoversLeg(n, std::vector<int>(0));

  for (int i = 0; i < n; i++) {
    std::vector<P> triangPoints(6);
    for (int j = 0; j < 6; j++) {
      int x, y;
      std::cin >> x >> y;
      triangPoints[j] = P(x, y);
    }

    L line1(triangPoints[0], triangPoints[1]);
    L line2(triangPoints[2], triangPoints[3]);
    L line3(triangPoints[4], triangPoints[5]);

    auto obj0 = CGAL::intersection(line1, line2);
    auto obj1 = CGAL::intersection(line2, line3);
    auto obj2 = CGAL::intersection(line3, line1);

    // Since we know the intersection exists, we can do this
    const P *corner0 = boost::get<P>(&*obj0);
    const P *corner1 = boost::get<P>(&*obj1);
    const P *corner2 = boost::get<P>(&*obj2);

    bool prevCovered = isInside(*corner0, *corner1, *corner2, points[0]);

    for (int j = 1; j < m; j++) {
      bool currCovered = isInside(*corner0, *corner1, *corner2, points[j]);

      // Check if the leg between two points is thus covered
      if (prevCovered && currCovered) {
        triangCoversLeg[i].push_back(j - 1);
      }
      prevCovered = currCovered;
    }
  }

  // Left is inclusive, right is exclusive
  int left = 0, right = 1;
  std::vector<int> coveredCount(numLegs, 0);
  int minCost = n;

  for (int leg : triangCoversLeg[left]) {
    coveredCount[leg]++;
  }

  while (true) {
    // First we check if still all legs are covered
    bool allCovered = true;
    for (int i = 0; i < numLegs; i++) {
      if (coveredCount[i] == 0) {
        allCovered = false;
      }
    }

    if (allCovered) {
      minCost = std::min(minCost, right - left);
      if (left == n - 1) {
        break;
      } else {
        for (int leg : triangCoversLeg[left]) {
          coveredCount[leg]--;
        }
        left++;
      }
    } else {
      if (right == n) {
        break;
      } else {
        for (int leg : triangCoversLeg[right]) {
          coveredCount[leg]++;
        }
        right++;
      }
    }
  }
  std::cout << minCost << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int c;
  std::cin >> c;

  for (int i = 0; i < c; i++) {
    testcase();
  }
  return 0;
}