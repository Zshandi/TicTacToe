#ifndef ROWCHECKER_H_INCLUDED
#define ROWCHECKER_H_INCLUDED
#include "Container.h"
#include <iostream>
#include "Grid.h"

struct row{
    int value[3];
    int num;
    row();
    row(int first);
    void add(int i);
};
Container<row> checkRows(Grid& grid, int player, int level);

#endif // ROWCHECKER_H_INCLUDED
