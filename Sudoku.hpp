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
  bool _debugMode;
  unsigned _seed;
  int conflicts; //
  std::vector<std::vector<int> > data;
  std::vector<std::vector<int> > solved;
  std::vector<std::vector<int> > solution;
  std::vector<std::vector<int> > solutionidx; //index of solutions in square
  std::vector<SolutionNeighbour> neighbours;
  std::vector<std::vector<int> > dptable;
  int  __get_conflicts_by_elem(int sq, int item, int val);
  void __get_avial_inSQ(int sq);
  int  __get_conflicts();
  void __update_solution();
  void __calc_neighbours(bool addBad = false);
  void __calc_neighbours_allperm();
  void __calc_neighbours_allperm_sq(int sq, int mask, int w, int item, int max_w, std::vector<int> &sol);
  std::vector<std::vector< int> > initdpTable(unsigned size);
public:
  Sudoku(int size, const char* file_name, bool debugMode = false);
  void randomSolution();
  int evalSolution();
  void printSolution();

  //solving methods
  //simple hill-climbing
  void localSearch();
  void extendedLocalSearch();
  //trajectory metaheuristics
  void simmulatedAnnealing(int time, double temp, std::ofstream &out); //time in millis
};
#endif