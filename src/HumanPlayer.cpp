#include "HumanPlayer.h"
#include "CApp.h"
#include "utility.h"
#include <iostream>

HumanPlayer::~HumanPlayer(){}

// Initialize the current game with the given grid and oponent
void HumanPlayer::initGame(Player* oponent, bool oponentHasFirstMove){}

// Get the next move with the given data
// currentGridIndex should be set to the index to be chosen
// If waitForEvent is set to true, then getNextMoveOnEvent will be called
//  the next time an event is fired
//  This also means that currentGridIndex will be highlighted, not selected
// lastOponentGridIndex is the index of the last oponent move
// mouseGridIndex is the index of the grid space the mouse is currently over
void HumanPlayer::getNextMove(int& currentGridIndex, bool& waitForEvent,
                         int lastOponentGridIndex, int mouseGridIndex){
    currentGridIndex = mouseGridIndex;
    mouseDownIndex = GRID_POS_NONE;
    waitForEvent = true;
}

// Get the next move with the given data
// currentGridIndex should be set to the index to be chosen
// If waitForEvent is set to true, then getNextMoveOnEvent will be called
//  the next time an event is fired
//  This also means that currentGridIndex will be highlighted, not selected
// lastOponentGridIndex is the index of the last oponent move
// mouseGridIndex is the index of the grid space the mouse is currently over
// event is the SDL_Event which triggered this call
void HumanPlayer::getNextMoveOnEvent(int& currentGridIndex, bool& waitForEvent,
                         int lastOponentGridIndex, int mouseGridIndex,
                         SDL_Event* event){
    currentGridIndex = mouseGridIndex;
    if(event->button.button != SDL_BUTTON_LEFT){
        waitForEvent = true;
        return;
    }
    switch(event->type){
    case SDL_MOUSEBUTTONUP:
        if(mouseDownIndex == GRID_POS_NONE){
            waitForEvent = true;
        }else if(mouseDownIndex == mouseGridIndex){
            waitForEvent = false;
        }else{
            mouseDownIndex = GRID_POS_NONE;
            waitForEvent = true;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        mouseDownIndex = mouseGridIndex;
        waitForEvent = true;
        break;
    default:
        waitForEvent = true;
    }
}

bool HumanPlayer::isHuman(){
    return true;
}
