#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#define GRID_PIXEL_SIZE 200
#define GRID_INDEX_WIDTH 3
#define GRID_INDEX_HEIGHT 3
#define GRID_SIZE 9

#define GRID_POS_NONE -1
#define GRID_POS_MIN 0
#define GRID_POS_MAX GRID_SIZE-1

namespace utility{
    int getGridIndexForScreenPos(int x, int y);

    void getScreenPosForGridIndex(int index, int& x, int& y);

    void getMousePosition(int& x, int& y);

    int getGridIndexForMouse();
}

#endif // UTILITY_H_INCLUDED
