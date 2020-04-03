#ifndef CAPP_H_INCLUDED
#define CAPP_H_INCLUDED

#include <vector>
#include <SDL.h>

#include "Texture.h"
#include "ComputerPlayer.h"
#include "RowChecker.h"
#include "HumanPlayer.h"
#include "utility.h"

class CApp{
public:
    struct PlayerData{
        Player* player;
        Texture* graphic;
    };

    // Starts a new game
    void newGame();
    // Checks the current state for win conditions
    void checkWin();
    // Finishes up the current turn
    void finishCurrentTurn();
    // Starts the next turn
    void startNextTurn();
    // Gets the next turn (player 1 or player 2) based on the current turn
    int switchTurn(int turn);

    // Undoes the last move (If playing against computer, last 2 moves
    void undo();
    // Redoes the last move that was undone
    void redo();
    // Clears history to prevent "redoing" after making a moved
    //  (doesn't actually check anything, I need to rename it)
    void checkHistory();

    // Called to start the apps execuition
    int OnExecute();

    // Called to initialize the app
    bool OnInit();
    // Event handler for app
    void OnEvent(SDL_Event* event);
    // Main loop for execution
    void OnLoop();
    // Render the current state
    void OnRender();
    // Cleanup after execution is complete
    void OnCleanup();

private:
    // Whether the app should close
    bool running = true;

    // SDL resource variables
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Texture graphic_grid;
    Texture graphic_x;
    Texture graphic_o;

    // The current grid states
    int grid[GRID_SIZE];

    // The state of the current game moves
    //int turn;
    int currentGridIndex;
    int previousGridIndex;
    bool shouldWaitForEvent;

    int currentPlayer;

    // History state
    vector<int> history;

    // Various settings
    //bool computer = false;
    //bool reversed = false;
    bool reversedGraphic = false;

    // Whether the game has finished
    bool gameOver = false;

    // Whether the render function should be called
    bool render = true;

    // The players
    PlayerData* playerData[2];

};


#endif // CAPP_H_INCLUDED
