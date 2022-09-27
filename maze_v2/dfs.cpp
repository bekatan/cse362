//
// dfs.cpp - Version 1.0
//
// Please write your name, your student ID, and your email address here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.
// Name: Bekatan Satyev
// Student ID: 20182024
// Email: bekatans@unist.ac.kr
//

/***
 * Depth First Search
    - The problem of finding the Exit in a maze is converted to traversing a tree with a depth first search approach. 
    In this tree each node represents a point in the maze with properties such as Coordinate, pointer to the parent Node, 
    a container with pointers to children Nodes and the number of steps taken to reach this point - traveled. The 
    DepthFirstSearchAgent has properties such as pointers to the root node, current node, the tip of the shortest path, 
    stack of fringe nodes, an array of several depth limits of increasing degrees and an ordered map of visited 
    Coordinates as keys and shortest distance traveled as values. map container is choosen due to its logarithmic 
    insertion and access time and ability to hold the value shortest distance traveled to reach each coordinate. In order 
    to implement the orderet map, operator<(...) is overridden in Coordinates implementation.
*/

#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
#include <queue>
#include <map>
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

struct Node {
  Node *parent;
  Coordinate *coordinate;
  list <Node*> children;
  int traveled;
  Node(Node* parent, Coordinate* coordinate, int traveled): parent(parent), coordinate(coordinate), traveled(traveled) {}
  ~Node(){
    delete coordinate;
    for (Node* n: children){
      delete n;
    }
  }
};


class DepthFirstSearchAgent {

  Node *root;
  Node *current;
  Node *shortest;
  stack<Node*> fringe;
  map<Coordinate,int> visited;

  int limits[10];
  int limitIndex;

public:

  DepthFirstSearchAgent(int size_x, int size_y) {
    root = new Node(NULL, new Coordinate(0,0), 0);
    shortest = NULL;
    current = root;
    fringe.push(current);
    visited[*current->coordinate] = current->traveled;
    
    int sum = size_x + size_y;
    for (int i = 0; i < 9; i++){
      limits[i] = sum/5 * (i+1);
    }
    limits[9] = size_x * size_y;
    limitIndex = 0;
  }

  ~DepthFirstSearchAgent() {
    delete root;
  }

  //method to add new nodes into the fringe
  void add(bool hasWall, Coordinate coordinate){
    if (hasWall){
      return;
    }

    if (visited.find(coordinate) != visited.end() && visited[coordinate] < current->traveled + 1){
      return;
    }

    Node* n = new Node(current, new Coordinate(coordinate.getX(), coordinate.getY()), current->traveled+1);
    current->children.push_back(n);
    fringe.push(n);
    visited[*n->coordinate] = n->traveled;
  }
  /***
   * - In move(...) all new coordinates where the path can go are first checked if they are not in the visited map or 
   * if the current distance traveled is less than previous distance, if so then the node is added to the array of 
   * current's children nodes (so we can delete them later), added to the stack of fringe nodes and map of visited 
   * coordinates with current distance traveled. The FILO policy of stacks allows us to implement dfs, since new child 
   * nodes are accessed before older nodes. When a current limit of depth is reached the new children are not visited. 
   * Instead we make sure that all paths in the fringe are within the limits. Only then the next depth limit is set. 
   * The highest depth limit is the area of the maze, which bounds the longest non-looping path in the maze. When isExit 
   * is 1, we need to make sure that we have the shortest path to it, so we traverse each path to the limit to make find 
   * the shortest path. Node pointer shortest is given to the node with the lowest traveled value.
   * 
  */
  optional<Coordinate> move(bool isExit, bool hasWallSouth, bool hasWallNorth, bool hasWallEast, bool hasWallWest) {
    
    if (isExit){
      if (shortest == NULL || shortest->traveled > current->traveled){
        shortest = current;
      }
    }

    visited[*current->coordinate] = current->traveled;
    
    //if the current depth limit is reached return without adding deeper nodes in this path
    if(current->traveled > limits[limitIndex]){
      fringe.pop();
      
      if (fringe.empty()){
        if (shortest != NULL){
          return {};
        } else {
          limitIndex++;
          fringe.push(root);
        }
      }

      current = fringe.top();

      return *current->coordinate;
    }
    
    int x = current->coordinate->getX(), y = current->coordinate->getY();
    fringe.pop();
    
    //add new nodes only while the current traveled distance is less than the shortest path distance
    if (shortest == NULL || shortest->traveled > current->traveled){
      add(hasWallSouth, Coordinate(x, y + 1));
      add(hasWallNorth, Coordinate(x, y - 1));
      add(hasWallEast, Coordinate(x + 1, y));
      add(hasWallWest, Coordinate(x - 1, y));
    }

    if (fringe.empty()){
      if (shortest != NULL){
        return {};
      } else {
        limitIndex++;
        fringe.push(root);
      }
    } 

    current = fringe.top();

    return *current->coordinate; 
  }
  /***
   * - In getShortestPath(...) since the shortest points to the exit we can trace back the shortest path to the root by 
   * continuously referencing the parent pointer of the shortest.
  */
  list<Coordinate> getShortestPath() {
    
    list <Coordinate> path;
    while (shortest != NULL){
      path.push_front(*shortest->coordinate);
      shortest = shortest->parent;
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

  DepthFirstSearchAgent agent(size_x, size_y);

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

