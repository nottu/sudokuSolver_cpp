#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

#include "Sudoku.hpp"

using namespace std;

Sudoku::Sudoku(int size, const char* file_name) {
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
    get_avial_inSQ(i);
}
// returns available numbers in the nth square
void Sudoku::get_avial_inSQ(int sq){
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

int Sudoku::evalSolution() {
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
  return __get_conflicts();
}

void Sudoku::printSolution() {
  cout<< "conflictos: "<< evalSolution()<<endl;
  auto size = solved.size();
  auto sqLen = (int)sqrt(size);
  for (int i = 0; i < size; ++i){
    int cnt = 0;
    for (int j = 0; j < size; ++j){
      cout << solved[i][j] << " ";
    }
    cout << endl;
  }
  calc_neighbours();
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
void Sudoku::calc_neighbours(){
  neighbours.clear();
  auto size = (int)solution.size();
  for (int i = 0; i < size; ++i) {
    auto v_size = (int)solution[i].size();
    for (int j = 0; j < v_size - 1; ++j) {
      for (int k = j+1; k < v_size; ++k) {
        //can be improved with dynamic programming, we check items n times, only 1 is actually needed
        int conf_o = __get_conflicts_by_elem(i, solutionidx[i][j], solution[i][j]) +
                     __get_conflicts_by_elem(i, solutionidx[i][k], solution[i][k]);

        int conf   = __get_conflicts_by_elem(i, solutionidx[i][j], solution[i][k]) +
                     __get_conflicts_by_elem(i, solutionidx[i][k], solution[i][j]);
        if(conf_o <= conf) continue; //dont add bad neighbouts
        vector<int> s_n(solution[i]);
        s_n[j] = solution[i][k]; s_n[k] = solution[i][j]; //swap
        neighbours.emplace_back(SolutionNeighbour(i, s_n, conf_o - conf));
      }
    }
  }
  auto seed = 0;// chrono::system_clock::now().time_since_epoch().count();
  auto rng = std::default_random_engine(seed);
  shuffle(begin(neighbours), end(neighbours), rng);
}

void Sudoku::randomSolution(){
  auto size = (int)solution.size();
  unsigned seed = 0; //std::chrono::system_clock::now().time_since_epoch().count();
  auto rng = std::default_random_engine(seed);
  for (int i = 0; i < size; ++i)
  {
    shuffle(begin(solution[i]), end(solution[i]), rng);
  }
}

void Sudoku::localSearch(){
  bool improved = false;
  int best = evalSolution();
  int iter = 0;
  do {
    iter++;
    calc_neighbours();
    auto size_neighbours = neighbours.size();
    if(size_neighbours == 0) break; //no improving...
    for (int i = 0; i < size_neighbours; ++i)
    {
      int sq = neighbours[i].get_square();
      vector<int> tmp = solution[sq];
      solution[sq] = neighbours[i].get_neighbour();
      int new_fit = evalSolution();
      if( new_fit < best){
        best = new_fit;
        improved = true;
        break;
      }
      improved = false;
      solution[sq] = tmp;
    }
  } while(improved);
  cout<<"Iteraciones de busqueda local: " << iter << endl;
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