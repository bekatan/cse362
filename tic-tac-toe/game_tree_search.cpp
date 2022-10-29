/*
 * Author: Bekatan Satyev
 * Student_ID: 20182024
 * Email: bekatans@unist.ac.kr
 */

#include <iostream>
#include <queue>

#include "game_spec.h"
#include "game_tree_search.h"

struct State* computeCase(int size, std::vector<std::vector<int>> winseq, std::vector<int> field, bool turn, bool verbose = false, State* parent = NULL){
    // for (auto i : field)
    //     std::cout<<i;
    // std::cout<<std::endl;
    struct State* cur = new State();
    cur->parent = parent;
    cur->field = field;
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
                // winseq.erase(winseq.begin() + i);
                // i--;    
                break;
            }
        }
        // std::cout<<"waawki"<<std::endl;
        if (win){
            // std::cout<<"win case"<<std::endl;
            cur->payoff = 1-2*turn;
            return cur;
        }
    }
    // std::cout<<"hlaaaaami"<<std::endl;

    //check if the game is over
    if (winseq.empty()){
        cur->payoff = 0;
        return cur;
    }
    // std::cout<<"the eeend"<<std::endl;

    bool full = true;
    for (int i : field){
        if (i == 0){
            full = false;
            break;
        }
    }

    if (full){
        cur->payoff = 0;
        return cur;
    }

    //branch out to children

    int payoff = 1-2*turn, curPayoff;
    // std::cout<<"the eeend"<<std::endl;

    for (int i = 0; i < size; i++){
        if (field[i] != 0){
            continue;
        }
        field[i] = 2-turn;
        State* curChild = computeCase(size, winseq, field, !turn, verbose, cur);
        cur->children.push_back(curChild);
        curPayoff = curChild->payoff;

        if (turn){
            if (curPayoff == 1){
                cur->payoff = 1;
                for (i++; i < size; i++){
                    cur->children.push_back(new State(cur, 2, field));
                }
                return cur;
            } else {
                payoff = std::max(payoff, curPayoff);
            }
        } else {
            if (curPayoff == -1){
                cur->payoff = -1;
                for (i++; i < size; i++){
                    cur->children.push_back(new State(cur, 2, field));
                }
                return cur;
            }else{
                payoff = std::min(payoff, curPayoff);
            }
        }
        
        field[i] = 0;
    }
    cur->payoff = payoff;
    // std::cout<<"the eeend"<<std::endl;
    return cur;
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
    // std::cout<<"my tut"<<std::endl;
    State* res = computeCase(spec.getSize(), spec.getWinSeq() , field, true);
    // std::cout<<"my tut"<<std::endl;
    int payoff = res->payoff;
    delete res;
    return payoff;
}


void GameTree::printGameTree(const GameSpec& spec) {
    std::vector<int> field;
    for (int i = 0; i < spec.getSize(); i++){
        if (spec.isBlocked(i)){
            field.push_back(-1);
        } else {
            field.push_back(0);
        }
    }
    
    State* res = computeCase(spec.getSize(), spec.getWinSeq() , field, true);
    
    int id = 0;
    std::queue<State*> q;
    q.push(res);
    State* cur;
    std::string print = "";

    while (!q.empty()){
        cur = q.front();
        q.pop();
        print += "--- s" + std::to_string(id++) + " payoff = " + std::to_string(cur->payoff) + "\n";
        // if (cur->parent){
        //     print += "s" + cur->parent
        // }
        // std::cout<<"my tut"<<std::endl;

        for (int i = 0; i < spec.getSizeY(); i++){
            for (int j = 0; j < spec.getSizeX(); j++){
                // std::cout<<"my tut"<<std::endl;
                print += std::to_string(cur->field[spec.getMoveId(j,i)]);
            }
            print += "\n";
        }

        for (State *child: cur->children){
            q.push(child);
        }
    }
    std::cout<<print<<std::endl;
}


void GameTree::playAsFirstPlayer(const GameSpec& spec) {

}


void GameTree::playAsSecondPlayer(const GameSpec& spec) {

}

