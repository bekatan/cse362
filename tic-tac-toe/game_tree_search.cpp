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
    
    struct State* cur = new State();
    if (verbose){
        cur->parent = parent;
        cur->field = field;
    }

    //check if anybody won
    for (int i = winseq.size()-1; i >= 0; i--){
        int move = field[winseq[i][0]];
        bool win = true;
        if (move < 1){
            continue;
        }

        for (int index : winseq[i]){
            if (field[index] == 0){
                win = false;
                break;
            }
            if (field[index] != move){
                win = false;
                std::vector<int> temp = winseq[winseq.size()-1];
                winseq[winseq.size()-1] = winseq[i];
                winseq[i] = temp;
                winseq.pop_back();
                break;
            }
        }
        if (win){
            cur->payoff = 1-2*turn;
            return cur;
        }
    }

    //check if the game is over
    if (winseq.empty()){
        cur->payoff = 0;
        return cur;
    }

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
    
    for (int i = 0; i < size; i++){
        if (field[i] != 0){
            continue;
        }

        field[i] = 2-turn;
        
        State* curChild = computeCase(size, winseq, field, !turn, verbose, cur);
        curPayoff = curChild->payoff;
        
        if (!verbose){
            delete curChild;
        } else {
            cur->children.push_back(curChild);
        }

        //if a desired payoff is reached prune all other children of the current state, otherwise save the best payoff
        if (turn){
            if (curPayoff == 1){
                cur->payoff = 1;
                if (verbose){
                    i++;
                    for (i; i < size; i++){
                        if (field[i] != 0){
                            continue;
                        }
                        cur->children.push_back(new State(cur, 2, field));
                    }
                }
                return cur;
            } else {
                payoff = std::max(payoff, curPayoff);
            }
        } else {
            if (curPayoff == -1){
                cur->payoff = -1;
                if (verbose){
                    i++;
                    for (i; i < size; i++){
                        if (field[i] != 0){
                            continue;
                        }
                        cur->children.push_back(new State(cur, 2, field));
                    }
                }
                return cur;
            }else{
                payoff = std::min(payoff, curPayoff);
            }
        }
        
        field[i] = 0;
    }
    cur->payoff = payoff;
    return cur;
}

int GameTree::computeGameTree(const GameSpec& spec) {
    //initialize the playing field
    std::vector<int> field;
    for (int i = 0; i < spec.getSize(); i++){
        if (spec.isBlocked(i)){
            field.push_back(-1);
        } else {
            field.push_back(0);
        }
    }
    //compute the pruned game tree
    State* res = computeCase(spec.getSize(), spec.getWinSeq() , field, true);
    int payoff = res->payoff;
    delete res;
    return payoff;
}

void GameTree::printGameTree(const GameSpec& spec) {
    //initialize the playing field
    std::vector<int> field;
    for (int i = 0; i < spec.getSize(); i++){
        if (spec.isBlocked(i)){
            field.push_back(-1);
        } else {
            field.push_back(0);
        }
    }
    
    //compute the pruned tree
    State* res = computeCase(spec.getSize(), spec.getWinSeq() , field, true, true);
    
    //breadth first traversal of the tree and append each states string to the print string
    int id = 0;
    std::queue<State*> q;
    q.push(res);
    State* cur;

    //the resulting text variable
    std::string print = "";

    //index each node
    while (!q.empty())
    {
        cur = q.front();
        cur->id = id++;
        q.pop();
        for (State *child: cur->children){
            q.push(child);
        }
    }
    
    q.push(res);
    
    //components of the string for all strings
    std::string parentId = "nil ";
    std::string childrenId = "";
    std::string payOffString = "";

    while (!q.empty()){
        
        cur = q.front();
        q.pop();
        
        if (cur->parent){
            parentId="s"+std::to_string(cur->parent->id) + " ";
        }
        
        for (int i = 0; i < cur->children.size(); i++){
            childrenId += "s" + std::to_string(cur->children[i]->id) + " ";
        }
        
        //if there are no children it is a terminal node
        if (childrenId.size()){
            childrenId = " children={" + childrenId + "} ";
        } else {
            if (cur->payoff == 1){
                childrenId = "WIN ";
            } else if (cur->payoff == -1){
                childrenId = "LOSE ";
            } else if (cur->payoff == 0) {
                childrenId = "DRAW ";
            }
        }
        
        //if payoff is 2 it means it is pruned
        if (cur->payoff == 2){
            payOffString = "pruned";
        } else {
            payOffString = "payoff=" + std::to_string(cur->payoff);
        }
        
        print += "--- s" + std::to_string(cur->id) + " parent=" + parentId + childrenId + payOffString + "\n";
        
        //clear variable
        childrenId = "";
        
        for (int i = 0; i < spec.getSizeY(); i++){
            for (int j = 0; j < spec.getSizeX(); j++){
                if (cur->field[spec.getMoveId(j,i)] <= 0){
                    print += ".";
                } else if (cur->field[spec.getMoveId(j,i)] == 1){
                    print += "O";
                } else {
                    print += "X";
                }
            }
            print += "\n";
        }

        for (State *child: cur->children){
            q.push(child);
        }
    }
    // std::FILE* out = std::freopen("out.txt", "w", stdout);
    std::cout<<print<<std::endl;
    delete res;
}

//compute the complete tree so it is more tailored for playing
struct State* completeTree(int size, std::vector<std::vector<int>> winseq, std::vector<int> field, bool turn){
    struct State* cur = new State();
    cur->field = field;

    //check if anybody won
    for (int i = winseq.size()-1; i >= 0; i--){
        int move = field[winseq[i][0]];
        bool win = true;
        if (move < 1){
            continue;
        }

        for (int index : winseq[i]){
            if (field[index] != move){
                win = false;
                break;
            }
        }
        if (win){
            cur->payoff = 1-2*turn;
            return cur;
        }
    }

    //check if the game is over
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

    for (int i = 0; i < size; i++){
        if (field[i] != 0){
            continue;
        }
        
        field[i] = 2-turn;

        State* curChild = completeTree(size, winseq, field, !turn);
        
        cur->children.push_back(curChild);
        
        if (turn){
            if (curChild->payoff > payoff){
                cur->bestMove = i;
                cur->bestMoveIndex = cur->children.size()-1;
                payoff = curChild->payoff;
            }
        } else {
            if (curChild->payoff < payoff){
                cur->bestMove = i;
                cur->bestMoveIndex = cur->children.size()-1;
                payoff = curChild->payoff;
            }
        }
        
        field[i] = 0;
    }
    cur->payoff = payoff;
    return cur;
}

void GameTree::playAsFirstPlayer(const GameSpec& spec) {
    //initialize the playing field
    std::vector<int> field;
    for (int i = 0; i < spec.getSize(); i++){
        if (spec.isBlocked(i)){
            field.push_back(-1);
        } else {
            field.push_back(0);
        }
    }

    //compute the whole tree
    State* root = completeTree(spec.getSize(), spec.getWinSeq(), field, true);
    
    //current state of the game
    State* cur = root;
    
    int bestMove = 0;
    int opponentMove;
    
    //make best moves according to the computed tree, then read in the opponents move 
    while(!cur->children.empty()){
        std::cout<<cur->bestMove<<std::endl;
        cur = cur->children[cur->bestMoveIndex];
        std::cin>>opponentMove;
        for(State* child: cur->children){
            if (child->field[opponentMove] == 2){
                cur = child;
                break;
            }
        }
    }
}


void GameTree::playAsSecondPlayer(const GameSpec& spec) {
    //initialize the playing field
    std::vector<int> field;
    for (int i = 0; i < spec.getSize(); i++){
        if (spec.isBlocked(i)){
            field.push_back(-1);
        } else {
            field.push_back(0);
        }
    }

    //compute the whole tree
    State* root = completeTree(spec.getSize(), spec.getWinSeq(), field, true);
    
    //current state of the game
    State* cur = root;

    int bestMove = 0;
    int opponentMove;

    //read in the opponents move and do the best move from the computed tree
    while(!cur->children.empty()){
        std::cin>>opponentMove;
        for(State* child: cur->children){
            if (child->field[opponentMove] == 1){
                cur = child;
                break;
            }
        }
        std::cout<<cur->bestMove<<std::endl;
        cur = cur->children[cur->bestMoveIndex];
    }
}

