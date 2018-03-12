#include <iostream>
#include "Sudoku.hpp"

using namespace std;

int main(int argc, const char **argv) {
  int sz = atoi(argv[2]);
  Sudoku s = Sudoku(sz, argv[1]);
  int reps = atoi(argv[4]);
  for (int i = 0; i < reps; ++i) {
    s.randomSolution();
    if(atoi(argv[3]) == 1)s.localSearch();
    else s.extendedLocalSearch();
    cout<<s.evalSolution()<<endl;
  }
}
