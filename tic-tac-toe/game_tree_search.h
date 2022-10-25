/*
 * Author: Your_Name
 * Student_ID: Your_Student_Id
 * Email: Your_Email_Address
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
