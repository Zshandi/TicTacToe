#include "utility.h"

#include <SDL.h>

int utility::getGridIndexForScreenPos(int x, int y){
    return x/GRID_PIXEL_SIZE + y/GRID_PIXEL_SIZE*GRID_SIZE;
}

void utility::getScreenPosForGridIndex(int index, int& x, int& y){
    x = (index%GRID_SIZE) * GRID_PIXEL_SIZE;
    y = (index/GRID_SIZE) * GRID_PIXEL_SIZE;
}

void utility::getMousePosition(int& x, int& y){
    SDL_GetMouseState(&x, &y);
}

int utility::getGridIndexForMouse(){
    int x=0, y=0;
    getMousePosition(x,y);
    return getGridIndexForScreenPos(x,y);
}
