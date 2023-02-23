#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> ns;

int calc(int a, int b, int op) {
  switch (op) {
  case 0: return a + b;
  case 1: return a - b;
  case 2: return a * b;
  case 3: return a % b ? 1'000'000 : a / b;
  }
}

int main() {
  int a, b, c;
  int z = 1'000'000;

  cin >> a >> b >> c;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      int y = calc(calc(a, b, i), c, j);
      if (y >= 0)
        z = min(z, y);
    }
  }
  cout << z << '\n';
}