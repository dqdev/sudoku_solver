#include <utils.h>
#include <empty_cell.h>

#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

namespace sudoku {
namespace utils {

using namespace std;

namespace {

void print_line()
{
    cout << "+---+---+---++---+---+---++---+---+---+" << endl;
}


void print_bold()
{
    cout << "+===+===+===++===+===+===++===+===+===+" << endl;
}


vector<set<char> >
get_empty_rows(const string& m)
{
    vector<set<char> > ret(9);
    int idx = 0;
    for (int r = 0; r < 9; r++) {
        set<char> row_set {'1','2','3','4','5','6','7','8','9'};
        
        for (int c = 0; c < 9; c++) {
            if (m[idx] != ',')
                row_set.erase(m[idx]);

            idx++;
        }

        ret[r] = row_set;
    }
    return ret;
}


vector<set<char> >
get_empty_cols(const string& m)
{
    vector<set<char> > ret(9);
    for (int c = 0; c < 9; c++) {
        set<char> col_set {'1','2','3','4','5','6','7','8','9'};
        
        for (int r = 0; r < 9; r++) {
            int idx = r*9 + c;
            if (m[idx] != ',')
                col_set.erase(m[idx]);
        }

        ret[c] = col_set;
    }
    return ret;
}

int
get_zone_from(int row, int col)
{
    if (row < 3) {
        if (col < 3) return 0;
        if (col < 6) return 1;
        else return 2;
    }
    else if (row < 6) {
        if (col < 3) return 3;
        if (col < 6) return 4;
        else return 5;
    }
    else {
        if (col < 3) return 6;
        if (col < 6) return 7;
        else return 8;
    }
}

vector<set<char> >
get_empty_zones(const string& m)
{
    vector<set<char> > ret = {
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'},
        {'1','2','3','4','5','6','7','8','9'}
    };

    int idx = 0;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (m[idx] != ',') {
                int zone = get_zone_from(r,c);
                ret[zone].erase(m[idx]);
            }
            idx++;
        }
    }
    return ret;
}


} //anonymous namespace

void 
print_matrix(const string& m)
{
    print_line();

    int idx = 1;
    for (auto& c : m) {

        if (c == ',') { cout << "|   "; }
        else { cout << "| " << c << " "; }

        if (idx%9 == 0) { 
            cout << "|\n"; 
            
            //27 is after 3 lines, 54 is after 6 lines
            if (idx == 27 || idx == 54) { print_bold(); }
            else { print_line(); }
        }
        else if (idx%3 == 0) { cout << "|"; }

        idx++;
    }
}

bool 
check_solved(const string& m)
{
    for (auto& c : m) {
        if (c == ',')
            return false;
    }

    return true;
}

vector<Cell*>
get_empty_cells(const string& m)
{
    vector<set<char> > row_values = get_empty_rows(m);
    vector<set<char> > col_values = get_empty_cols(m);
    vector<set<char> > zone_values = get_empty_zones(m);

    vector<Cell*> ret;

    int idx = 0;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (m[idx] == ',') {
                int z = get_zone_from(r,c);

                set<char>& row = row_values[r];
                set<char>& col = col_values[c];
                set<char>& zone = zone_values[z];

                vector<char> temp(9);
                set_intersection(row.begin(), row.end(),
                                 col.begin(), col.end(),
                                 temp.begin());

                vector<char> vals(9);
                auto it = set_intersection(temp.begin(), temp.end(),
                                           zone.begin(), zone.end(),
                                           vals.begin());
                vals.resize(it-vals.begin());

                set<char> possibleValues(vals.begin(), vals.end());
                
                //TODO - make unique_ptr
                Cell* empty = new Cell(r, c, possibleValues);
                ret.push_back(empty);
            }
            idx++;
        }
    }

    return ret;
}


vector<vector<Cell*> > 
get_cell_ref(vector<Cell*>& cells)
{
    vector<vector<Cell*> > refs(9, vector<Cell*>(9, NULL));  

    for (auto& c : cells)
        refs[c->row][c->col] = c;

    return refs;
}


vector<Cell*> 
make_snapshot(const vector<Cell*>& cells)
{
    int size = cells.size();
    vector<Cell*> snapshot(size);
    for (int i = 0; i < size; i++) {
        Cell* c = new Cell(*cells[i]);
        snapshot[i] = c;
    }
    
    return snapshot;
}

} //namespace util
} //namespace sudoku
