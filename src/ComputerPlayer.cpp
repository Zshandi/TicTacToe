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
        grid[currentGridIndex] = GRID_VAL_SELF;
        waitForEvent = false;
        timerID = 0;
    }else{
        waitForEvent = true;
    }
 }

// Called when the user executes an "undo" so as to reset the state of the "player"
void ComputerPlayer::undo(int lastTurn){
    grid[lastTurn] = 0;
    if(timerID != 0){
        SDL_RemoveTimer(timerID);
        timerID = 0;
    }
}

bool ComputerPlayer::isHuman(){ return false; }

// Adds the oponent move to the current grid data
void ComputerPlayer::processOponentMove(int gridIndex){
    if(gridIndex == GRID_POS_NONE) return;
    grid[gridIndex] = GRID_VAL_OPONENT;
}

// Determines if there is an obvious course of action to take
//  (Either to win, or to prevent oponent from winning)
// If there is, sets hasObviousMove to true and obviousMoveIndex to the grid index
//  also returns whether or not it will be a winning move
bool ComputerPlayer::getObviousMove(bool& hasObviousMove, int &obviousMoveIndex){
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
        grid[moveIndex] = GRID_VAL_SELF;
        waitForEvent = false;
    }
}

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
    grid.debugDisplay();
    currentGridIndex = SimpleAI().getNextMove(grid.values);
    finalizeMove(currentGridIndex, waitForEvent);
}

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
