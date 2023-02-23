#include <algorithm>
#include <cstdint>
#include <iostream>
using namespace std;

using Int = int64_t;

Int gcf(Int a, Int b) {
  if (a % b == 0) {
    return b;
  }
  else {
    return gcf(b, a % b);
  }
}

int main() {
  Int taxi, spidey;
  cin >> taxi >> spidey;

  Int t = 2 * taxi;
  Int s = 2 * spidey;
  Int ds = 1;
  Int m = spidey * 2 / 3;
  Int n = 0;
  Int d = 0;

  // single quadrant
  Int i = 0;
  while (s > 0) {
    d += s / 2;
    n += max(Int{ 0 }, min(t / 2, s / 2));
    if (i == m) {
      if (spidey % 3) {
        s -= 2;
      }
      else {
        s -= 4;
      }
      ds = 4;
    }
    else {
      s -= ds;
    }
    t -= 2;
    ++i;
  }

  // all four quadrants
  n *= 4;
  d *= 4;

  // origin
  ++n;
  ++d;

  // reduce
  Int f = gcf(n, d);
  n /= f;
  d /= f;

  cout << n;
  if (d != 1) cout << '/' << d;
  cout << '\n';
}
