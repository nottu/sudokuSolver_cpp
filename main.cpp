#include <iostream>
#include "Sudoku.hpp"

using namespace std;

int main(int argc, const char **argv) {
  Sudoku s = Sudoku(9, argv[1]);
  s.printSolution();
}
