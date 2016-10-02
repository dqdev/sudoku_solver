#ifndef SUDOKU_EMPTY_CELL
#define SUDOKU_EMPTY_CELL

#include <set>

namespace sudoku {

struct Cell {
    int row;
    int col;
    std::set<char> possibleValues;

    Cell(int r, int c, std::set<char>& vals) 
        : row(r), col(c), possibleValues(vals) {}
    
    Cell(const Cell& other) 
        : row(other.row), 
          col(other.col), 
          possibleValues(other.possibleValues) {}
};

} //namespace sudoku

#endif
