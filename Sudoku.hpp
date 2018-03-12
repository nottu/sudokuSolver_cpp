#ifndef SUDOKU_H
#define SUDOKU_H
#include <vector>
#include <fstream>

class SolutionNeighbour{
  int eval;
  int square;
  std::vector<int> neighbour;
public:
  int get_eval();
  int get_square();
  std::vector<int> get_neighbour();
  SolutionNeighbour(int sq, std::vector<int> &n, int eval);
};
class Sudoku {
private:
  int conflicts; //
  std::vector<std::vector<int> > data;
  std::vector<std::vector<int> > solved;
  std::vector<std::vector<int> > solution;
  std::vector<std::vector<int> > solutionidx; //index of solutions in square
  std::vector<SolutionNeighbour> neighbours;
  int  __get_conflicts_by_elem(int sq, int item, int val);
  void __get_avial_inSQ(int sq);
  int  __get_conflicts();
  void __update_solution();
  void __calc_neighbours();
public:
  Sudoku(int size, const char* file_name);
  void randomSolution();
  void localSearch();
  int evalSolution();
  void printSolution();
};
#endif