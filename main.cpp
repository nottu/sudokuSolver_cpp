#include <iostream>
#include "Sudoku.hpp"

using namespace std;

//int main(int argc, const char **argv) {
//  int sz = atoi(argv[2]);
//  Sudoku s = Sudoku(sz, argv[1]);
////  s.printSolution();
//  int reps = atoi(argv[4]);
//  for (int i = 0; i < reps; ++i) {
//    s.randomSolution();
//    int met = atoi(argv[3]);
//    if(met == 1)s.localSearch();
//    // else if(met == 2) s.extendedLocalSearch();
//    else{
//      s.localSearch();
//      s.extendedLocalSearch();
//    }
//    cout<<s.evalSolution()<<endl;
//  }
////  s.printSolution();
//}
int main(int argc, const char **argv) {
  int sz = atoi(argv[2]);
  Sudoku s = Sudoku(sz, argv[1], false);
  s.randomSolution();
//  s.localSearch();
  s.printSolution();
  s.simmulatedAnnealing(60000, 10);
  s.printSolution();
}
