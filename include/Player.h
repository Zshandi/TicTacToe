#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Grid.h"
#include <SDL.h>

class Player{
public:
    virtual ~Player();

    // Initialize the current game with the given oponent, and whether they go first
    virtual void initGame(Player* oponent, bool oponentHasFirstMove) = 0;

    // Get the next move with the given data
    // currentGridIndex should be set to the index to be chosen
    // If waitForEvent is set to true, then getNextMoveOnEvent will be called
    //  the next time an event is fired
    //  This also means that currentGridIndex will be highlighted, not selected
    // lastOponentGridIndex is the index of the last oponent move
    // mouseGridIndex is the index of the grid space the mouse is currently over
    virtual void getNextMove(int& currentGridIndex, bool& waitForEvent,
                             int lastOponentGridIndex, int mouseGridIndex) = 0;

    // Get the next move with the given data
    // currentGridIndex should be set to the index to be chosen
    // If waitForEvent is set to true, then getNextMoveOnEvent will be called
    //  the next time an event is fired
    //  This also means that currentGridIndex will be highlighted, not selected
    // lastOponentGridIndex is the index of the last oponent move
    // mouseGridIndex is the index of the grid space the mouse is currently over
    // event is the SDL_Event which triggered this call
    virtual void getNextMoveOnEvent(int& currentGridIndex, bool& waitForEvent,
                             int lastOponentGridIndex, int mouseGridIndex,
                             SDL_Event* event) = 0;

    virtual bool isHuman() = 0;
    virtual bool isComputer();

};

#endif // PLAYER_H_INCLUDED
