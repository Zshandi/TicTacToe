#include "ComputerPlayer.h"
#include <random>
#include <chrono>
#include "RowChecker.h"

#ifdef BUILD_DEBUG
#include <iostream>
#endif // BUILD_DEBUG
using namespace std;


// Initialize the current game with the given oponent, and whether they go first
void ComputerPlayer::initGame(Player* oponent, bool oponentHasFirstMove){
    this->oponent = oponent;
    this->isOponentFirst = oponentHasFirstMove;
    grid.setAllValues();
}

// Get the next move with the given data
// currentGridIndex should be set to the index to be chosen
// If waitForEvent is set to true, then getNextMoveOnEvent will be called
//  the next time an event is fired
//  This also means that currentGridIndex will be highlighted, not selected
// lastOponentGridIndex is the index of the last oponent move
// mouseGridIndex is the index of the grid space the mouse is currently over
// event is the SDL_Event which triggered this call
void ComputerPlayer::getNextMoveOnEvent(int& currentGridIndex, bool& waitForEvent,
                         int lastOponentGridIndex, int mouseGridIndex,
                         SDL_Event* event){
    if(currentGridIndex == GRID_POS_NONE){
        // This shouldn't happen
        getNextMove(currentGridIndex, waitForEvent, lastOponentGridIndex, mouseGridIndex);
    }else if(event->type == SDL_USEREVENT){
        grid.set(currentGridIndex, GRID_VAL_SELF);
        waitForEvent = false;
        timerID = 0;
    }else{
        waitForEvent = true;
    }
 }

// Called when the user executes an "undo" so as to reset the state of the "player"
void ComputerPlayer::undo(int lastTurn){
    cout << "Undo move at " << lastTurn << " with value " << grid[lastTurn] << endl;
    grid.set(lastTurn, 0);
    if(timerID != 0){
        SDL_RemoveTimer(timerID);
        timerID = 0;
    }
}

bool ComputerPlayer::isHuman(){ return false; }

// Adds the oponent move to the current grid data
void ComputerPlayer::processOponentMove(int gridIndex){
    if(gridIndex == GRID_POS_NONE) return;
    grid.set(gridIndex, GRID_VAL_OPONENT);
}

// Determines if there is an obvious course of action to take
//  (Either to win, or to prevent oponent from winning)
// If there is, sets hasObviousMove to true and obviousMoveIndex to the grid index
//  also returns whether or not it will be a winning move
bool ComputerPlayer::getObviousMove(bool& hasObviousMove, int &obviousMoveIndex){

    // First, check if there is only one space
    int openSpace = GRID_POS_NONE;
    for(int i = 0; i < grid.getSize(); i++){
        if(grid[i] == 0){
            if(openSpace == GRID_POS_NONE){
                openSpace = i;
            }else{
                openSpace = GRID_POS_NONE;
                break;
            }
        }
    }
    if(openSpace != GRID_POS_NONE){
        hasObviousMove = true;
        obviousMoveIndex = openSpace;
        return false;
    }

    // Then check if oponent or self have winning move
    //  Self takes priority
    const vector<Grid::Line>& lines = grid.getAllLines();

    int oponentWin = GRID_POS_NONE;
    for(int i = 0; i < lines.size(); i++){
        if(lines[i].getValueCount(0) != 1) continue;

        if(lines[i].getValueCount(GRID_VAL_SELF) == 2){
            for(int j = 0; j < grid.getDimension(); j++){
                if( lines[i][j] == 0 ){
                    obviousMoveIndex = lines[i].indexOf(j);
                    hasObviousMove = true;
                    return true;
                }
            }
        }else if(lines[i].getValueCount(GRID_VAL_OPONENT) == 2){
            for(int j = 0; j < grid.getDimension(); j++){
                if( lines[i][j] == 0 ){
                    oponentWin = lines[i].indexOf(j);
                }
            }
        }
    }

    if(oponentWin != GRID_POS_NONE){
        // There was a space where oponent could win
        obviousMoveIndex = oponentWin;
        hasObviousMove = true;
        return false;
    }
    // No obvious move found
    hasObviousMove = false;
    return false;
}

Uint32 ComputerPlayer::timerCallback(Uint32 interval, void *param){
    SDL_Event event;

    event.type = SDL_USEREVENT;
    event.user = SDL_UserEvent();

    event.user.type = SDL_USEREVENT;
    event.user.code = 0;
    event.user.data1 = NULL;
    event.user.data2 = NULL;

    SDL_PushEvent(&event);
    return 0;
}

// Add timer delay if oponent is computer
void ComputerPlayer::finalizeMove(int& moveIndex, bool &waitForEvent){
    if( oponent->isComputer() ){
        timerID = SDL_AddTimer(1000 /*1 second delay*/, timerCallback, 0);
        waitForEvent = true;
    }else{
        grid.set(moveIndex, GRID_VAL_SELF);
        waitForEvent = false;
    }
}
