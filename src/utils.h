#ifndef SUDOKU_UTIL
#define SUDOKU_UTIL

#include <empty_cell.h>
#include <string>
#include <vector>

namespace sudoku {
namespace utils {

void print_matrix(const std::string& m);

bool check_solved(const std::string& m);

std::vector<Cell*> get_empty_cells(const std::string& m);

std::vector<std::vector<Cell*> > get_cell_ref(std::vector<Cell*>& cells);

std::vector<Cell*> make_snapshot(const std::vector<Cell*>& cells);

} //namespace util
} //namespace sudoku

#endif
