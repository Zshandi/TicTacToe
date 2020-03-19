#ifndef CAPP_H_INCLUDED
#define CAPP_H_INCLUDED

#include <SDL.h>
#include "Textures.h"
#include "ImageUtility.h"
#include "ComputerPlayer.h"
#include "RowChecker.h"

int winCombos[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

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
    void newGame();
    void checkWin();
    int switchTurn(int turn);

    void undo();
    void redo();
    void checkHistory();

    void getPosition(int* i, int* x = 0, int* y = 0);

    int OnExecute();

    bool OnInit();
    void OnEvent(SDL_Event* event);
    void OnLoop();
    void OnRender();
    void OnCleanup();
};


#endif // CAPP_H_INCLUDED
