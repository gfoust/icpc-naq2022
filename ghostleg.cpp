#include <iostream>
#include <vector>
#include <utility>
using namespace std;

int main() {
  int size, rungs;
  cin >> size >> rungs;

  vector<int> ns(size);
  int i = 1;
  for (auto& n : ns) {
    n = i++;
  }

  for (i = 0; i < rungs; ++i) {
    int a;
    cin >> a;
    swap(ns[a - 1], ns[a]);
  }

  for (auto n : ns) {
    cout << n << '\n';
  }
}