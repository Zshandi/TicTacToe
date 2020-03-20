#ifndef CAPP_H_INCLUDED
#define CAPP_H_INCLUDED

#include <SDL.h>
#include "Texture.h"
#include "ComputerPlayer.h"
#include "RowChecker.h"

class CApp{
private:
    bool running = true;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Texture graphic_grid;
    Texture graphic_x;
    Texture graphic_o;

    int grid[9];
    int turn = 1;
    int mouse_pressed;
    int mouse_over = -1;

    int history[9];
    int moves = 0;

    bool computer = false;
    bool reversed = false;
    bool reversedGraphic = false;

    bool gameOver = false;

    bool render = true;
public:
    // Starts a new game
    void newGame();
    // Checks the current state for win conditions
    void checkWin();
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
};


#endif // CAPP_H_INCLUDED
