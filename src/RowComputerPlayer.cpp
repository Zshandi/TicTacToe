#include "RowComputerPlayer.h"

#include <vector>
#include <iostream>

using namespace std;

RowComputerPlayer::RowComputerPlayer() : RandomComputerPlayer() {}

// Get the next move with the given data
// currentGridIndex should be set to the index to be chosen
// If waitForEvent is set to true, then getNextMoveOnEvent will be called
//  the next time an event is fired
//  This also means that currentGridIndex will be highlighted, not selected
// lastOponentGridIndex is the index of the last oponent move
// mouseGridIndex is the index of the grid space the mouse is currently over
void RowComputerPlayer::getNextMove(int& currentGridIndex, bool& waitForEvent,
                         int lastOponentGridIndex, int mouseGridIndex){
    processOponentMove(lastOponentGridIndex);

    bool hasObviousMove = false;
    getObviousMove(hasObviousMove, currentGridIndex);
    if(!hasObviousMove){
        currentGridIndex = getRowMove();
    }
    finalizeMove(currentGridIndex, waitForEvent);
}

int RowComputerPlayer::getRowMove(){

    // Either empty rows, or rows with at least one spot are considered
    //  Rows with one spot are preferred, since it's closer to winning

    // Keep track of which spaces are open in empty rows,
    //  and which are open in partial rows
    vector<bool> emptyRows(GRID_SIZE, false);
    vector<bool> partialRows(GRID_SIZE, false);
    // Keep track of whether each one has a true value
    bool hasPartialRow = false;
    bool hasEmptyRow = false;

    // Get all lines
    const vector<Grid::Line>& lines = grid.getAllLines();
    // Go through each line
    for(int i = 0; i < lines.size(); i++){
        // If there are any opponent values,
        //  don't consider this line at all
        if(lines[i].getValueCount(GRID_VAL_OPONENT) > 0) continue;

        // Check if the line is empty
        if(lines[i].getValueCount(GRID_VAL_SELF) == 0){
            hasEmptyRow = true;
            // Get the indices, and set them to true
            for(int j = 0; j < grid.getDimension(); j++){
                int moveIndex = lines[i].indexOf(j);
                emptyRows[moveIndex] = true;
            }
        }else{ // The line is not empty, so has a spot taken
            hasPartialRow = true;
            // Get the indices that are empty, and set them to true
            for(int j = 0; j < grid.getDimension(); j++){
                if( lines[i][j] == 0 ){
                    int moveIndex = lines[i].indexOf(j);
                    partialRows[moveIndex] = true;
                }
            }
        }
    }

    // Which set of empty spaces to choose from
    vector<bool>* selection;
    // If there happens to not be any rows without opponent,
    //  then choose a random move, as none will win
    if(!hasPartialRow && !hasEmptyRow){
        return getRandomMove();
    // Otherwise, check which to choose from,
    //  favoring partial rows if there are any
    }else if(hasPartialRow){
        selection = &partialRows;
    }else{
        selection = &emptyRows;
    }

    // Randomly choose a space from the selection
    int index = GRID_POS_NONE;
    do{
        index = getRandomNumber(GRID_POS_MIN, GRID_POS_MAX);
    }while( (*selection)[index] == false );
    return index;
}
