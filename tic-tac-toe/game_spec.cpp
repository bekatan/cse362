/*
 * Author: Tsz-Chiu Au
 *
 * Created on 2022-09-29
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "game_spec.h"


GameSpec::GameSpec(const std::string& spec_filename) {
  std::ifstream f(spec_filename);

  if (!f.is_open()) throw std::runtime_error("Error: Cannot open the game specification file.");

  f >> sizeX >> sizeY;

  if (f.bad()) throw std::runtime_error("Error: Fail to read from the game specification file.");
  if (sizeX < 1 || sizeY < 1) throw std::runtime_error("Error: Invalid board size.");

  size = sizeX * sizeY;

  board.resize(size);

  for (int y=0; y<sizeY; y++) {
    for (int x=0; x<sizeX; x++) {
      int i = getMoveId(x, y);
      int v;
      f >> v;
      if (v >= 0) {
        if (v != i) throw std::runtime_error("Error: Incorrect board entry.");
        board[i] = true;
      } else {
        board[i] = false;
      }
    }
  }

  while(true) {
    if (f.bad()) throw std::runtime_error("Error: Unable to read winning sequences.");
    int n;
    f >> n;
    if (n < 0) break;  // no more winning sequences
    winseq_list.emplace_back();
    for(int i=0; i<n; i++) {
      int v;
      f >> v;
      if (isBlocked(v)) throw std::runtime_error("Error: Winning sequences cannot include blocked locations");
      for(auto v2 : winseq_list.back()) {
        if (v2 == v) throw std::runtime_error("Error: Duplicated locations in a winning sequences.");
      }
      winseq_list.back().push_back(v);
    }
  }

  f.close();
}


void GameSpec::print() const {
  std::cout << "------ Game Specification Begin ------"<< std::endl;
  std::cout << "Board positions:"<< std::endl;
  for (int y=0; y<sizeY; y++) {
    std::cout << " ";
    for (int x=0; x<sizeX; x++) {
      if (isBlocked(x, y)) {
        std::cout << "  #";
      } else {
        std::cout << " " << std::setw(2) << getMoveId(x, y);
      }
    }
    std::cout << std::endl;
  }
  std::cout << "Winning sequences:"<< std::endl;
  for (auto & ws : winseq_list) {
    std::cout << "  ";
    for(int move : ws) {
      std::cout << std::setw(2) << move << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "------ Game Specification End ------"<< std::endl;
  std::cout << std::endl;
}

