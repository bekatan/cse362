/*
 * Author: Tsz-Chiu Au
 *
 * Created on 2022-09-29
 */

#include <iostream>
#include <chrono>
#include <memory>
#include <cstring>

#include "game_spec.h"
#include "game_tree_search.h"


class CommandLineArgument {

  std::string program_name;
  std::string spec_filename;

  bool is_arg_valid = false;
  bool is_help = false;
  bool is_verbose = false;
  bool is_playing = false;
  bool is_play_as_first_player = false;

public:

  CommandLineArgument(int argc, char *argv[]) : program_name(argv[0]) {
    if (argc == 2) {
      if (strcmp(argv[1], "-h") == 0) {
        is_help = true;
        is_arg_valid = true;
      } else {
        spec_filename = argv[1];
        is_arg_valid = true;
      }
    } else if (argc == 3) {
      if (strcmp(argv[1], "-v") == 0) {
        is_verbose = true;
        spec_filename = argv[2];
        is_arg_valid = true;
      } else if (strcmp(argv[1], "-p1") == 0) {
        is_playing = true;
        is_play_as_first_player = true;
        spec_filename = argv[2];
        is_arg_valid = true;
      } else if (strcmp(argv[1], "-p2") == 0) {
        is_playing = true;
        is_play_as_first_player = false;
        spec_filename = argv[2];
        is_arg_valid = true;
      } else {
        is_arg_valid = false;
      }
    } else {
      is_arg_valid = false;
    }
  }

  bool isArgValid() const { return is_arg_valid; }
  bool isHelp() const { return is_help; }
  bool isVerbose() const { return is_verbose; }
  bool isPlaying() const { return is_playing; }
  bool isPlayAsFirstPlayer() const { return is_play_as_first_player; }
  std::string getSpecFileName() const { return spec_filename; }

  void printUsage() {
    std::cout << "Usage: " << program_name << " [-v] spec.txt" << std::endl;
    std::cout << "       " << program_name << " [-p1|-p2] spec.txt" << std::endl;
    std::cout << "       " << program_name << " -h" << std::endl;
    std::cout << std::endl;
    std::cout << "where -v        printGameTree the game tree's states." << std::endl;
    std::cout << "      -p1       computer starts playing as the first player." << std::endl;
    std::cout << "      -p2       computer starts playing as the second player." << std::endl;
    std::cout << "      -h        show this help message." << std::endl;
    std::cout << "      spec.txt  the game specification file." << std::endl;
    std::cout << std::endl;
  }
};


/*
 * The main function
 */
int main(int argc, char** argv) {

  CommandLineArgument cl_arg{argc, argv};

  if (!cl_arg.isArgValid()) {
    std::cout << "Error: Invalid command line argument" << std::endl;
    std::cout << std::endl;
    cl_arg.printUsage();
    return EXIT_FAILURE;
  }

  if (cl_arg.getSpecFileName()[0] == '-') {
    std::cout << "Error: Invalid filename" << std::endl;
    std::cout << std::endl;
    cl_arg.printUsage();
    return EXIT_FAILURE;
  }

  if (cl_arg.isHelp()) {
    cl_arg.printUsage();
    return EXIT_SUCCESS;
  }

  std::unique_ptr<GameSpec> spec;

  try {
    spec = std::make_unique<GameSpec>(cl_arg.getSpecFileName());
  } catch(std::exception const& e) {
    std::cout << e.what() << std::endl;
    std::cout << "Error: Invalid game specification file." << std::endl;
    std::cout << std::endl;
    cl_arg.printUsage();
    return EXIT_FAILURE;
  }

  std::unique_ptr<GameTree> game_tree;

  try {
    game_tree = std::make_unique<GameTree>();
  } catch(std::exception const& e) {
    std::cout << e.what() << std::endl;
    std::cout << "Error: Failed to create the game tree." << std::endl;
    std::cout << std::endl;
    cl_arg.printUsage();
    return EXIT_FAILURE;
  }

  if (!cl_arg.isPlaying()) {
    if (!cl_arg.isVerbose()) {
      auto start_time = std::chrono::high_resolution_clock::now();
      int root_minimax_value = game_tree->computeGameTree(*spec);
      auto end_time = std::chrono::high_resolution_clock::now();

      if (root_minimax_value == 0) {
        std::cout << "This is a balanced game." << std::endl;
      } else if (root_minimax_value > 0) {
        std::cout << "This game favors the first player." << std::endl;
      } else {  // v < 0
        std::cout << "This game favors the second player." << std::endl;
      }

      std::chrono::duration<double> time_diff = end_time - start_time;
      std::cout << "Running time = " << std::fixed << time_diff.count() << "s" << std::endl;
    } else {
      game_tree->printGameTree(*spec);
    }
  } else {  // cl_arg.isPlaying() is true
    if (cl_arg.isPlayAsFirstPlayer()) {
      game_tree->playAsFirstPlayer(*spec);
    } else {
      game_tree->playAsSecondPlayer(*spec);
    }
  }

  return EXIT_SUCCESS;
}

