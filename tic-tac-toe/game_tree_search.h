/*
 * Author: Bekatan Satyev
 * Student_ID: 20182024
 * Email: bekatans@unist.ac.kr
 */

#ifndef GAME_TREE_GAME_TREE_SEARCH_H
#define GAME_TREE_GAME_TREE_SEARCH_H

#include <iostream>

#include "game_spec.h"

struct State{
  State() = default;
  State(State* parent, int payoff, std::vector<int> field): parent(parent), payoff(payoff), field(field){}
  ~State(){
    for (auto child : children){
      delete child;
    }
  }
  struct State* parent;
  std::vector<struct State*> children;
  int payoff;
  std::vector<int>field;
};

class GameTree {

public:

  int computeGameTree(const GameSpec& spec);

  void printGameTree(const GameSpec& spec);

  void playAsFirstPlayer(const GameSpec& spec);

  void playAsSecondPlayer(const GameSpec& spec);

};


#endif //GAME_TREE_GAME_TREE_SEARCH_H
