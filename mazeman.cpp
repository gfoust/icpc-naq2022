#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>
#include <array>
#include <cctype>
using namespace std;

using p = tuple<int, int>;

vector<array<char, 100>> maze(100);
int width, height;

bool inbounds(int r, int c) {
  return r >= 0 && r < height&& c >= 0 && c < width;
}

bool fill(int r, int c) {
  bool isdoor = isalpha(maze[r][c]);
  bool dot = maze[r][c] == '.';
  maze[r][c] = 'X';
  
  for (auto [r2, c2] : { p{ r - 1, c }, p{ r + 1, c }, p{ r, c - 1 }, p{ r, c + 1 } }) {
    if (inbounds(r2, c2) && (isdoor ? !isalpha(maze[r2][c2]) : maze[r2][c2] != 'X')) {
      if (fill(r2, c2)) {
        dot = true;
      }
    }
  }
  return dot;
}

int main() {
  cin >> height >> width;
  string line;
  getline(cin, line);
  for (int r = 0; r < height; ++r) {
    getline(cin, line);
    copy(begin(line), end(line), begin(maze[r]));
  }

  int doors = 0;
  for (int c = 0; c < width; ++c) {
    if (maze[0][c] != 'X' && fill(0, c)) {
      ++doors;
    }
    if (maze[height - 1][c] != 'X' && fill(height - 1, c)) {
      ++doors;
    }
  }
  for (int r = 0; r < height; ++r) {
    if (maze[r][0] != 'X' && fill(r, 0)) {
      ++doors;
    }
    if (maze[r][width - 1] != 'X' && fill(r, width - 1)) {
      ++doors;
    }
  }

  int dots = 0;
  for (auto& row : maze) {
    dots += count(begin(row), begin(row) + width, '.');
  }

  cout << doors << ' ' << dots << '\n';
}

