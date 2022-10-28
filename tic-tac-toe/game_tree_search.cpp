/*
 * Author: Bekatan Satyev
 * Student_ID: 20182024
 * Email: bekatans@unist.ac.kr
 */

#include <iostream>

#include "game_spec.h"
#include "game_tree_search.h"

int computeCase(int size, std::vector<std::vector<int>> winseq, std::vector<int> field, bool turn, bool verbose = false){
    
    //check if anybody won
    // auto winseq = spec.getWinSeq();
    
    for (int i = 0; i < winseq.size(); i++){
        int move = field[winseq[i][0]];
        bool win = true;
        if (move < 1){
            continue;
        }

        for (int index : winseq[i]){
            if (field[index] != move){
                win = false;
                winseq.erase(winseq.begin() + i);
                break;
            }
        }

        if (win){
            return 1-2*turn;
        }
    }

    //check if the game if over
    if (winseq.empty()){
        return 0;
    }

    bool full = true;
    for (int i : field){
        if (i == 0){
            full = false;
            break;
        }
    }

    if (full){
        return 0;
    }

    //branch out to children

    int payoff = 1-2*turn, curPayoff;

    for (int i = 0; i < size; i++){
        if (field[i] != 0){
            continue;
        }
        field[i] = 2-turn;
        curPayoff = computeCase(size, winseq, field, !turn);
        
        if (turn){
            if (curPayoff == 1){
                return 1;
            } else {
                payoff = std::max(payoff, curPayoff);
            }
        } else {
            if (curPayoff == -1){
                return -1;
            }else{
                payoff = std::min(payoff, curPayoff);
            }
        }
        
        field[i] = 0;
    }

    return payoff;
}

int GameTree::computeGameTree(const GameSpec& spec) {
    std::vector<int> field;
    for (int i = 0; i < spec.getSize(); i++){
        if (spec.isBlocked(i)){
            field.push_back(-1);
        } else {
            field.push_back(0);
        }
    }
    return computeCase(spec.getSize(), spec.getWinSeq() , field, true);
}


void GameTree::printGameTree(const GameSpec& spec) {

}


void GameTree::playAsFirstPlayer(const GameSpec& spec) {

}


void GameTree::playAsSecondPlayer(const GameSpec& spec) {

}

