#include <bits/stdc++.h>
#include <chrono>
#include <thread>

using namespace std;

#define int long long

void display(vector<int> &vx, int idx1, int idx2, int circleCount,
             const string &message) {

  // move cursor to top-left
  cout << "\033[H";

  // clear remaining screen
  cout << "\033[J";

  cout << "Simulation:\n\n";

  for (int i = 0; i < vx.size(); i++) {

    if (vx[i] == -1) {
      cout << " XX  ";
    }

    else if (i == idx1 || i == idx2) {
      cout << "[" << setw(2) << vx[i] << "]";
    }

    else {
      cout << " " << setw(2) << vx[i] << " ";
    }
  }

  cout << "\n\n";

  cout << message << '\n';

  cout << "Circle Count: " << circleCount << '\n';

  cout.flush();
}

void solve() {

  srand(time(0));

  int n = 50;

  int circleCount = 0;

  vector<int> vx(2 * n);

  for (int i = 0; i < 2 * n; i++) {
    vx[i] = i;
  }

  int alive = 2 * n;

  // clear screen once
  cout << "\033[2J";

  while (alive > 1) {

    vector<int> aliveIdx;

    for (int i = 0; i < vx.size(); i++) {

      if (vx[i] != -1)
        aliveIdx.push_back(i);
    }

    int rx1 = rand() % aliveIdx.size();

    int rx2 = rand() % (aliveIdx.size() - 1);

    if (rx2 >= rx1)
      rx2++;

    int idx1 = aliveIdx[rx1];
    int idx2 = aliveIdx[rx2];

    int num1 = vx[idx1];
    int num2 = vx[idx2];

    // show picked cells
    display(vx, idx1, idx2, circleCount,
            "Picked: " + to_string(num1) + " and " + to_string(num2));

    this_thread::sleep_for(chrono::seconds(1));

    // apply rules
    if (abs(num1 - num2) == 1) {

      circleCount++;

      vx[idx1] = -1;
      vx[idx2] = -1;

      alive -= 2;

      display(vx, -1, -1, circleCount, "Adjacent -> deleted BOTH");
    } else {

      // delete MAX value
      if (num1 > num2)
        vx[idx1] = -1;
      else
        vx[idx2] = -1;

      alive--;

      display(vx, -1, -1, circleCount, "Not adjacent -> deleted MAX");
    }

    this_thread::sleep_for(chrono::seconds(1));
  }

  cout << "\nFinal Circle Count: " << circleCount << '\n';
}

int32_t main() {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}

/*
  https://www.youtube.com/shorts/ZHXt0-_gSj4
  C++ simulation + live terminal visualization of the classic 3Blue1Brown
string-loop puzzle.

Problem:
Given `n` strings with 2 loose ends each, repeatedly pick 2 random loose ends
and tie them together until no ends remain.

* If both ends belong to the same chain → a loop is formed
* Otherwise → chains merge

This simulation visualizes the entire stochastic process live in the terminal
while counting the total loops formed.

 */
