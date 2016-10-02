#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <utils.h>
#include <solver.h>

using namespace std;
using namespace sudoku;

string get_matrix()
{
    ifstream file("data/sudoku.in");

    string whole_line("");
    string line;

    while (getline(file,line))
    {
        whole_line += line;
    }
    file.close();

    return whole_line;
}

int main()
{
    string matrix = get_matrix();

    cout << "INPUT: " << endl;
    utils::print_matrix(matrix);

    solver::solve(matrix);
    cout << "\n\n" << endl;

    cout << "SOLVED: " << endl;
    utils::print_matrix(matrix);
}
