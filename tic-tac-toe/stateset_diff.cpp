/*
 * Author: Tsz-Chiu Au
 *
 * Created on 2022-09-29
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <cstring>
#include <cassert>

#include "game_spec.h"


// -------------------------------------------------------------------------------------
// bit state manipulation
// -------------------------------------------------------------------------------------

#define INT_BIT_NUM 32
#define COMPLETE_BIT_MASK 0xffffffff

using bcode = uint32_t;
using bstate = uint64_t;

const bcode bit_table[INT_BIT_NUM] = {
    0x00000001,
    0x00000002,
    0x00000004,
    0x00000008,
    0x00000010,
    0x00000020,
    0x00000040,
    0x00000080,
    0x00000100,
    0x00000200,
    0x00000400,
    0x00000800,
    0x00001000,
    0x00002000,
    0x00004000,
    0x00008000,
    0x00010000,
    0x00020000,
    0x00040000,
    0x00080000,
    0x00100000,
    0x00200000,
    0x00400000,
    0x00800000,
    0x01000000,
    0x02000000,
    0x04000000,
    0x08000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000
};


// one_bit_flag should have exactly one 1-bit. This function returns the index of that 1-bit. Return -1 if all bits are zero.
inline int getBitId(bcode one_bit_flag) { return __builtin_ffs(one_bit_flag) - 1; }

//int getBitId(bcode bit) {    // an alternative implementation of getBitId()
//  int i = -1;
//  do {
//    bit >>= 1;
//    i++;
//  } while (bit != 0);
//  return i;
//}

inline bcode getLowerBitCode(bstate state) { return (COMPLETE_BIT_MASK & state); }

inline bcode getUpperBitCode(bstate state) { return (state >> INT_BIT_NUM); }

inline bstate calcBitState(bcode lower_bcode, bcode upper_bcode) {
  return static_cast<bstate>(lower_bcode) | (static_cast<bstate>(upper_bcode) << INT_BIT_NUM);
}



// -------------------------------------------------------------------------------------
// Compact Game Spec
// -------------------------------------------------------------------------------------


class CompactGameSpec {

  int move_num;

  std::vector<int> original_move_to_compact_move;  // -1 if the original move is a block
  std::vector<int> compact_move_to_original_move;

  int ws_code_num;
  std::vector<bcode> ws_code;

  bcode terminal_mask;  // all compact move ids are 1

public:

  explicit CompactGameSpec(const GameSpec& spec);

  int getNumOfMove() const { return move_num; }
  int convertFromOriginalToCompactMoveId(int i) const { return original_move_to_compact_move[i]; }
  int convertFromCompactToOriginalMoveId(int i) const { return compact_move_to_original_move[i]; }

  int getNumOfWinSeqCode() const { return ws_code_num; }
  bcode getWinSeqCode(int i) const { return ws_code[i]; }
  const std::vector<bcode>& getWinSeqCodes() const { return ws_code; }

  bcode getTerminalMask() const { return terminal_mask; }

};


CompactGameSpec::CompactGameSpec(const GameSpec& spec) :
  original_move_to_compact_move(spec.getSize()),
  compact_move_to_original_move(spec.getSize()),
  ws_code_num(spec.getNumOfWinSeq()),
  ws_code(spec.getNumOfWinSeq())
{
  move_num=0;

  for(int i=0; i<spec.getSize(); i++) {
    if (!spec.isBlocked(i)) {
      original_move_to_compact_move[i] = move_num;
      compact_move_to_original_move[move_num] = i;
      move_num++;
    } else {
      original_move_to_compact_move[i] = -1;
    }
  }

  if (move_num > INT_BIT_NUM) throw std::runtime_error("Error: the number of possible moves is too large.");

  for(int i=0; i<ws_code_num; i++) {
    ws_code[i] = 0;
    for(auto orig_move : spec.getWinSeq()[i]) {
      ws_code[i] |= bit_table[original_move_to_compact_move[orig_move]];
    }
  }

  terminal_mask = 0;
  for(int i=0; i < move_num; i++) {
    terminal_mask |= bit_table[i];
  }

  assert(move_num == __builtin_popcount(terminal_mask));
}


// -------------------------------------------------------------------------------------
// State Set
// -------------------------------------------------------------------------------------


class StateSet {

  using payoff_t = char;

  static constexpr payoff_t PRUNED_PAYOFF = std::numeric_limits<payoff_t>::min() + 1;

  const GameSpec& spec;
  const CompactGameSpec compact_spec;

  std::unordered_map<bstate, payoff_t> payoff_table;

public:

  StateSet(const GameSpec& spec, const std::string& statefilename);

  int setminus(const StateSet &sset, bool is_check_payoff);

  void printDivider();
  void printState(bstate state);
};


StateSet::StateSet(const GameSpec& spec, const std::string& statefilename) :
  spec(spec), compact_spec(spec)
{
  assert(sizeof(uint32_t) == 4 && "Error: This program only works in system in which the size of uint32_t is 4 bytes.");
  assert(sizeof(uint64_t) == 8 && "Error: This program only works in system in which the size of uint64_t is 8 bytes.");

  std::string bar;

  for (int x = 0; x < spec.getSizeX(); x++) { bar += "-"; }

  std::ifstream infile(statefilename);
  if (!infile.is_open()) throw std::runtime_error("Error: Cannot open the game tree file.");

  while (infile.good()) {

    // --- process the divider line ---
    std::string str1;
    infile >> str1;
    if (str1.empty()) break; // assume an empty line means the end of file
    if (str1 != bar) throw std::runtime_error("Error: Incorrect state file format.");
    std::getline(infile, str1); // read until the end of the line
    std::stringstream ss1(str1);
    std::string last_word;
    while(std::getline(ss1, last_word, ' '));  // skip until the last word

    bool isPruned;
    payoff_t payoff;
    if (last_word == "pruned") {
      isPruned = true;
    } else {
      isPruned = false;
      std::string last_number;
      std::stringstream ss2(last_word);
      while (std::getline(ss2, last_number, '='));  // skip until the string after =
      payoff = static_cast<payoff_t>(std::stoi(last_number));
      if (std::to_string(static_cast<int>(payoff)) != last_number) throw std::runtime_error("Error: payoff is not a number.");
    }

    // --- process state ---
    int id = 1;
    bcode bcode1 = 0, bcode2 = 0;
    for (int y = 0; y < spec.getSizeY(); y++) {
      std::string str2;
      infile >> str2;
      if (str2.size() != spec.getSizeX()) throw std::runtime_error("Error: Incorrect state format.");
      for (int x = 0; x < spec.getSizeX(); x++) {
        char c = str2[x];
        if (c != '#') {
          if (c == 'O') {
            bcode1 |= id;
          } else if (c == 'X') {
            bcode2 |= id;
          } else if (c == '.') {
            // do nothing
          } else {
            throw std::runtime_error("Error: Incorrect character in state format.");
          }
          id <<= 1;
        }
      }
    }

    bstate state = calcBitState(bcode1, bcode2);
    payoff_table[state] = isPruned?PRUNED_PAYOFF:payoff;
  }

  infile.close();
}


int StateSet::setminus(const StateSet& sset, bool is_check_payoff) {
  int diff_count = 0;

  for (auto [state1, payoff1] : payoff_table) {
    bool isFound = false;
    if (sset.payoff_table.count(state1) == 0) {
      printDivider();
      std::cout << " this state does not exist in the second game tree" << std::endl;
      printState(state1);
      isFound = true;
    } else if (is_check_payoff) {
      if (payoff1 != PRUNED_PAYOFF) {
        int payoff2 = static_cast<int>(sset.payoff_table.at(state1));
        if (payoff2 != PRUNED_PAYOFF) {
          if (payoff1 != payoff2) {
            printDivider();
            std::cout << " payoff " << static_cast<int>(payoff1) << " is different from payoff " << static_cast<int>(payoff2) << std::endl;
            printState(state1);
            isFound = true;
          }
        } // else payoff2 == PRUNED_PAYOFF, then no need to check the payoff
      } // else payoff1 == PRUNED_PAYOFF, then no need to check the payoff
    }
    if (isFound) {
      diff_count++;
    }
  }

  return diff_count;
}


void StateSet::printDivider() {
  for (int x = 0; x < spec.getSizeX(); x++) std::cout << "-";
}


void StateSet::printState(bstate state) {
  bcode pos1 = getLowerBitCode(state);
  bcode pos2 = getUpperBitCode(state);
  for (int y = 0; y < spec.getSizeY(); y++) {
    for (int x = 0; x < spec.getSizeX(); x++) {
      int move_id = spec.getMoveId(x, y);
      int compact_move_id = compact_spec.convertFromOriginalToCompactMoveId(move_id);
      if (compact_move_id >= 0) {
        if ((pos1 & bit_table[compact_move_id]) != 0) {
          std::cout << "O";
        } else if ((pos2 & bit_table[compact_move_id]) != 0) {
          std::cout << "X";
        } else {
          std::cout << "*";
        }
      } else {
        std::cout << "#";
      }
    }
    std::cout << std::endl;
  }
}


// -------------------------------------------------------------------------------------
// The main function
// -------------------------------------------------------------------------------------


int main(int argc, char **argv) {

  bool isCheckPayoff = false;

  std::string spec_filename;
  std::string state_filename_1;
  std::string state_filename_2;

  if (argc == 4) {
    spec_filename = argv[1];
    state_filename_1 = argv[2];
    state_filename_2 = argv[3];
  } else if (argc == 5 && strcmp(argv[1], "-p") == 0) {
    isCheckPayoff = true;
    spec_filename = argv[2];
    state_filename_1 = argv[3];
    state_filename_2 = argv[4];
  } else {
    std::cerr << "Error in command line arguments." << std::endl;
    std::cerr << std::endl;
    std::cerr << "Usage: " << argv[0] << " [-p] spec.txt stateset1.txt stateset2.txt" << std::endl;
    std::cerr << std::endl;
    std::cerr << "where -p             ignore the payoff when checking the equality of states." << std::endl;
    std::cerr << "      spec.txt       the game specification file." << std::endl;
    std::cerr << "      stateset1.txt  a file containing the states of a game tree." << std::endl;
    std::cerr << "      stateset2.txt  a file containing the states of a game tree that" << std::endl;
    std::cerr << "                     could be a subset of those in stateset1.txt." << std::endl;
    return EXIT_FAILURE;
  }

  GameSpec spec(spec_filename);
  StateSet sset1(spec, state_filename_1);
  StateSet sset2(spec, state_filename_2);

  int diff_count = sset2.setminus(sset1, isCheckPayoff);

  if (diff_count == 0) {
    std::cout << "There is no state that exists in the second set but not in the first set.";
    if (!isCheckPayoff) std::cout << " (payoff ignored)" << std::endl;
  } else {
    std::cout << std::endl;
    if (diff_count == 1) {
      std::cout << "There is 1 state that exists ";
    } else {
      std::cout << "There are " << diff_count << " states that exist ";
    }
    std::cout << "in the second set but not in the first set.";
    if (!isCheckPayoff) {
      std::cout << " (payoff ignored)" << std::endl;
    } else {
      std::cout << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
