
#include "Grid.h"

#include <vector>

#include <iostream>
using namespace std;

int Grid::xyToIndex(int x, int y){
    return x + y*dimension;
}
void Grid::indexToXy(int index, int& x, int& y){
    x = index%dimension;
    y = index/dimension;
}

Grid::Line::Line(int index, int offset, int* values, int maxValue) :
    index(index), offset(offset), values(values){
    valueCount = new int[maxValue+1];
    for(int i = 0; i <= maxValue; i++){
        valueCount[i] = 0;
    }
}

int Grid::Line::operator [] (int lineIndex) const{
    return values[ indexOf(lineIndex) ];
}
int Grid::Line::indexOf(int lineIndex) const{
    return index + offset*lineIndex;
}

int Grid::Line::getValueCount(int value) const{
    return valueCount[value];
}

void Grid::Line::updateValueCount(int oldValue, int newValue){
    if(oldValue != newValue){
        valueCount[ oldValue ] --;
        valueCount[ newValue ] ++;
    }
}

void Grid::Line::resetValueCount(int value, int dimension, int maxValue){
    for(int i = 0; i <= maxValue; i++){
        valueCount[i] = 0;
    }
    valueCount[value] = dimension;
}

// Initialize grid with specified initial values and dimension
Grid::Grid(int init, int dimension){
    this->dimension = dimension;
    size = dimension*dimension;
    values = new int[size];
    initAllLines();

    // Init the value lines
    valueLines = new vector<int>[size];
    for(int i = 0; i < lines.size(); i++){
        for(int j = 0; j < dimension; j++){
            int index = lines[i].indexOf(j);
            valueLines[index].push_back(i);
        }
    }
    setAllValues(init);
}
// Sets all values to specified value
void Grid::setAllValues(int val){
    for(int i = 0; i < size; i++){
        values[i] = val;
    }
    for(int i = 0; i < lines.size(); i++){
        lines[i].resetValueCount(val, dimension);
    }
}

// Sets the given index to the given value
void Grid::set(int index, int value){
    if(index >= 0 && index < size){
        for(int i = 0; i < valueLines[index].size(); i++){
            int lineIndex = valueLines[index][i];
            lines[lineIndex].updateValueCount(values[index], value);
        }
        values[index] = value;
    }
}

// Get value at specified index (between 0 and size)
int Grid::operator [] (int index){
    return values[index];
}
// Get value at specified x/y coordinate
int Grid::operator () (int x, int y){
    return values[ xyToIndex(x,y) ];
}

// Get the dimension of grid
int Grid::getDimension(){
    return dimension;
}
// Get size of grid (dimension * dimension)
int Grid::getSize(){
    return size;
}

// Get line associated with specified start x/y and orientation
// orientation can be one of: ROW, COL, DIAG_RIGHT, DIAG_LEFT
Grid::Line Grid::getLineFor(int x, int y, int orient){
    return Line(xyToIndex(x,y), orient, values);
}

// Initializes the lines for this grid, called once
void Grid::initAllLines(){
    // rows, cols, and 2 diagonals
    //int numLines = 2*dimension + 2;

    int i;
    for(i = 0; i < dimension; i++){
        // rows
        lines.push_back( getLineFor(0, i, ROW) );
        // cols
        lines.push_back( getLineFor(i, 0, COL) );
    }
    // diagonals
    lines.push_back( getLineFor(0, 0, DIAG_RIGHT) );
    lines.push_back( getLineFor(dimension-1, 0, DIAG_LEFT) );
}

// Gets all Lines for this grid
const vector<Grid::Line>& Grid::getAllLines(){
    return lines;
}


// Debug display
void Grid::debugDisplay(){
    cout << "Grid:" << endl;
    for(int i = 0; i < size; i++){
        cout << " " << values[i];
        if( (i+1)%dimension == 0 )
            cout << endl;
    }
    cout << "Lines:" << endl;
    for(int i = 0; i < lines.size(); i++){
        // Output the line data:
        cout << "( ";
        for(int j = 0; j < dimension; j++){
            cout << lines[i].indexOf(j) << " ";
        }
        cout << "): ";
        for(int j = 0; j <= 2; j++){
            cout << "valueCount["<<j<<"]"<<" = "<<lines[i].getValueCount(j)<<", ";
        }
        cout << endl;
    }
}
