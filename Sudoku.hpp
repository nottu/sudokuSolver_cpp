#ifndef SUDOKU_H
#define SUDOKU_H
#include <vector>
#include <fstream>

class Sudoku {
private:
  std::vector<std::vector<int>> data;
  std::vector<std::vector<int>> solved;
  std::vector<std::vector<int>> solution;
  std::vector<int> get_avial_inSQ(int sq);
public:
  Sudoku(int size, const char* file_name);
  int evalSolution();
  void printSolution();
};

#endif