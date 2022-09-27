//
// astar.cpp - Version 1.0
//
// Please write your name, your student ID, and your email address here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.

// Name: Bekatan Satyev
// Student ID: 20182024
// Email: bekatans@unist.ac.kr

/***
 * A Star First Search
 *  -astar is implemented in a way similar to bfs. We have a queue of nodes, so we have a FIFO policy. However the queue 
 * is ordered depending on the estimatedTotalDistance from the node to the goal. this estimated total distance is the sum 
 * of traveled distance and estimated distance from this nodes coordinates. Because priority queue requires 
 * estimatedTotalDistance to be calculated, child nodes are not pushed to the queue until they are visited. Insted the 
 * coordinates of children of the node with lowest estimatedTotalDistance are returned from move(...). compareTraveled(...) 
 * is utility method that is used in the priority queue to order nodes in the queue. One problem is that some path may not 
 * be optimal even though it has lower estimated distance for some time compared to the actual optimal path, so similar to 
 * the dfs implementation for each new node to be added it's coordinates are checked to be absent in the visited list or 
 * it's traveled distance to be less than previous path's traveled distance to this coordinate. if isExit == 1, we can be 
 * sure that we have found the shortest path.
 * 
*/

#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
#include <queue>
#include <stack>
#include <map>

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
  double traveled;
  list<Node*> children;
  double estimatedTotalDistance;

  Node(Node* parent, Coordinate* coordinate, double traveled): parent(parent), coordinate(coordinate), traveled(traveled) {}
  ~Node(){
    delete coordinate;
    for (Node* n: children){
      delete n;
    }
  }
};
//utility method that is used in the priority queue to order nodes in the queue
struct compareTraveled{
  bool operator()(Node* lhs, Node *rhs){
      return lhs->estimatedTotalDistance > rhs->estimatedTotalDistance;
  }  
};

class AStarFirstSearchAgent {

priority_queue<Node*, vector<Node*>, compareTraveled> fringe;
Node* root;
Node* current;
map<Coordinate, int> visited;

public:

  AStarFirstSearchAgent(int size_x, int size_y) {
    root = new Node(NULL, new Coordinate(0, 0), 0);
    current = root;    
    visited[Coordinate(0,0)] = 0;
  }

  ~AStarFirstSearchAgent() {
    delete root;
  }

  //method to add new nodes into the fringe
  void add(bool hasWall, Coordinate coordinate, int childOrder){
    if (hasWall){
      return;
    }

    if (visited.find(coordinate) != visited.end() && visited[coordinate] <= current->traveled + 1){
      return;
    }

    Node* n = new Node(current, new Coordinate(coordinate.getX(), 
                        coordinate.getY()), current->traveled+1);
    current->children.push_back(n);
    visited[*n->coordinate] = n->traveled;
  }

  optional<Coordinate> move(bool isExit, bool hasWallSouth, bool hasWallNorth, bool hasWallEast, bool hasWallWest, double distance) {
    
    if (isExit){
      return {};
    }

    int x = current->coordinate->getX(), y = current->coordinate->getY(); 
    current->estimatedTotalDistance = current->traveled + distance;
    fringe.push(current);

    add(hasWallSouth, Coordinate(x, y + 1), 0);
    add(hasWallNorth, Coordinate(x, y - 1), 1);
    add(hasWallEast, Coordinate(x + 1, y), 2);
    add(hasWallWest, Coordinate(x - 1, y), 3);
    
    //if the most desirable node has no children, we don't need it in the fringe anymore 
    while(fringe.top()->children.empty()){
      fringe.pop();
    }

    current = fringe.top()->children.front();
    fringe.top()->children.pop_front();
    
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

  AStarFirstSearchAgent agent(size_x, size_y);

  while(true) {
    string s1, s2, s3, s4, s5, s6;
    cin >> s1 >> s2 >> s3 >> s4 >> s5 >> s6;

    bool isExit = (s1 != "0");
    bool hasWallSouth = (s2 != "0");
    bool hasWallNorth = (s3 != "0");
    bool hasWallEast = (s4 != "0");
    bool hasWallWest = (s5 != "0");
    double distance = stof(s6);

    auto coord = agent.move(isExit, hasWallSouth, hasWallNorth, hasWallEast, hasWallWest, distance);

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
