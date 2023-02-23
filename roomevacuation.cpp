#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
#include <numeric>
#include <stdexcept>

using namespace std;

struct Location {
  int row, col;
};

enum Direction { None, Up, Down, Left, Right, Stay, Occupy, Start };

Direction opposite(Direction dir) {
  switch (dir) {
  case Up:    return Down;
  case Down:  return Up;
  case Left:  return Right;
  case Right: return Left;
  }
  return Stay;
}

Location moveInDir(Location loc, Direction dir) {
  switch (dir) {
  case Up:    return { loc.row - 1, loc.col };
  case Down:  return { loc.row + 1, loc.col };
  case Left:  return { loc.row, loc.col - 1 };
  case Right: return { loc.row, loc.col + 1 };
  }
  return loc;
}




int width = 0, height = 0;

bool inBounds(Location loc) {
  return loc.row >= 0 && loc.row < height&& loc.col >= 0 && loc.col < width;
}

template <typename T>
struct Grid {
  vector<T> els = vector<T>(width * height);

  void resize() {
    els.resize(width * height);
  }

  auto operator[](size_t i) {
    return els.begin() + i * width;
  }

  T& operator[](Location loc) {
    return els[loc.row * width + loc.col];
  }
};




struct Flow {
  uint8_t up = 0, down = 0, left = 0, right = 0, stay = 0;
};

struct Node {
  Direction dir = None;
  int time;
  Location loc;
};




vector<Grid<Flow>> movements;
vector<Grid<int>> occupies;

int getMovement(int time, Location loc, Direction dir) {
  auto& m = movements[time / 2];
  switch (dir) {
  case Up:    return m[loc].up;
  case Down:  return m[loc].down;
  case Left:  return m[loc].left;
  case Right: return m[loc].right;
  case Stay:  return m[loc].stay;
  }
  throw std::logic_error("whoops");
}

int incMovement(int time, Location loc, Direction dir, uint8_t val) {
  auto& m = movements[time / 2];
  switch (dir) {
  case Up:    return m[loc].up += val;
  case Down:  return m[loc].down += val;
  case Left:  return m[loc].left += val;
  case Right: return m[loc].right += val;
  case Stay:  return m[loc].stay += val;
  }
  throw std::logic_error("whoops");
}

int getOccupy(int time, Location loc) {
  auto& s = occupies[time / 2];
  return s[loc];
}

void incOccupy(int time, Location loc, uint8_t val) {
  auto& s = occupies[time / 2];
  s[loc] += val;
}



int maxTime;
Grid<char> floorPlan;
int escaped = 0;

const int STAY = 0;
const int MOVE = 1;

// debug only
void print() {
  cerr << "------------------------------------------------------------\n";
  for (int i = 0; i < occupies.size(); ++i) {
    if (all_of(occupies[i].els.begin(), occupies[i].els.end(), [](int i) { return i == 0; })) {
      break;
    }
    cerr << i << '\n';
    for (int r = 0; r < height; ++r) {
      for (int c = 0; c < width; ++c) {
        if (floorPlan[r][c] == 'E') {
          cerr << 'E';
        }
        else if (occupies[i][r][c]) {
          cerr << 'P';
        }
        else if (floorPlan[r][c] == '#') {
          cerr << '#';
        }
        else {
          cerr << '.';
        }
      }
      cerr << '\n';
    }
    cerr << '\n';
  }
}

struct PathFinder {
  queue<Node> discovered;
  vector<Grid<Node>> parents = vector<Grid<Node>>(maxTime * 2 + 2);
  bool foundGoal = false;
  Node goal;

  void discover(Node node, Node parent) {
    Node& record = parents[node.time][node.loc];
    if (record.dir == None) {
      record = parent;
      if (node.time % 2 == MOVE && floorPlan[node.loc] == 'E') {
        goal = node;
        foundGoal = true;
      }
      else {
        discovered.push(node);
      }
    }
  }

  bool findPath() {
    for (int r = 0; r < height; ++r) {
      for (int c = 0; c < width; ++c) {
        if (floorPlan[r][c] == 'P') {
          Node node = { Start, 0, { r, c } };
          discovered.push(node);
        }
      }
    }

    while (!foundGoal && !discovered.empty()) {
      Node node = discovered.front();
      discovered.pop();

      if (node.time % 2 == MOVE) {
        // move forward
        if (node.time < maxTime * 2) {
          for (auto dir : { Up, Down, Left, Right, Stay }) {
            auto dest = moveInDir(node.loc, dir);
            if (inBounds(dest) && floorPlan[dest] != '#') {
              discover({ dir, node.time + 1, dest }, node);
            }
          }
        }

        // undo stay
        if (getOccupy(node.time - 1, node.loc) == 1) {
          discover({ Occupy, node.time - 1, node.loc }, node);
        }
      }
      else {
        // stay forward
        if (getOccupy(node.time, node.loc) < 1) {
          discover({ Occupy, node.time + 1, node.loc }, node);
        }

        // undo move
        if (node.time > 0) {
          for (auto dir : { Up, Down, Left, Right, Stay }) {
            auto dest = moveInDir(node.loc, opposite(dir));
            if (inBounds(dest) && getMovement(node.time - 1, dest, dir) == 1) {
              discover({ dir, node.time - 1, dest }, node);
            }
          }
        }
      }
    }
    return foundGoal;
  }

  vector<Node> buildPath(Node node, int size) {
    if (node.dir == Start) {
      vector<Node> path;
      path.reserve(size);
      path.push_back(node);
      return path;
    }
    else {
      auto path = buildPath(parents[node.time][node.loc], size + 1);
      path.push_back(node);
      return path;
    }
  }

  vector<Node> buildPath() {
    return buildPath(goal, 1);
  }

};




void addPath(vector<Node> path) {
  ++escaped;
  int time = 0;

  for (int i = 0; i < path.size() - 1; ++i) {
    auto& node = path[i];
    auto& next = path[i + 1];
    if (node.time % 2 == MOVE) {
      if (next.time > node.time) {
        incMovement(node.time, node.loc, next.dir, 1);
      }
      else {
        incOccupy(node.time, node.loc, -1);
      }
    }
    else {
      if (next.time > node.time) {
        incOccupy(node.time, node.loc, 1);
      }
      else {
        incMovement(node.time - 1, node.loc, opposite(next.dir), -1);
      }
    }
  }
}

void calculateFlow() {
  bool pathFound;
  do {
    PathFinder finder;
    pathFound = finder.findPath();
    if (pathFound) {
      auto path = finder.buildPath();
      addPath(move(path));
    }
    //print();
    //cerr << escaped << '\n';
  } while (pathFound);
}

int main() {
  cin >> height >> width >> maxTime;
  movements.resize(maxTime + 1);
  occupies.resize(maxTime + 1);
  floorPlan.resize();
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      cin >> floorPlan[r][c];
    }
  }
  calculateFlow();
  cout << escaped << '\n';
}