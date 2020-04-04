#ifndef COMPUTERPLAYER_H_INCLUDED
#define COMPUTERPLAYER_H_INCLUDED

#include "Grid.h"
#include "Player.h"
#include "utility.h"

#define GRID_VAL_SELF 1
#define GRID_VAL_OPONENT 2



class ComputerPlayer : public Player{
public:

    // Initialize the current game with the given oponent, and whether they go first
    virtual void initGame(Player* oponent, bool oponentHasFirstMove);

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

    // Called when the user executes an "undo" so as to reset the state of the "player"
    virtual void undo(int lastTurn);

    virtual bool isHuman();

    static Uint32 timerCallback(Uint32 interval, void *param);
protected:
    Grid grid;
    bool isOponentFirst;
    Player* oponent;

    // Adds the oponent move to the current grid data
    void processOponentMove(int gridIndex);

    // Determines if there is an obvious course of action to take
    //  (Either to win, or to prevent oponent from winning)
    // If there is, sets hasObviousMove to true and obviousMoveIndex to the grid index
    //  also returns whether or not it will be a winning move
    bool getObviousMove(bool& hasObviousMove, int &obviousMoveIndex);

    // Add timer delay if oponent is computer
    void finalizeMove(int& moveIndex, bool &waitForEvent);
private:
    SDL_TimerID timerID = 0;
};

class RowComputerPlayer : public ComputerPlayer{
public:

    // Get the next move with the given data
    // currentGridIndex should be set to the index to be chosen
    // If waitForEvent is set to true, then getNextMoveOnEvent will be called
    //  the next time an event is fired
    //  This also means that currentGridIndex will be highlighted, not selected
    // lastOponentGridIndex is the index of the last oponent move
    // mouseGridIndex is the index of the grid space the mouse is currently over
    virtual void getNextMove(int& currentGridIndex, bool& waitForEvent,
                             int lastOponentGridIndex, int mouseGridIndex);

protected:
};

#define PL 2
#define CPU 1

class AI{
public:
    virtual int getNextMove(int* grid) = 0;
};


int randomMove(int max_v);

class RandomAI : public AI{
public:
    int getNextMove(int* grid);
};

class SimpleAI : public AI{
public:
    int getNextMove(int* grid);
};

/*class SimpleComputerPlayer : public Player{

};*/


#endif // COMPUTERPLAYER_H_INCLUDED
