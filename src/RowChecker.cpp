#include "RowChecker.h"

int rows[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

row::row(){
    num = 0;
}
row::row(int first){
    num = 1;
    value[0] = first;
}
void row::add(int i){
    if(num >= 3) return;
    if(num < 0) num = 0;
    value[num] = i;
    num++;
}
Container<row> checkRows(int* grid, int player, int level)
{
    Container<row> result;
    for(int* combo : rows)
    {
        row current;
        for(int i = 0; i < 3; i++)
        {
            int value = grid[combo[i]];
            if(value == 0)
            {
                current.add(combo[i]);
            }
            else if(value != player)
            {
                current.num = -1;
                break;
            }
        }
        if(3-current.num >= level && current.num != -1)
        {
            result.add(current);
        }
    }
    return result;
}
