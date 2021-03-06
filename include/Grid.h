#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>

class Grid{
public:
    class Line{
        int index;
        int offset;
        int* values;

        int* valueCount;
    public:
        Line(int index=0, int offset=0, int* values=0, int maxValue = 2);
        int operator [] (int lineIndex) const;
        int indexOf(int lineIndex) const;
        int getValueCount(int value) const;
        void updateValueCount(int oldValue, int newValue);
        void resetValueCount(int value, int dimension, int maxValue = 2);
    };

    // Initialize grid with specified initial values and dimension
    Grid(int init = 0, int dimension = 3);

    // Sets all values to specified value
    void setAllValues(int val = 0);
    // Sets the given index to the given value
    void set(int index, int value);

    // Get value at specified index
    int operator [] (int index);
    // Get value at specified x/y coordinate
    int operator () (int x, int y);

    // Get the dimension of grid
    int getDimension();
    // Get size of grid (dimension * dimension)
    int getSize();

    // Get line associated with specified start x/y and orientation
    // orientation can be one of: ROW, COL, DIAG_RIGHT, DIAG_LEFT
    const int ROW = 1, COL = 3, DIAG_RIGHT = 4, DIAG_LEFT = 2;
    Line getLineFor(int x, int y, int orient);

    // Gets all Lines for this grid
    const std::vector<Line>& getAllLines();

    // Debug display
    void debugDisplay();

private:
    public: // TEMP
    int * values;
    private: // TEMP
    // An array of vectors, each of which contains
    //  all the lines which correspond to a grid index
    std::vector<int>* valueLines;
    int size;
    int dimension;

    std::vector<Line> lines;

    int xyToIndex(int x, int y);
    void indexToXy(int index, int& x, int& y);

    // Initializes the lines for this grid, called once
    void initAllLines();
};

#endif // GRID_H_INCLUDED
