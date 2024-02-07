#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <iostream>
#include <map>
#include <vector>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Mine {
  int dangerThreshold;
  std::vector<int> mineralAvlbl;
};

int numMines, numMinerals;
Program lp;
std::vector<Mine> mines;
std::vector<int> dangerousMap;
std::vector<std::vector<int>> adjList;

int getMineVar(int mineIdx, int mineralIdx) {
  if (dangerousMap[mineIdx] == -1) {
    std::cout << "DANGER!\n";
    return 0;
  }

  // The first numMinerals variables are reserved for how much we need to buy
  // per mineral
  return numMinerals + numMinerals * dangerousMap[mineIdx] + mineralIdx;
}

void addCapacityConstraints(int root, int current, int baseConstraint) {

  if (dangerousMap[current] != -1 && current != root) {
    for (int i = 0; i < numMinerals; i++) {
      lp.set_a(getMineVar(current, i), baseConstraint + i, -1);
    }
    return;
  }

  if (root != current) {
    for (int i = 0; i < numMinerals; i++) {
      mines[root].mineralAvlbl[i] += mines[current].mineralAvlbl[i];
    }
  }

  for (int child : adjList[current]) {
    addCapacityConstraints(root, child, baseConstraint);
  }

  if (root == current) {
    // Finish the LP constraint
    for (int i = 0; i < numMinerals; i++) {
      lp.set_a(getMineVar(root, i), baseConstraint + i, 2);

      lp.set_b(baseConstraint + i, 2 * mines[root].mineralAvlbl[i]);
    }
  }
}

void testcase() {

  std::cin >> numMines >> numMinerals;

  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  lp = Program(CGAL::SMALLER, true, 0, false, 0);

  mines = std::vector<Mine>(numMines);

  dangerousMap = std::vector<int>(numMines, -1);
  dangerousMap[0] = 0;

  adjList = std::vector<std::vector<int>>(numMines, std::vector<int>());
  // The entry has idx 0, so we start with 1
  int currConstraint = 1;

  for (int i = 0; i < numMines; i++) {
    int dangerThreshold;
    std::cin >> dangerThreshold;

    std::vector<int> minerals = std::vector<int>(numMinerals);
    for (int j = 0; j < numMinerals; j++) {
      std::cin >> minerals[j];
    }

    if (dangerThreshold >= 0) {
      dangerousMap[i] = currConstraint;

      for (int j = 0; j < numMinerals; j++) {
        lp.set_a(getMineVar(i, j), currConstraint, 1);
      }

      lp.set_b(currConstraint, dangerThreshold);
      currConstraint++;
    }

    mines[i] = {dangerThreshold, minerals};
  }

  for (int i = 0; i < numMines - 1; i++) {
    int u, v;
    std::cin >> u >> v;

    adjList[v].push_back(u);
  }

  for (int i = 0; i < numMines; i++) {
    if (dangerousMap[i] != -1) {
      addCapacityConstraints(i, i, currConstraint);
      // In addCapacityConstraints we add 1 constraint per mineral
      currConstraint += numMinerals;
    }
  }

  for (int i = 0; i < numMinerals; i++) {
    int required, supply, price;
    std::cin >> required >> supply >> price;

    // cannot buy more than is available
    lp.set_a(i, currConstraint, 1);
    lp.set_b(currConstraint, supply);
    currConstraint++;

    // need at least required
    lp.set_a(i, currConstraint, -1);
    lp.set_a(getMineVar(0, i), currConstraint, -1);
    lp.set_b(currConstraint, -1 * required);
    currConstraint++;

    lp.set_c(i, price);
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible()) {
    std::cout << "Impossible!\n";
    return;
  }

  std::cout << (long)CGAL::to_double(s.objective_value()) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
}