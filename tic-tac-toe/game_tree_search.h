/*
 * Author: Bekatan Satyev
 * Student_ID: 20182024
 * Email: bekatans@unist.ac.kr
 */

#ifndef GAME_TREE_GAME_TREE_SEARCH_H
#define GAME_TREE_GAME_TREE_SEARCH_H

#include <iostream>

#include "game_spec.h"


class GameTree {

public:

  int computeGameTree(const GameSpec& spec);

  void printGameTree(const GameSpec& spec);

  void playAsFirstPlayer(const GameSpec& spec);

  void playAsSecondPlayer(const GameSpec& spec);

};


#endif //GAME_TREE_GAME_TREE_SEARCH_H
