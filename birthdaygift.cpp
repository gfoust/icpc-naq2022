#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

const int64_t B = 1'000'000'007;

struct Mod {
  int64_t value;
  Mod() : value{ 0 } {}
  Mod(int64_t value) : value{ (value % B + B) % B } {}
};

Mod operator +(Mod a, Mod b) {
  return a.value + b.value;
}

Mod& operator += (Mod& a, Mod b) {
  a.value = (a + b).value;
  return a;
}

Mod operator -(Mod a, Mod b) {
  return a.value - b.value;
}

Mod operator *(Mod a, Mod b) {
  return a.value * b.value;
}

using Table = vector<array<array<Mod, 9>, 10>>;

Table tableOne() {
  Table table(10);
  table[0][0][0] = 1;
  table[1][1][1] = 1;
  table[2][2][2] = 1;
  table[3][3][3] = 1;
  table[4][4][4] = 1;
  table[5][5][5] = 1;
  table[6][6][6] = 1;
  table[7][7][7] = 1;
  table[8][8][8] = 1;
  table[9][9][0] = 1;
  return table;
}

Table operator *(Table const& a, Table const& b) {
  Table c(10);
  for (int f1 = 0; f1 < 10; ++f1) {
    for (int b1 = 0; b1 < 10; ++b1) {
      for (int m1 = 0; m1 < 9; ++m1) {
        for (int f2 = 0; f2 < 10; ++f2) {
          if (f2 != b1) {
            for (int b2 = 0; b2 < 10; ++b2) {
              for (int m2 = 0; m2 < 9; ++m2) {
                c[f1][b2][(m1 + m2) % 9] += a[f1][b1][m1] * b[f2][b2][m2];
              }
            }
          }
        }
      }
    }
  }

  return c;
}

// 1000000000000000000 100

Table pow(Table x, int64_t e) {
  Table p = x;
  int64_t bit = int64_t{ 1 } << 62;
  while (bit && (bit & e) == 0) bit >>= 1;
  bit >>= 1;
  while (bit) {
    p = p * p;
    if (bit & e) {
      p = p * x;
    }
    bit >>= 1;
  }
  return p;
}

int main() {
  int64_t n, m;
  cin >> n >> m;

  Table table;
  Mod total = 0;
  if (n > 2) {
    table = pow(tableOne(), n - 2);

    int m25 = m % 25;
    int m9 = m % 9;
    for (int i = 0; i < 4; ++i) {

      int d1 = m25 / 10;
      int d2 = m25 % 10;
      if (d1 != d2) {

        int goal = ((m9 - m25) % 9 + 9) % 9;
        for (int f = 1; f < 10; ++f) {
          for (int b = 0; b < 10; ++b) {
            if (b != d1) {
              total += table[f][b][goal];
            }
          }
        }
      }

      m25 += 25;
    }
  }
  else if (n == 2 && m < 100) {
    int d1 = m / 10;
    int d2 = m % 10;
    if (d1 != 0 && d1 != d2) {
      total += 1;
    }
  }
  else if (n == 1 && m < 10) {
    total += 1;
  }

  cout << total.value << '\n';
}
