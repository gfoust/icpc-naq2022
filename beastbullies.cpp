#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
using namespace std;


int main() {
  int64_t size;
  cin >> size;
  vector<int64_t> ns(size);
  for (auto& n : ns) {
    cin >> n;
  }

  sort(rbegin(ns), rend(ns));

  int64_t a = ns[0];
  int64_t d = 0;
  int64_t b = 0;
  for (int64_t i = 1; i < size; ++i) {
    d += ns[i];
    if (d >= a) {
      b = i;
      a += d;
      d = 0;
    }
  }
  
  cout << b + 1 << '\n';
}