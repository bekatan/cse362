# CSE362 Coursework Repository - Maze Runner and Tic-Tac-Toe AI

Welcome to the CSE362 Coursework Repository! This repository contains two modules: one for implementing maze runner algorithms (DFS, BFS, and A* search algorithms) and another for a minimax game tree search algorithm for tic-tac-toe. The purpose of these modules is to demonstrate various artificial intelligence algorithms used in different scenarios.

## Table of Contents

- [Introduction](#introduction)
- [Maze Runner Module](#maze-runner-module)
- [Tic-Tac-Toe AI Module](#tic-tac-toe-ai-module)
- [How to Use](#how-to-use)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This repository is created as part of the CSE362 coursework to showcase the application of artificial intelligence techniques in two different domains: maze navigation and playing tic-tac-toe. The repository contains separate modules for each domain, with well-documented code and explanations.

## Maze Runner Module

The Maze Runner module focuses on solving mazes using three different search algorithms: Depth-First Search (DFS), Breadth-First Search (BFS), and A* search algorithm. Each algorithm is implemented in its respective file:

- `dfs.cpp`: Contains the DFS algorithm implementation.
- `bfs.cpp`: Contains the BFS algorithm implementation.
- `astar.cpp`: Contains the A* search algorithm implementation.

In each file, you will find functions that take a maze as input and return the path from the start point to the goal point using the corresponding algorithm. The algorithms use appropriate data structures and heuristics to navigate through the maze efficiently.

## Tic-Tac-Toe AI Module

The Tic-Tac-Toe AI module implements a Minimax game tree search algorithm to create an unbeatable AI for playing Tic-Tac-Toe. The files in this module are as follows:

- `game_tree_search.cpp`: Contains the Tic-Tac-Toe game implementation and the Minimax algorithm for the AI player.

The Minimax algorithm performs an exhaustive search of the game tree to determine the best possible move for the AI player in each turn. By using this algorithm, the AI player can guarantee a win or, at worst, a draw in any situation.

## How to Use

To use either of the modules, follow the steps below:

1. Clone the repository to your local machine:

```
git clone https://github.com/bekatan/cse362.git
```

2. Change into the repository directory:

```
cd cse362
```

3. For the Maze Runner module, follow instructions [here](http://ai.unist.ac.kr/~chiu/ai_resource/prog/hw1/index.html).

4. For the Tic-Tac-Toe AI module, follow instructions [here](http://ai.unist.ac.kr/~chiu/ai_resource/prog/hw2/index.html).

## Contributing

Contributions to this coursework repository are welcome! If you find any issues or want to enhance the code or documentation, feel free to create a pull request. Make sure to follow the repository's guidelines and code of conduct when contributing.

## License

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute the code as long as you provide appropriate attribution to the original authors. See the [LICENSE](LICENSE) file for more details.

---

We hope this repository helps you understand maze navigation algorithms and how to create an unbeatable AI for Tic-Tac-Toe using the Minimax algorithm. Enjoy exploring the code and have fun experimenting with AI techniques! If you have any questions or need further assistance, please don't hesitate to reach out. Happy coding!
