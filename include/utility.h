#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#define GRID_PIXEL_SIZE 200
#define GRID_SIZE 3

int getGridIndexForScreenPos(int x, int y);

void getScreenPosForGridIndex(int index, int& x, int& y);

void getMousePosition(int& x, int& y);

int getGridIndexForMouse();

#endif // UTILITY_H_INCLUDED
