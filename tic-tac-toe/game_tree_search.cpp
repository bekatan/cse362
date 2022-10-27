/*
 * Author: Bekatan Satyev
 * Student_ID: 20182024
 * Email: bekatans@unist.ac.kr
 */

#include <iostream>

#include "game_spec.h"
#include "game_tree_search.h"

int computeCase(GameSpec spec, std::vector<int> field, bool turn){
    
    //check if anybody won
    auto winseq = spec.getWinSeq();

    for (auto seq : winseq){
        int move = field[seq[0]];
        bool win = true;
        if (move < 1){
            continue;
        }

        for (int index : seq){
            if (field[index] != move){
                win = false;
                break;
            }
        }

        if (win){
            return 1-2*turn;
        }
    }

    //check if the field is full
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

    for (int i = 0; i < spec.getSize(); i++){
        if (field[i] != 0){
            continue;
        }
        field[i] = 2-turn;
        curPayoff = computeCase(spec, field, !turn);
        
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
    return computeCase(spec, field, true);
}


void GameTree::printGameTree(const GameSpec& spec) {

}


void GameTree::playAsFirstPlayer(const GameSpec& spec) {

}


void GameTree::playAsSecondPlayer(const GameSpec& spec) {

}

