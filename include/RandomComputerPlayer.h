#ifndef RANDOMCOMPUTERPLAYER_H
#define RANDOMCOMPUTERPLAYER_H

#include <random>
#include "ComputerPlayer.h"


class RandomComputerPlayer : public ComputerPlayer{
public:

    RandomComputerPlayer();

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
    int getRandomMove();

    int getRandomNumber(int minVal, int maxVal);
private:
    std::default_random_engine engine;
};

#endif // RANDOMCOMPUTERPLAYER_H
