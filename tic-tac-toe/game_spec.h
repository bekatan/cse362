/*
 * Author: Tsz-Chiu Au
 *
 * Created on 2022-09-29
 */

#ifndef GAME_TREE_GAME_SPEC_H
#define GAME_TREE_GAME_SPEC_H

#include <iostream>
#include <string>
#include <vector>


class GameSpec {

  int size;
  int sizeX, sizeY;

  std::vector<bool> board;  //  board[i] is true if there is no block at position i
  std::vector<std::vector<int>> winseq_list;  // winseq_list is a list of positions in a winning sequence

public:

  GameSpec() = default;
  GameSpec(const std::string& spec_filename);

  int getSize() const { return size; }
  int getSizeX() const { return sizeX; }
  int getSizeY() const { return sizeY; }

  int getNumOfWinSeq() const { return winseq_list.size(); }
  const std::vector<std::vector<int>>& getWinSeq() const { return winseq_list; }

  std::pair<int,int> getMovePos(int move) const { return std::make_pair(move % sizeX, move / sizeX); }
  int getMoveId(int x, int y) const { return x + sizeX * y; }

  bool isBlocked(int i) const { return !board[i]; }
  bool isBlocked(int x, int y) const { return !board[getMoveId(x, y)]; }
  bool isValidMove(int move) const { return 0 <= move && move < size && board[move]; }

  void print() const;

};

#endif //GAME_TREE_GAME_SPEC_H
