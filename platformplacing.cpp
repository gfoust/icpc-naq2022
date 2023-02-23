#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;


int main() {
  int numPlatforms, maxSize, minSize;
  cin >> numPlatforms >> minSize >> maxSize;

  vector<int> anchors(numPlatforms);

  for (auto& a : anchors) {
    cin >> a;
    a *= 2;
  }

  sort(begin(anchors), end(anchors));

  vector<int> platforms(numPlatforms);

  if (numPlatforms == 1) {
    cout << maxSize << '\n';
    return 0;
  }

  int i = 0;

  platforms.front() = min(maxSize, anchors[i + 1] - anchors[i] - minSize);

  for (++i; i < numPlatforms - 1; ++i) {
    platforms[i] = min({
      maxSize,
      anchors[i + 1] - anchors[i] - minSize,
      anchors[i] - anchors[i - 1] - platforms[i - 1]
    });
  }

  platforms.back() = min(maxSize, anchors[i] - anchors[i - 1] - platforms[i - 1]);
  
  if (any_of(begin(platforms), end(platforms), [=](int n) { return n < minSize; })) {
    cout << -1 << '\n';
  }
  else {
    cout << accumulate(begin(platforms), end(platforms), 0);
  }
}