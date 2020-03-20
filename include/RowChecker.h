#ifndef ROWCHECKER_H_INCLUDED
#define ROWCHECKER_H_INCLUDED
#include "Container.h"
#include <iostream>

struct row{
    int value[3];
    int num;
    row();
    row(int first);
    void add(int i);
};
Container<row> checkRows(int* grid, int player, int level);

#endif // ROWCHECKER_H_INCLUDED
