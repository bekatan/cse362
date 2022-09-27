//
// bfs.cpp - Version 1.0
//
// Please write your name, your student ID, and your email address here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.
// Name: Bekatan Satyev
// Student ID: 20182024
// Email: bekatans@unist.ac.kr

/***
 * Breadth First Search
 *   - The problem of finding the Exit in a maze is converted to traversing a tree with a breadth first search approach. 
 *   In this tree each node represents a point in the maze with properties such as Coordinate, pointers to the parent Node
 *   and a container with pointers to children Nodes. The BreadFirstSearchAgent has properties such as pointer to the root 
 *   its logarithmic insertion and access time. In order to implement iestthe orderet set of Coordinates, operator<(...) is 
 *   overridden in Coordinates implementation.
*/

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
  //for the ordered set of coordinates visited
  bool operator<(const Coordinate &rhs) const {
    string lhsString = to_string(x) + " " + to_string(y);
    string rhsString = to_string(rhs.x) + " " + to_string(rhs.y);
    return lhsString < rhsString;
  }
};

struct Node {
  Node *parent;
  Coordinate *coordinate;
  list<Node*> children;
  Node(Node* parent, Coordinate* coordinate): parent(parent), coordinate(coordinate) {}
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
  queue<Node*> fringe;
  set<Coordinate> visited;

public:

  BreadthFirstSearchAgent(int size_x, int size_y) {
    root = new Node(NULL, new Coordinate(0,0));
    current = root;
    fringe.push(current);
    visited.insert(Coordinate(0,0));
  }

  ~BreadthFirstSearchAgent() {
    delete root;
  }
  //method to add new nodes into the fringe
  void add(bool hasWall, Coordinate coordinate){
    if (hasWall){
      return;
    }
    //if visited, do not create a node
    if (visited.find(coordinate) != visited.end()){
      return;
    }

    Node* n = new Node(current, new Coordinate(coordinate.getX(), 
                        coordinate.getY()));
    current->children.push_back(n);
    fringe.push(n);
    visited.insert(coordinate);
  }
  /***
   * In move(...) all new coordinates where the path can go are first checked if they are in the visited set, if not then 
   * added to the array of current's children nodes (so we can delete them later), added to the queue of fringe nodes and 
   * set of visited places. The FIFO policy of queues allows us to implement bfs, since all new child nodes are the first 
   * ones to be accessed. When isExit is 1, we can be sure that the first path we found is the shortest one, because of bfs 
   * approach.
  */
  optional<Coordinate> move(bool isExit, bool hasWallSouth, bool hasWallNorth, bool hasWallEast, bool hasWallWest) {
    if (isExit){
      return {};
    }
    int x = current->coordinate->getX(), y = current->coordinate->getY(); 
    
    add(hasWallSouth, Coordinate(x, y + 1));
    add(hasWallNorth, Coordinate(x, y - 1));
    add(hasWallEast, Coordinate(x + 1, y));
    add(hasWallWest, Coordinate(x - 1, y));

    fringe.pop();
    current = fringe.front();
    return *current->coordinate;
  }
  /***
   * - In getShortestPath(...) since the current points to the exit we can trace back the shortest path to the root by 
   * continuously referencing the parent pointer of the current.
  */
  list<Coordinate> getShortestPath() {
    list <Coordinate> path;
    while (current != NULL){
      path.push_front(*current->coordinate);
      current = current->parent;
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
