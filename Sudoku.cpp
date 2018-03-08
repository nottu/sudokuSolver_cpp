#include "Sudoku.hpp"
#include <iostream>
#include <math.h>

using namespace std;

Sudoku::Sudoku(int size, const char* file_name) {
  ifstream ifs(file_name);
  data.reserve(size);
  solved.reserve(size);
  if(ifs.is_open()) {
    int v;
    for (int i = 0; i < size; ++i) {
      vector<int> vi, vs;
      vi.reserve(size);
      vs.reserve(size);
      for (int j = 0; j < size; ++j) {
        ifs >> v;
        vi.push_back(v);
        vs.push_back(v);
      }
      data.push_back(vi);
      solved.push_back(vs);
    }
    ifs.close();
  }
  //init solution
  solution.reserve(size);
  for (int i = 0; i < size; ++i)
    solution.push_back(get_avial_inSQ(i));
}
// returns available numbers in the nth square
vector<int> Sudoku::get_avial_inSQ(int sq){
  int size = data.size();
  int sqLen = (int)sqrt(size);
  vector<int> all_nums;
  all_nums.reserve(size);
  int sqrow = (sq / sqLen) * sqLen;
  int sqcol = (sq * sqLen) % size;
  for (int i = 0; i < size; ++i) all_nums.push_back(i+1);
  for (int i = 0; i < size; ++i){
    int i_idx = i/sqLen + sqrow, j_idx = i%sqLen + sqcol;
    if(data[i_idx][j_idx])
      all_nums[data[i_idx][j_idx] - 1] = 0;
  }
  vector<int> nums;
  for (int i = 0; i < size; ++i)
    if(all_nums[i])
      nums.push_back(all_nums[i]);
  return nums;
}
int Sudoku::__get_conflicts(){
  int conflicts = 0;
  int size = data.size();
  //check each row/col
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if(data[i][j] == 0){
        for (int k = 0; k < size; ++k)
        {
          if(solved[i][j] == data[i][k]) conflicts += 100; //hard conflict
          else if(k != j && solved[i][j] == solved[i][k]) conflicts += 1;
        }
      }
      if(data[j][i] == 0){
        for (int k = 0; k < size; ++k)
        {
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
  int size = data.size();
  int sqLen = (int)sqrt(size);
  for (int i = 0; i < size; ++i)
  {
    int sqrow = (i / sqLen) * sqLen;
    int sqcol = (i * sqLen) % size;
    int cnt = 0;
    for (int j = 0; j < size; ++j)
    {
      int i_idx = j/sqLen + sqrow, j_idx = j%sqLen + sqcol;
      if(!data[i_idx][j_idx]){
        solved[i_idx][j_idx] = solution[i][cnt++];
      }
    }
  }
  return __get_conflicts();
}

void Sudoku::printSolution() {
  cout<< "conflictos: "<< evalSolution()<<endl;
  int size = solved.size();
  int sqLen = (int)sqrt(size);
  for (int i = 0; i < size; ++i){
    int cnt = 0;
    for (int j = 0; j < size; ++j){
      cout << solved[i][j] << " ";
    }
    cout << endl;
  }
}