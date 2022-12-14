#IMPLEMENTATION
1. Breadth First Search
    - The problem of finding the Exit in a maze is converted to traversing a tree with a breadth first search approach. In this tree each node represents a point in the maze with properties such as Coordinate, pointers to the parent Node and a container with pointers to children Nodes. The BreadFirstSearchAgent has properties such as pointer to the root node, current node, queue of fringe nodes and an ordered set of visited Coordinates. set container is choosen due to its logarithmic insertion and access time. In order to implement iestthe orderet set of Coordinates, operator<(...) is overridden in Coordinates implementation.
    - In move(...) all new coordinates where the path can go are first checked if they are in the visited set, if not then added to the array of current's children nodes (so we can delete them later), added to the queue of fringe nodes and set of visited places. The FIFO policy of queues allows us to implement bfs, since all new child nodes are the first ones to be accessed. When isExit is 1, we can be sure that the first path we found is the shortest one, because of bfs approach.
    - In getShortestPath(...) since the current points to the exit we can trace back the shortest path to the root by continuously referencing the parent pointer of the current.
2. Depth First Search
    - The problem of finding the Exit in a maze is converted to traversing a tree with a depth first search approach. In this tree each node represents a point in the maze with properties such as Coordinate, pointer to the parent Node, a container with pointers to children Nodes and the number of steps taken to reach this point - traveled. The DepthFirstSearchAgent has properties such as pointers to the root node, current node, the tip of the shortest path, stack of fringe nodes, an array of several depth limits of increasing degrees and an ordered map of visited Coordinates as keys and shortest distance traveled as values. map container is choosen due to its logarithmic insertion and access time and ability to hold the value shortest distance traveled to reach each coordinate. In order to implement the orderet map, operator<(...) is overridden in Coordinates implementation.
    - In move(...) all new coordinates where the path can go are first checked if they are not in the visited map or if the current distance traveled is less than previous distance, if so then the node is added to the array of current's children nodes (so we can delete them later), added to the stack of fringe nodes and map of visited coordinates with current distance traveled. The FILO policy of stacks allows us to implement dfs, since new child nodes are accessed before older nodes. When a current limit of depth is reached the new children are not visited. Instead we make sure that all paths in the fringe are within the limits. Only then the next depth limit is set. The highest depth limit is the area of the maze, which bounds the longest non-looping path in the maze. When isExit is 1, we need to make sure that we have the shortest path to it, so we traverse each path to the limit to make find the shortest path. Node pointer shortest is given to the node with the lowest traveled value.
    - In getShortestPath(...) since the shortest points to the exit with the shortest path we can trace back the shortest path to the root by continuously referencing the parent pointer of the current.
3. A Star First Search
    - astar is implemented in a way similar to bfs. We have a queue of nodes, so we have a FIFO policy. However the queue is ordered depending on the estimatedTotalDistance from the node to the goal. this estimated total distance is the sum of traveled distance and estimated distance from this nodes coordinates. Because priority queue requires estimatedTotalDistance to be calculated, child nodes are not pushed to the queue until they are visited. Insted the coordinates of children of the node with lowest estimatedTotalDistance are returned from move(...). compareTraveled(...) is utility method that is used in the priority queue to order nodes in the queue. One problem is that some path may not be optimal even though it has lower estimated distance for some time compared to the actual optimal path, so similar to the dfs implementation for each new node to be added it's coordinates are checked to be absent in the visited list or it's traveled distance to be less than previous path's traveled distance to this coordinate. if isExit == 1, we can be sure that we have found the shortest path.

#PERFORMANCE

    Note: Trying to create a maze of size 300x300 even before starting the agent causes Stack Overflow on the server. Even with ulimit -s 100000.
    """
        Exception in thread "AWT-EventQueue-0" java.lang.StackOverflowError
                at java.base/java.util.Random.nextInt(Random.java:322)
                at mazegame.Maze.mazeGenByDFS(Maze.java:177)
                at mazegame.Maze.mazeGenByDFS(Maze.java:189)
    """
    Since this is not related to my code and both BFS and ASTAR have no issues with solving 300x300 
    mazes on my 5th gen i5 with 8 GB RAM system, I will be reporting the max size as 300x300 for BFS and ASTAR.
    
    The performance of DFS is also seems to be lower on the server than on my laptop. It takes longer to complete and can't handle 100x100 as well as the run on my system.

1. BFS:
    - It can always return the shortest path
    - Biggest size of the maze it can consistently solve under 2 minutes: 300x300
    - Average number of moves for different maze sizes (for each maze size we have 5 mazes generated):
        - 100x100: 2072 moves
        - 200x200: 5065 moves
        - 300x300: 28313 moves

2. ASTAR:
    - It can always return the shortest path
    - Biggest size of the maze it can consistently solve under 2 minutes: 300x300
    - Average number of moves for different maze sizes (for each maze size we have 5 mazes generated):
        - 100x100: 1344 moves
        - 200x200: 2557 moves
        - 300x300: 21782 moves

3. DFS:
    - It can always return the shortest path
    - Biggest size of the maze it can consistently solve under 2 minutes: 80x60
    - Average number of moves for different maze sizes (for each maze size we have 5 mazes generated):
        - 30x30: 3016 moves
        - 50x50: 10301 moves
        - 80x60: 41348 moves
