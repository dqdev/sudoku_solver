#include <solver.h>
#include <utils.h>
#include <empty_cell.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>

namespace sudoku {
namespace solver {

using namespace std;

namespace {

// Forward declaration
bool make_move(string& m,
               vector<Cell*>& emptyCells,
               vector<vector<Cell*> >& ref);

void
sort_cells(vector<Cell*>& c)
{
    sort(c.begin(), c.end(),
         [](const Cell* l, const Cell* r) -> bool {
            return l->possibleValues.size() > r->possibleValues.size();
         }
    );
}


void
make_safe_move(string& m,
               Cell* c,
               char val,
               vector<vector<Cell*> >& ref)
{
    if (c->possibleValues.find(val) == c->possibleValues.end()) {
        cout << "Value: " << val << " not a possible value in Cell ("
             << c->row << "," << c->col << ")" << endl;
        throw logic_error("Trying to set value not in cell");
    }

    int idx = c->row*9 + c->col;
    m[idx] = val;

    for (int i = 0; i < 9; i++) {
        //row
        if (ref[c->row][i] != NULL)
            ref[c->row][i]->possibleValues.erase(val);
        
        //col
        if (ref[i][c->col] != NULL)
            ref[i][c->col]->possibleValues.erase(val);
    }

    for (int row = 0; row < 3; row++) {
        int real_row = c->row + row - c->row%3;
        for (int col = 0; col < 3; col++) {
            int real_col = c->col + col - c->col%3;

            if (ref[real_row][real_col] != NULL)
                ref[real_row][real_col]->possibleValues.erase(val);
        }
    }

    ref[c->row][c->col] = NULL;
    delete c;
}

void
resolve_unsafe_move(string& m,
                    int idx,
                    vector<Cell*>& emptyCells,
                    vector<vector<Cell*> >& ref)
{
    string matrix_copy(m);
    set<char> values(emptyCells[idx]->possibleValues);

    auto snapshot = utils::make_snapshot(emptyCells);

    int count = 0;
    int size = values.size();
    
    for (auto& v : values) {
        //choose v as the only possible choice for this cell
        Cell* c = emptyCells[idx];
        c->possibleValues.clear();
        c->possibleValues.insert(v);

        //if its last possible value return to save on stack frames
        if (count == size-1) {
            for (auto& cell : snapshot)
                delete cell;

            return;
        }

        bool res = make_move(m, emptyCells, ref);

        //if this move was bad, backtrack and try
        //next value;
        if (res == false) {
            m = matrix_copy;
            emptyCells = utils::make_snapshot(snapshot);
            ref = utils::get_cell_ref(emptyCells);
        }
        //made the right move, cleanp and return;
        else {
            for (auto& cell : snapshot)
                delete cell;

            return;
        }
        count++;
    }
}

bool
make_move(string& m,
          vector<Cell*>& emptyCells,
          vector<vector<Cell*> >& ref)
{
    int size = 0;
    bool modified = false;
    while ((size = emptyCells.size()) > 0) {
        Cell* lastCell = emptyCells[size-1];

        // Cell has no possible moves, made an invalid move
        // return;
        if (lastCell->possibleValues.size() == 0) {
            return false;
        }
        if (lastCell->possibleValues.size() == 1) {
            char v = *(lastCell->possibleValues.begin());
            make_safe_move(m, lastCell, v, ref);
            emptyCells.pop_back();
            modified = true;
        }
        else if (modified) {
            sort_cells(emptyCells);
            modified = false;
        }
        else {
            resolve_unsafe_move(m, size-1, emptyCells, ref);
        }
    }

    return utils::check_solved(m);
}

} //anonymous namespace

void 
solve(string& m)
{
    vector<Cell*> emptyCells = utils::get_empty_cells(m);
    auto ref = utils::get_cell_ref(emptyCells);
    sort_cells(emptyCells);

    bool res = make_move(m, emptyCells, ref);

    if (res == false) {
        cout << "PUZZLE UNSOLVABLE:" << endl;
        utils::print_matrix(m);
        throw logic_error("puzzle unsolvable");
    }
}


} //namespace util
} //namespace sudoku

