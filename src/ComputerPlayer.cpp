#include "ComputerPlayer.h"
#include <random>
#include <chrono>
#include "RowChecker.h"

#ifdef BUILD_DEBUG
#include <iostream>
#endif // BUILD_DEBUG
using namespace std;

int randomMove(int max_v){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);
    uniform_int_distribution<int> distribution(0, max_v);
    return distribution(engine);
}

int RandomAI::getNextMove(int* grid){
    int i = randomMove(8);
    while(grid[i] != 0)
    {
        i = randomMove(8);
    }
    return i;
}

int SimpleAI::getNextMove(int* grid)
{
#ifdef BUILD_DEBUG
    cout << "Selecting move";
#endif // BUILD_DEBUG
    for(int lvl = 2; lvl >= 0; lvl--)
    {
#ifdef BUILD_DEBUG
        cout << "Level " << lvl << ":\n";
#endif // BUILD_DEBUG
        Container<row> rows = checkRows(grid, CPU, lvl);
        for(int i = 0; i < rows.getSize(); i++)
        {
            row current = *rows.get(i);
#ifdef BUILD_DEBUG
            cout << "  Row " << i << ": " << current.num << "(";
            for(int j = 0; j < current.num; j++)
            {
                cout << current.value[j] << ' ';
            }
            cout << ")\n";
#endif // BUILD_DEBUG
        }
        if(rows.getSize() != 0)
        {
            int row_i = randomMove(rows.getSize()-1);
#ifdef BUILD_DEBUG
            cout << "Value of row_i: " << row_i << '\n';
#endif // BUILD_DEBUG
            row choice = *(rows.get(row_i));
            int i = randomMove(choice.num-1);
#ifdef BUILD_DEBUG
            cout << "Value of i: " << i << '\n';
            cout << "Selected move" << choice.value[i] << '\n';
#endif // BUILD_DEBUG
            return choice.value[i];
        }
        if(lvl == 2)
        {
            rows = checkRows(grid, PL, lvl);
            if(rows.getSize() != 0)
            {
                int row_i = randomMove(rows.getSize()-1);
                row choice = *(rows.get(row_i));
                int i = randomMove(choice.num-1);
                return choice.value[i];
            }
        }
    }
#ifdef BUILD_DEBUG
    cout << "Found no moves, selecting random\n";
#endif // BUILD_DEBUG

    int i = randomMove(8);
    while(grid[i] != 0)
    {
        i = randomMove(8);
    }
#ifdef BUILD_DEBUG
    cout << "Randomly selected: " << i << '\n';
#endif // BUILD_DEBUG
    return i;
}
