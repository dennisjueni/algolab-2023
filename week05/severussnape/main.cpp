#include <algorithm>
#include <iostream>
#include <vector>

struct PotionA {
  int p;
  int h;
};

struct PotionB {
  int w;
};

void testcase() {
  int n, m;
  std::cin >> n >> m;

  int a, b;
  std::cin >> a >> b;

  long P, H, W;
  std::cin >> P >> H >> W;

  std::vector<PotionA> potionsA(n);
  for (int i = 0; i < n; i++) {
    int p, h;
    std::cin >> p >> h;
    potionsA[i] = {p, h};
  }

  std::vector<PotionB> potionsB(m);
  for (int i = 0; i < m; i++) {
    int w;
    std::cin >> w;
    potionsB[i] = {w};
  }

  // DP dimension = (n+1)*(n+1)*(H+1)
  // DP[i][j][h] = max power we can get using exactly j potions out of the first
  // i potions while achieving at least h happiness
  std::vector<std::vector<std::vector<long>>> dp(
      n + 1,
      std::vector<std::vector<long>>(
          n + 1, std::vector<long>(H + 1, std::numeric_limits<long>::min())));

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= i; j++) {
      for (int h = 0; h <= H; h++) {

        if (j == 1) {
          // Base case
          if (potionsA[i - 1].h >= h) {
            dp[i][j][h] = std::max(dp[i - 1][j][h], (long)potionsA[i - 1].p);
          } else {
            dp[i][j][h] = dp[i - 1][j][h];
          }
          continue;
        }

        // If we take the current potion (potionsA[i-1]!), we still need
        // "neededH" happiness from other potions
        int neededH = std::max(0, h - potionsA[i - 1].h);
        dp[i][j][h] = std::max(dp[i - 1][j][h],
                               dp[i - 1][j - 1][neededH] + potionsA[i - 1].p);
      }
    }
  }

  std::sort(potionsB.begin(), potionsB.end(),
            [](PotionB p1, PotionB p2) { return p1.w >= p2.w; });

  long totalWit = 0;
  long minPotions = std::numeric_limits<long>::max();

  for (int w = 1; w <= m; w++) {
    totalWit += potionsB[w - 1].w;
    if (totalWit < W) {
      // In this case we will never be able to fulfill the W property since
      // adding potions of type A, can only decrease the total wit.
      continue;
    }

    // BE CAREFUL WITH THIS! IF w and b are ints, they can overflow!!
    long neededPower = P + (long)w * b;
    for (int i = 1; i <= n; i++) {
      long neededWit = W + (long)i * a;
      if (dp[n][i][H] >= neededPower && totalWit >= neededWit) {
        minPotions = std::min(minPotions, (long)w + i);
      }
    }
  }

  minPotions =
      (minPotions == std::numeric_limits<long>::max()) ? -1 : minPotions;
  std::cout << minPotions << "\n";
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