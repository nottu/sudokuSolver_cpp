#ifndef SUDOKU_H
#define SUDOKU_H
#include <vector>
#include <fstream>

class SolutionNeighbour{
  int square;
  std::vector<int> neighbour;
  int eval;
public:
  int get_square();
  std::vector<int> get_neighbour();
  int get_eval();
  SolutionNeighbour(int sq, std::vector<int> &n, int eval);
};
class Sudoku {
private:
  std::vector<std::vector<int> > data;
  std::vector<std::vector<int> > solved;
  std::vector<std::vector<int> > solution;
  std::vector<std::vector<int> > solutionidx; //index of solutions in square
  std::vector<std::vector<int> > conflicts; //index of solutions in square
  void get_avial_inSQ(int sq);
  std::vector<SolutionNeighbour> neighbours;
  //square number [0, n-1], item number [0, n-1] and value [1, n]
  int __get_conflicts_by_elem(int sq, int item, int val);
  int __get_conflicts();
  void calc_neighbours();
public:
  Sudoku(int size, const char* file_name);
  void randomSolution();
  void localSearch();
  int evalSolution();
  void printSolution();
};
#endif