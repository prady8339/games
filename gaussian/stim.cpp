#include <bits/stdc++.h>
#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;
// coding was so fun before Ai :(
// https://www.youtube.com/shorts/TwctT3Ncm1w
#define int long long

void displayMat(vector<vector<int>> &mat) {

  cout << "\033[H";

  for (auto &x : mat) {

    for (auto &y : x) {

      if (y == 0)
        cout << setw(6) << ".";
      else
        cout << setw(6) << y;
    }

    cout << '\n';
  }

  cout.flush();

  // this_thread::sleep_for(chrono::milliseconds(150));
  this_thread::sleep_for(chrono::seconds(1));
}

void solve() {

  srand(time(0));

  int n = 14;
  int m = 25;

  vector<vector<int>> mat(n, vector<int>(m, 0));

  int balls = 1e5;

  mat[0][m / 2] = balls;

  displayMat(mat);

  for (int i = 0; i < n - 1; i++) {

    for (int j = 0; j < m; j++) {

      for (int k = 0; k < mat[i][j]; k++) {

        int nextJ = (rand() % 3) - 1;

        if (nextJ + j < 0 || nextJ + j >= m)
          nextJ = 0;

        mat[i + 1][j + nextJ]++;
      }

      mat[i][j] = 0;
    }

    displayMat(mat);
  }
}

int32_t main() {

  solve();

  return 0;
}
