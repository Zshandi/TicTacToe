#include "utility.h"

#include <SDL.h>

int getGridIndexForScreenPos(int x, int y){
    return x/GRID_PIXEL_SIZE + y/GRID_PIXEL_SIZE*GRID_SIZE;
}

void getScreenPosForGridIndex(int index, int& x, int& y){
    x = (index%GRID_SIZE) * GRID_PIXEL_SIZE;
    y = (index/GRID_SIZE) * GRID_PIXEL_SIZE;
}

void getMousePosition(int& x, int& y){
    SDL_GetMouseState(&x, &y);
}

int getGridIndexForMouse(){
    int x=0, y=0;
    getMousePosition(x,y);
    return getGridIndexForScreenPos(x,y);
}
