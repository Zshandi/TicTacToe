
//#include <random>
#include <chrono>
#include <iostream>
#include "RandomComputerPlayer.h"

using namespace std;


RandomComputerPlayer::RandomComputerPlayer(){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    engine = default_random_engine(seed);
}

// Get the next move with the given data
// currentGridIndex should be set to the index to be chosen
// If waitForEvent is set to true, then getNextMoveOnEvent will be called
//  the next time an event is fired
//  This also means that currentGridIndex will be highlighted, not selected
// lastOponentGridIndex is the index of the last oponent move
// mouseGridIndex is the index of the grid space the mouse is currently over
void RandomComputerPlayer::getNextMove(int& currentGridIndex, bool& waitForEvent,
                         int lastOponentGridIndex, int mouseGridIndex){
    processOponentMove(lastOponentGridIndex);
    grid.debugDisplay();

    bool hasObviousMove = false;
    getObviousMove(hasObviousMove, currentGridIndex);
    if(!hasObviousMove){
        currentGridIndex = getRandomMove();
    }
    finalizeMove(currentGridIndex, waitForEvent);
}


int RandomComputerPlayer::getRandomMove(){
    int index = GRID_POS_NONE;
    do{
        index = getRandomNumber(GRID_POS_MIN, GRID_POS_MAX);
    }while( grid[index] != 0 );
    return index;
}

int RandomComputerPlayer::getRandomNumber(int minVal, int maxVal){
    uniform_int_distribution<int> distribution(minVal, maxVal);
    return distribution(engine);
}
