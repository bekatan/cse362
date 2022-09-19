//
// bfs.cpp - Version 1.0
//
// Please write your name, your student ID, and your email address here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.
// Name: Bekatan Satyev
// Student ID: 20182024
// Email: bekatans@unist.ac.kr

#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
#include <queue>
#include <set>
#include <stack>

// ---------------------------------------------------------------------
// Include the optional header with backward compatibility
// see http://codereview.stackexchange.com/questions/136350/seamlessly-migrating-experimental-optional-to-optional
#if __has_include(<optional>)
#   include <optional>
namespace stdx {
  using namespace ::std;
}
#elif __has_include(<experimental/optional>)
#   include <experimental/optional>
namespace stdx {
  using namespace ::std;
  using namespace ::std::experimental;
}
#else
#   error <experimental/optional> and <optional> not found
#endif

using namespace stdx;
// ---------------------------------------------------------------------


#define MAX_SIZE 300


class Coordinate {

  int x, y;

public:
  Coordinate(int x, int y) : x(x), y(y) {}

  int getX() const {
    return x;
  }

  int getY() const {
    return y;
  }

  bool operator==(const Coordinate &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  bool operator!=(const Coordinate &rhs) const {
    return !(rhs == *this);
  }

  bool operator<(const Coordinate &rhs) const {
    string lhsString = to_string(x) + " " + to_string(y);
    string rhsString = to_string(rhs.x) + " " + to_string(rhs.y);
    return lhsString < rhsString;
  }
};

struct Node
  {
    Coordinate *coordinate;
    Node *parent;
    Node *children[4];
    Node(Node* parent, Coordinate* coordinate){
      this->parent = parent;
      this->coordinate = coordinate;
    }
    ~Node(){
      delete coordinate;
      for (Node* n: children){
        delete n;
      }
    }
  };

class BreadthFirstSearchAgent {
  
  Node *root;
  Node *current;
  queue<Node*> q;
  set<Coordinate> visited;

public:

  BreadthFirstSearchAgent(int size_x, int size_y) {
    root = new Node(NULL, new Coordinate(0,0));
    current = root;
    q.push(current);
    visited.insert(Coordinate(0,0));
  }

  ~BreadthFirstSearchAgent() {
    delete root;
  }

  optional<Coordinate> move(bool isExit, bool hasWallSouth, bool hasWallNorth, bool hasWallEast, bool hasWallWest) {
    if (isExit){
      return {};
    }
    int x = current->coordinate->getX(), y = current->coordinate->getY(); 
    
    Node *n;
    if(!hasWallSouth && visited.find(Coordinate(x,y+1)) == visited.end()){
      n = new Node(current, new Coordinate(x,y+1));
      current->children[0] = n;
      q.push(n);
      visited.insert(Coordinate(x,y+1));
    }

    if(!hasWallNorth && visited.find(Coordinate(x,y-1)) == visited.end()){
      n = new Node(current, new Coordinate(x,y-1));
      current->children[1] = n;
      q.push(n);
      visited.insert(Coordinate(x,y-1));
    }
    if(!hasWallEast && visited.find(Coordinate(x+1,y)) == visited.end()){
      n = new Node(current, new Coordinate(x+1,y));
      current->children[2] = n;
      q.push(n);
      visited.insert(Coordinate(x+1,y));
    }
    if(!hasWallWest && visited.find(Coordinate(x-1,y)) == visited.end()){
      n = new Node(current, new Coordinate(x-1,y));
      current->children[3] = n;
      q.push(n);
      visited.insert(Coordinate(x-1,y));
    }
    
    q.pop();
    current = q.front();
    return *current->coordinate;
  }

  list<Coordinate> getShortestPath() {
    list <Coordinate> path;
    stack <Coordinate> backTrack;
    while (current != NULL){
      backTrack.push(*current->coordinate);
      current = current->parent;
    }
    while(!backTrack.empty()){
      path.push_back(backTrack.top());
      backTrack.pop();
    }
    return path;
  }
};


int main(int argc, char *argv[]) {

  int size_x, size_y;

  if (argc == 3) {
    size_x = atoi(argv[1]);
    size_y = atoi(argv[2]);
  } else {
    cerr << "Error: wrong arguments." << endl;
    return -1;  // do nothing
  }

  BreadthFirstSearchAgent agent(size_x, size_y);

  while(true) {
    string s1, s2, s3, s4, s5;
    cin >> s1 >> s2 >> s3 >> s4 >> s5;

    bool isExit = (s1 != "0");
    bool hasWallSouth = (s2 != "0");
    bool hasWallNorth = (s3 != "0");
    bool hasWallEast = (s4 != "0");
    bool hasWallWest = (s5 != "0");

    auto coord = agent.move(isExit, hasWallSouth, hasWallNorth, hasWallEast, hasWallWest);

    if (coord) {
      cout << coord->getX() << " " << coord->getY() << endl;
    } else {
      break;
    }
  }

  list<Coordinate> path = agent.getShortestPath();

  cout << "PATH" << endl;
  for(auto&& coord : path) {
    cout << coord.getX() << " " << coord.getY() << endl;
  }
  cout << "END" << endl;

  return 0;
}
