#include <iostream>
#include "Sudoku.hpp"

using namespace std;
#define SECOND 1000
#define MINUTE 60 * SECOND

int main(int argc, const char **argv) {
  int sz = atoi(argv[2]);
  Sudoku s = Sudoku(sz, argv[1], false);
  s.randomSolution();
 s.localSearch();
  // s.printSolution();
  ofstream output(argv[3]);
  s.simmulatedAnnealing(2 * MINUTE, 20, output);
  output.close();
  // s.printSolution();
}
