#ifndef COMPUTERPLAYER_H_INCLUDED
#define COMPUTERPLAYER_H_INCLUDED
#include <random>
#include <chrono>
#include "RowChecker.h"
#include <iostream>
using namespace std;

#define PL 1
#define CPU 2

class AI{
public:
    virtual int getNextMove(int* grid) = 0;
};


int randomMove(int max_v){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);
    uniform_int_distribution<int> distribution(0, max_v);
    return distribution(engine);
}

class RandomAI : public AI{
public:
    int getNextMove(int* grid){
        int i = randomMove(8);
        while(grid[i] != 0){
            i = randomMove(8);
        }
        return i;
    }
};

class SimpleAI : public AI{
public:
    int getNextMove(int* grid){
        cout << "Selecting move";
        for(int lvl = 2; lvl >= 0; lvl--){
            cout << "Level " << lvl << ":\n";
            Container<row> rows = checkRows(grid, CPU, lvl);
            for(int i = 0; i < rows.getSize(); i++){
                row current = *rows.get(i);
                cout << "  Row " << i << ": " << current.num << "(";
                for(int j = 0; j < current.num; j++){
                    cout << current.value[j] << ' ';
                }
                cout << ")\n";
            }
            if(rows.getSize() != 0){
                int row_i = randomMove(rows.getSize()-1);
                cout << "Value of row_i: " << row_i << '\n';
                row choice = *(rows.get(row_i));
                int i = randomMove(choice.num-1);
                cout << "Value of i: " << i << '\n';
                cout << "Selected move" << choice.value[i] << '\n';
                return choice.value[i];
            }
            if(lvl == 2){
                rows = checkRows(grid, PL, lvl);
                if(rows.getSize() != 0){
                    int row_i = randomMove(rows.getSize()-1);
                    row choice = *(rows.get(row_i));
                    int i = randomMove(choice.num-1);
                    return choice.value[i];
                }
            }
        }
        cout << "Found no moves, selecting random\n";

        int i = randomMove(8);
        while(grid[i] != 0){
            i = randomMove(8);
        }
        cout << "Randomly selected: " << i << '\n';
        return i;
    }
};


#endif // COMPUTERPLAYER_H_INCLUDED
