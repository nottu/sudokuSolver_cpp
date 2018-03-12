#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

#include "Sudoku.hpp"

using namespace std;

Sudoku::Sudoku(int size, const char* file_name) {
//  unsigned seed = 0;
  auto seed = chrono::system_clock::now().time_since_epoch().count();
  srand(seed);
  ifstream ifs(file_name);
  data.reserve((unsigned int)size);
  solved.reserve((unsigned int)size);
  if(ifs.is_open()) {
    int v;
    for (int i = 0; i < size; ++i) {
      vector<int> vi, vs;
      vi.reserve((unsigned int)size);
      vs.reserve((unsigned int)size);
      for (int j = 0; j < (unsigned int)size; ++j) {
        ifs >> v;
        vi.emplace_back(v);
        vs.emplace_back(v);
      }
      data.emplace_back(vi);
      solved.emplace_back(vs);
    }
    ifs.close();
  }
  //init solution
  solution.reserve((unsigned)size);
  solutionidx.reserve((unsigned)size);
  for (int i = 0; i < size; ++i)
    __get_avial_inSQ(i);
}
// returns available numbers in the nth square
void Sudoku::__get_avial_inSQ(int sq){
  auto size  = (int)data.size();
  auto sqLen = (int)sqrt(size);
  vector<int> all_nums; //could use bitset
  vector<int> idx;
  all_nums.reserve((unsigned)size);
  int sqrow = (sq / sqLen) * sqLen;
  int sqcol = (sq * sqLen) % size;
  for (int i = 0; i < size; ++i) all_nums.emplace_back(i+1);
  for (int i = 0; i < size; ++i){
    int i_idx = i/sqLen + sqrow, j_idx = i%sqLen + sqcol;
    if(data[i_idx][j_idx])
      all_nums[data[i_idx][j_idx] - 1] = 0;
    else
      idx.emplace_back(i);
  }
  solutionidx.emplace_back(idx);
  vector<int> nums;
  for (int i = 0; i < size; ++i)
    if(all_nums[i])
      nums.emplace_back(all_nums[i]);
  solution.emplace_back(nums);
}
int Sudoku::__get_conflicts(){
  int conflicts = 0;
  auto size = (int)data.size();
  //check each row/col
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if(data[i][j] == 0){
        for (int k = 0; k < size; ++k) {
          if(solved[i][j] == data[i][k]) conflicts += 100; //hard conflict
          else if(k != j && solved[i][j] == solved[i][k]) conflicts += 1;
        }
      }
      if(data[j][i] == 0){
        for (int k = 0; k < size; ++k) {
          if(solved[j][i] == data[k][i]) conflicts += 100; //hard conflict
          else if(k != j && solved[j][i] == solved[k][i]) conflicts += 1;
        }
      }
    }
  }
  return conflicts;
}
void Sudoku::__update_solution(){
  //create solution
  auto size = (int)data.size();
  auto sqLen = (int)sqrt(size);
  for (int i = 0; i < size; ++i) {
    int sqrow = (i / sqLen) * sqLen;
    int sqcol = (i * sqLen) % size;
    int cnt = 0;
    for (int j = 0; j < size; ++j) {
      int i_idx = j/sqLen + sqrow, j_idx = j%sqLen + sqcol;
      if(!data[i_idx][j_idx]) solved[i_idx][j_idx] = solution[i][cnt++];
    }
  }
}
int Sudoku::evalSolution() {
  __update_solution();
  return __get_conflicts();
//  return conflicts;
}

void Sudoku::printSolution() {
  cout<< "conflictos: "<< evalSolution()<<endl;
  auto size = solved.size();
  for (int i = 0; i < size; ++i){
    for (int j = 0; j < size; ++j){
      cout << solved[i][j] << " ";
    }
    cout << endl;
  }
}
int Sudoku::__get_conflicts_by_elem(int sq, int item, int val){
  int conflicts = 0;
  auto size = (int)solution.size();
  auto sqLen = (int)sqrt(size);
  int sqrow = (sq / sqLen) * sqLen; //row and col of first element
  int sqcol = (sq * sqLen) % size;
  int i_idx = (item / sqLen) + sqrow, j_idx = (item % sqLen) + sqcol;
  for (int i = 0; i < size; ++i) {
    if(i < sqcol || i >= sqcol + sqLen){
      if(data[i_idx][i] == val)
        conflicts += 100;
      else if(solved[i_idx][i] == val)
        conflicts += 2; //should be 2...
    }
    if(i < sqrow || i >= sqrow + sqLen) {
      if (data[i][j_idx] == val)
        conflicts += 100;
      else if (solved[i][j_idx] == val)
        conflicts += 2; //should be 2...
    }
  }
  return conflicts;
}
vector<vector< int> > Sudoku::initdpTable(unsigned size){
  vector<vector< int> > dptable;
  dptable.reserve(size);
  for (int j = 0; j < size ; ++j) {
    vector< int> tmp;
    tmp.reserve(size);
    for (int k = 0; k < size ; ++k) {
      tmp.push_back(-1);
    }
    dptable.push_back(tmp);
  }
  return dptable;
}
void Sudoku::__calc_neighbours_allperm_sq(int sq, int mask, int w, vector<vector<int> > &dptable, int item, int max_w, vector<int> s_n){
  auto size = (unsigned)solution[sq].size();
  for (int i = 0; i < size; ++i) {
    int v = 1 << i;
    if(v & mask) continue;
    if(dptable[item][i] == -1){
      int val = solution[sq][i];
      int idx = solutionidx[sq][item];
      dptable[item][i] = __get_conflicts_by_elem(sq, idx, val);
    }
    int n_mask = v | mask;
    int new_w = w + dptable[item][i];
    s_n[item] = solution[sq][i];
    int mx = (1<<size)- 1;
    if(n_mask < mx && new_w < max_w)
      __calc_neighbours_allperm_sq(sq, n_mask, new_w, dptable, item + 1, max_w, s_n);
    else{
      if (new_w < max_w)
        neighbours.emplace_back(SolutionNeighbour(sq, s_n, max_w - new_w ));
    }
  }
}
void Sudoku::__calc_neighbours_allperm(){
  neighbours.clear();
  auto size = (unsigned)solution.size();
  for (int i = 0; i < size; ++i) {
    auto v_size = (unsigned)solution[i].size();
    vector<vector< int> > dptable = initdpTable(v_size);
    int max_w = 0;
    for (int j = 0; j < v_size; ++j) {
      dptable[j][j] = __get_conflicts_by_elem(i, solutionidx[i][j], solution[i][j]);
      max_w += dptable[j][j];
    }
    vector<int> s_n(solution[i]);
    __calc_neighbours_allperm_sq(i, 0, 0, dptable, 0, max_w, s_n);
  }
}
void Sudoku::__calc_neighbours(){
  neighbours.clear();
  auto size = (int)solution.size();
  for (int i = 0; i < size; ++i) {
    auto v_size = (unsigned)solution[i].size();
    vector<vector< int> > dptable = initdpTable(v_size);
    for (int j = 0; j < v_size - 1; ++j) {
      for (int k = j+1; k < v_size; ++k) {
        //can be improved with dynamic programming, we check items n times, only 1 is actually needed
        if(dptable[j][j] == -1){
          dptable[j][j] = __get_conflicts_by_elem(i, solutionidx[i][j], solution[i][j]);
        }
        if(dptable[j][k] == -1){
          dptable[j][k] = __get_conflicts_by_elem(i, solutionidx[i][j], solution[i][k]);
        }
        if(dptable[k][j] == -1){
          dptable[k][j] = __get_conflicts_by_elem(i, solutionidx[i][k], solution[i][j]);
        }
        if(dptable[k][k] == -1){
          dptable[k][k] = __get_conflicts_by_elem(i, solutionidx[i][k], solution[i][k]);
        }
        int conf_o1 = dptable[j][j];
        int conf_o2 = dptable[k][k];

        int conf   = dptable[j][k];
        int conf2  = dptable[k][j];
        if((conf_o1 + conf_o2) <= (conf + conf2)) continue; //dont add bad neighbouts
        vector<int> s_n(solution[i]);
        s_n[j] = solution[i][k]; s_n[k] = solution[i][j]; //swap
        neighbours.emplace_back(SolutionNeighbour(i, s_n, (conf_o1 + conf_o2) - (conf + conf2) ));
      }
    }
  }
}

void Sudoku::randomSolution(){
  auto size = (int)solution.size();
  auto seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
  auto rng = std::default_random_engine(seed);
  for (int i = 0; i < size; ++i) {
    shuffle(begin(solution[i]), end(solution[i]), rng);
  }
}
//should make both use the same code...
void Sudoku::localSearch(){
  conflicts = evalSolution();
  int iter = 0;
  do {
    iter++;
    __calc_neighbours();
    auto size_neighbours = neighbours.size();
    if(size_neighbours == 0) break; //no improvement
    auto i = rand() % size_neighbours; //look into using c++11 random lib
    int sq = neighbours[i].get_square();
    vector<int> tmp = solution[sq];
    solution[sq] = neighbours[i].get_neighbour();
    int dif = neighbours[i].get_eval();
    __update_solution();
    conflicts -= dif;
  } while(iter < 10000);
//  cout<<"Iteraciones de busqueda local: " << iter << endl;
}
void Sudoku::extendedLocalSearch(){
  conflicts = evalSolution();
  int iter = 0;
  do {
    iter++;
    __calc_neighbours_allperm();
    auto size_neighbours = neighbours.size();
    if(size_neighbours == 0) break; //no improvement
    auto i = rand() % size_neighbours; //look into using c++11 random lib
    int sq = neighbours[i].get_square();
    vector<int> tmp = solution[sq];
    solution[sq] = neighbours[i].get_neighbour();
    int dif = neighbours[i].get_eval();
    __update_solution();
    int nev = evalSolution();
    conflicts -= dif;
  } while(iter < 10000);
//  cout<<"Iteraciones de busqueda local: " << iter << endl;
}
int SolutionNeighbour::get_square(){
  return square;
}
vector<int> SolutionNeighbour::get_neighbour(){
  return neighbour;
}
int SolutionNeighbour::get_eval() {
  return eval;
}
SolutionNeighbour::SolutionNeighbour(int sq, vector<int> &n, int e){
  square = sq;
  neighbour = n;
  eval = e;
}