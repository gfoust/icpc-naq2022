#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>

const int64_t B = 1'000'000'007;

struct Mod {
  std::int64_t value;
  Mod() : value{ 0 } {}
  Mod(int64_t value) : value{ (value % B + B) % B } {}
};

Mod operator +(Mod a, Mod b) {
  return a.value + b.value;
}

Mod operator *(Mod a, Mod b) {
  return a.value * b.value;
}

Mod operator -(Mod a, Mod b) {
  return a.value - b.value;
}


enum class State { Uninitialized, Exploring, Sorted };
const int NO_CYCLE = -1;


struct Node {
  State state = State::Uninitialized;
  Mod scenarios = { 1 };
  int cycle = -1;
  int outgoing = -1;
};

std::vector<Node> nodes;
std::vector<int> order;
Mod total_count = 1;

int toposort(size_t i) {
  Node& node = nodes[i];
  if (node.state == State::Sorted) {
    return NO_CYCLE;
  }

  node.state = State::Exploring;
  if (nodes[node.outgoing].state == State::Exploring) {
    node.cycle = node.outgoing;
  }
  else {
    node.cycle = toposort(node.outgoing);
  }
  node.state = State::Sorted;

  if (node.cycle == NO_CYCLE || node.cycle == i) {
    order.push_back(i);
    return NO_CYCLE;
  }
  else {
    return node.cycle;
  }
}

void visit(size_t i) {
  Node& node = nodes[i];
  node.scenarios = node.scenarios + 1;

  if (node.cycle == NO_CYCLE) {
    Node* next = &nodes[node.outgoing];
    if (next->cycle != NO_CYCLE) {
      next = &nodes[next->cycle];
    }

    next->scenarios = next->scenarios * node.scenarios;
  }
  else {
    total_count = total_count * node.scenarios;
  }
}

int main() {
  using std::cin;
  using std::cout;
  using std::endl;

  int n;
  cin >> n;
  nodes.resize(n);
  for (auto& node : nodes) {
    cin >> node.outgoing;
    --node.outgoing;
  }

  for (int i = 0; i < n; ++i) {
    toposort(i);
  }

  for (int i = order.size() - 1; i >= 0; --i) {
    visit(order[i]);
  }

  total_count = total_count - 1;
  cout << total_count.value << endl;
}
