#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

int main() {
  string a, b;
  cin >> a >> b;
  string c(a.size() + b.size(), ' ');

  merge(begin(a), end(a), begin(b), end(b), begin(c));

  cout << c << '\n';
}
