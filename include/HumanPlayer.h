#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class CApp;


class HumanPlayer : public Player{
public:
    virtual ~HumanPlayer();

    // Initialize the current game with the given oponent, and whether they go first
    virtual void initGame(Player* oponent, bool oponentHasFirstMove);

    // Get the next move with the given data
    // currentGridIndex should be set to the index to be chosen
    // If waitForEvent is set to true, then getNextMoveOnEvent will be called
    //  the next time an event is fired
    //  This also means that currentGridIndex will be highlighted, not selected
    // lastOponentGridIndex is the index of the last oponent move
    // mouseGridIndex is the index of the grid space the mouse is currently over
    virtual void getNextMove(int& currentGridIndex, bool& waitForEvent,
                             int lastOponentGridIndex, int mouseGridIndex);

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
                             SDL_Event* event);

    virtual bool isHuman();
protected:
    int mouseDownIndex;
private:
};

#endif // HUMANPLAYER_H
