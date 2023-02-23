#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
using namespace std;

struct Road {
  int from, to, cost;
};

struct Query {
  int from, to, min_cost, other_cities;
};

struct Node {
  int parent, size, height;
  set<int> queries;
};

struct Completion {
  int query, node;
};

vector<Road> roads;
vector<Node> nodes;
vector<Query> queries;
vector<Completion> completions;

int root(int i) {
  int p = nodes[i].parent;
  if (i != p) {
    nodes[i].parent = p = root(p);
  }
  return p;
}

void combine(int i, int j) {
  int ir = root(i);
  int jr = root(j);

  if (ir == jr) return;

  if (nodes[ir].height < nodes[jr].height) {
    swap(i, j);
    swap(ir, jr);
  }

  Node& iroot = nodes[ir];
  Node& jroot = nodes[jr];

  jroot.parent = ir;
  iroot.size += jroot.size;
  if (iroot.height == jroot.height) {
    ++iroot.height;
  }

  if (iroot.queries.size() < jroot.queries.size()) {
    swap(iroot.queries, jroot.queries);
  }

  for (int q : jroot.queries) {
    auto p = iroot.queries.lower_bound(q);
    if (p != iroot.queries.end() && *p == q) {
      iroot.queries.erase(p);
      completions.push_back({ q, ir });
    }
    else {
      iroot.queries.insert(p, q);
    }
  }
}

void search() {
  for (int i = 0; i < roads.size();) {
    int cost = roads[i].cost;
    do {
      combine(roads[i].from, roads[i].to);
      ++i;
    } while (i < roads.size() && roads[i].cost == cost);

    for (auto completion : completions) {
      auto& query = queries[completion.query];
      query.min_cost = cost;
      query.other_cities = nodes[root(completion.node)].size;
    }
    completions.clear();
  }
}

int main() {
  int numCities, numRoads, numQueries;
  cin >> numCities >> numRoads >> numQueries;

  nodes.resize(numCities + 1);
  for (int i = 0; i <= numCities; ++i) {
    nodes[i] = { i, 1, 1, {} };
  }

  roads.resize(numRoads);
  for (auto& road : roads) {
    cin >> road.from >> road.to >> road.cost;
  }

  queries.resize(numQueries);
  completions.reserve(numQueries);
  for (int i = 0; i < numQueries; ++i) {
    auto& query = queries[i];
    cin >> query.from >> query.to;
    nodes[query.from].queries.insert(i);
    nodes[query.to].queries.insert(i);
  }

  sort(begin(roads), end(roads), [](Road& a, Road& b) { return a.cost < b.cost; });

  search();

  for (auto& query : queries) {
    cout << query.min_cost << ' ' << query.other_cities << '\n';
  }
}