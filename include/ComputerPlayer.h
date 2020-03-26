#ifndef COMPUTERPLAYER_H_INCLUDED
#define COMPUTERPLAYER_H_INCLUDED

#include "Grid.h"

#define PL 1
#define CPU 2

class AI{
public:
    virtual int getNextMove(Grid& grid) = 0;
};


int randomMove(int max_v);

class RandomAI : public AI{
public:
    int getNextMove(Grid& grid);
};

class SimpleAI : public AI{
public:
    int getNextMove(Grid& grid);
};

/*class SimpleComputerPlayer : public Player{

};*/


#endif // COMPUTERPLAYER_H_INCLUDED
