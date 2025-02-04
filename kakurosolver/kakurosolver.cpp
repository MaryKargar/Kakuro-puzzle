#include <iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <vector>

#include <bits/stdc++.h>
#include <array>
#include <omp.h>
#include <chrono>
#include "kakurosolver.h"

using namespace std;

int iter = 0;
#define NUM_THREADS 1

void display_arr(int *arr, int n)
{
  cout << "arr: ";
  for (int i = 0; i < n; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;
}

void print_coords(COORD start, COORD end)
{
  cout << "Start:" << start.first << "," << start.second << endl;
  cout << "End:" << end.first << "," << end.second << endl;
}

int find_length(COORD start, COORD end, direction dir)
{
  if (dir == d_down)
    return end.first - start.first;
  if (dir == d_right)
    return end.second - start.second;
  return -1;
}

int convert_sol(int **mat, int **&sol_mat, int m, int n)
{
  sol_mat = new int *[m]; // Rows
  for (int i = 0; i < m; i++)
  {
    sol_mat[i] = new int[n]; // Cols
  }
  int count = 0;
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (mat[i][j] == -2)
      {
        sol_mat[i][j] = -2; // Empty value cell
        count++;
      }
      else
      {
        sol_mat[i][j] = -1; // Hint or empty cell
      }
    }
  }
  return count;
}

void print_one_matrix(int **matrix, int m, int n)
{
  if (matrix == NULL)
  {
    std::cout << "Could not find a solution" << std::endl;
    return;
  }
  std::cout << "Matrix: " << std::endl;
  for (int i = 0; i < m; i++)
  { // rows
    for (int j = 0; j < n; j++)
    { // cols
      std::cout << matrix[i][j] << "\t";
    }
    std::cout << "\n";
  }
}

void sol_to_file(int **mat, int **sol_mat, int m, int n, string fname = "visualize.kakuro")
{
  ofstream to_write(fname);
  if (sol_mat == NULL)
  {
    std::cout << "Could not find a solution" << std::endl;
    return;
  }
  to_write << m << " " << n << "\n";
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (mat[i][j] != -2)
        to_write << mat[i][j] << " ";
      else
        to_write << sol_mat[i][j] << " ";
    }
    to_write << "\n";
  }
  to_write.close();
}

/*void read_matrix(int **&matrix, std::ifstream &afile, int m, int n)
{
  matrix = new int *[m]; // rows
  for (int i = 0; i < m; i++)
  {
    matrix[i] = new int[n]; // cols
  }
  int val;
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      afile >> val;
      matrix[i][j] = val;
    }
  }
}*/

COORD find_end(int **matrix, int m, int n, int i, int j, direction dir, vector<COORD> &includes)
{ // 0 down 1 right
  if (dir == d_right)
  {
    for (int jj = j + 1; jj < n; jj++)
    {
      if (matrix[i][jj] != -2 || jj == n - 1)
      {
        if (matrix[i][jj] == -2 && jj == n - 1)
          jj++;
        COORD END = COORD(i, jj);
        return END;
      }
      includes.push_back(COORD(i, jj + 1));
    }
  }
  if (dir == d_down)
  {
    for (int ii = i + 1; ii < m; ii++)
    {
      if (matrix[ii][j] != -2 || ii == m - 1)
      {
        if (matrix[ii][j] == -2 && ii == m - 1)
          ii++;
        COORD END = COORD(ii, j);
        return END;
      }
      includes.push_back(COORD(ii + 1, j));
    }
  }
  return COORD(0, 0);
}

vector<sum> get_sums(int **matrix, int m, int n)
{
  vector<sum> sums;
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      int val = matrix[i][j];
      if (val != -1 && val != -2)
      {
        int hint = val;
        hint = hint / 10;

        if ((hint % 100) == 0)
        {
          vector<COORD> includes;
          hint = (int)(hint / 100);
          COORD START = COORD(i, j + 1);
          includes.push_back(START);
          COORD END = find_end(matrix, m, n, i, j, d_right, includes);
          includes.push_back(END);
          sum _sum = sum(START, END, hint, d_right, includes);
          sums.push_back(_sum);
        }

        else
        {
          int div = (int)(hint / 100);
          int rem = (int)(hint % 100);

          if (div == 0 && rem != 0)
          {
            vector<COORD> includes;
            COORD START = COORD(i + 1, j);
            includes.push_back(START);
            COORD END = find_end(matrix, m, n, i, j, d_down, includes);
            includes.push_back(END);
            sum _sum = sum(START, END, rem, d_down, includes);
            sums.push_back(_sum);
          }

          if (div != 0 && rem != 0)
          {
            vector<COORD> includes1;
            vector<COORD> includes2;
            COORD START1 = COORD(i + 1, j);
            COORD START2 = COORD(i, j + 1);
            includes1.push_back(START1);
            includes2.push_back(START2);
            COORD END1 = find_end(matrix, m, n, i, j, d_down, includes1);
            COORD END2 = find_end(matrix, m, n, i, j, d_right, includes2);
            includes1.push_back(END1);
            includes2.push_back(END2);
            sum _sum1 = sum(START1, END1, rem, d_down, includes1);
            sum _sum2 = sum(START2, END2, div, d_right, includes2);
            sums.push_back(_sum1);
            sums.push_back(_sum2);
          }
        }
      }
    }
  }
  return sums;
}

int** deepCopy(int **&M, int m, int n)
{
  int **copy = new int *[m];
  for (int i = 0; i < m; i++)
  {
    copy[i] = new int[n];
    for (int j = 0; j < n; j++)
    {
      copy[i][j] = M[i][j];
    }
  }
  return copy;
}

bool verifyRemainingNumbers(int sum_needed, int cells_left, const std::bitset<9> &exists)
{
  int possible_max = 0;
  int possible_min = 0;
  int local_max = 9;
  int local_min = 1;
  // calculate maximum and minimum possible sum
  // #pragma omp parallel for reduction (+: max,min)
  for (int i = 0; i < cells_left; i++)
  {
    while (exists[local_max - 1])
    {
      local_max--;
    }
    while (exists[local_min - 1])
    {
      local_min++;
    }
    possible_max += local_max;
    possible_min += local_min;
    local_max--;
    local_min++;
  }
  if (sum_needed < possible_min || sum_needed > possible_max)
    return false;
  return true;
}

bool wrongSolution(int **sol_mat, const sum &curr_sum)
{
  std::bitset<9> exists = {false};
  int dir = curr_sum.dir;
  int row = curr_sum.start.first;
  int col = curr_sum.start.second;
  int end_row = curr_sum.end.first;
  int end_col = curr_sum.end.second;
  int hint = curr_sum.hint;
  // Check if there is duplicate numbers or it can ever reach sum or will it pass the sum
  while ((dir == d_right ? col < end_col : row < end_row) && sol_mat[row][col] > 0)
  {
    if (exists[sol_mat[row][col] - 1])
      return true;
    hint = hint - sol_mat[row][col];
    bool verified = verifyRemainingNumbers(hint, (dir == d_right ? end_col - col - 1 : end_row - row - 1), exists);
    if (!verified)
      return true;
    exists[sol_mat[row][col] - 1] = true;
    (dir == d_right ? col : row)++;
  }
  return false;
}

int **solve_kakuro_serial(int **sol_mat, int k, vector<COORD> &coords, const int &cs,
  const vector<vector<vector<sum *>>> &which_sums_cell_exists)
{
  int i = coords[k].first;
  int j = coords[k].second;
  for (int num = 1; num < 10; num++)
  {
    sol_mat[i][j] = num;
    vector<sum *> curr_sum = which_sums_cell_exists[i][j];
    bool should_expand = true;
    for (int k = 0; k < curr_sum.size(); k++)
    {
      if (wrongSolution(sol_mat, *curr_sum[k]))
      {
        should_expand = false;
        break;
      }
    }
    if (should_expand)
    {
      if (k + 1 == cs)
      {
        return sol_mat;
      }
      int **result = solve_kakuro_serial(sol_mat, k + 1, coords, cs, which_sums_cell_exists);
      if (result != NULL)
      {
        return result;
      }
    }
    sol_mat[i][j] = -2; // Undo the change when backtracking
  }
  return NULL;
}

int **solution_serial(int **mat, int **sol_mat, vector<sum> sums, int m, int n, int size, int empty_cells)
{
  vector<vector<vector<sum *>>> which_sums_cell_exists(m, vector<vector<sum *>>(n, vector<sum *>()));
  vector<COORD> coords;
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (sol_mat[i][j] == -2)
      {
        coords.push_back(COORD(i, j));
      }
    }
  }
  int cs = coords.size();
  for (int i = 0; i < cs; i++)
  {
    for (int k = 0; k < sums.size(); k++)
    {
      for (int l = 0; l < sums[k].includes.size(); l++)
      {
        if (sums[k].includes[l] == coords[i])
        {
          which_sums_cell_exists[coords[i].first][coords[i].second].push_back(&sums[k]);
          break;
        }
      }
    }
  }
  return solve_kakuro_serial(sol_mat, 0, coords, cs, which_sums_cell_exists);
}

int **solve_kakuro(int **sol_mat, int k, vector<COORD> &coords, const int &cs, const vector<vector<vector<sum *>>> &which_sums_cell_exists, COORD size)
{
  int i = coords[k].first;
  int j = coords[k].second;
  std::bitset<9> expand;
  expand.set();
  vector<sum *> curr_sum = which_sums_cell_exists[i][j];
  //check which numbers can be but in the cell
#pragma omp parallel shared(expand)
  {
#pragma omp for
    for (int num = 1; num < 10; num++)
    {
      int tid = omp_get_thread_num();
      int **sol_mat_copy = deepCopy(sol_mat, size.first, size.second);
      sol_mat_copy[i][j] = num;
      for (int s = 0; s < curr_sum.size(); s++)
      {
        if (wrongSolution(sol_mat_copy, *curr_sum[s]))
        {
#pragma omp critical
          {
            expand[num - 1] = false;
          }
        }
      }
      for (int s = 0; s < size.first; s++)
      {
        delete[] sol_mat_copy[s];
      }
      delete[] sol_mat_copy;
    }
#pragma omp barrier
  }
  // Expand the possiible cells
  for (int num = 1; num < 10; num++)
  {
    if (expand[num - 1])
    {
      sol_mat[i][j] = num;
      if (k + 1 == cs)
      {
        return sol_mat;
      }
      int **result = solve_kakuro(sol_mat, k + 1, coords, cs, which_sums_cell_exists, size);
      if (result != NULL)
      {
        return result;
      }
    }
    sol_mat[i][j] = -2; // Undo the change when backtracking
  }
  return NULL;
}

int **solution(int **mat, int **sol_mat, vector<sum> sums, int m, int n, int size, int empty_cell)
{
  vector<vector<vector<sum *>>> which_sums_cell_exists(m, vector<vector<sum *>>(n, vector<sum *>()));
  vector<COORD> coords;
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (sol_mat[i][j] == -2)
      {
        coords.push_back(COORD(i, j));
      }
    }
  }
  int cs = coords.size();
  // #pragma omp parallel for
  for (int i = 0; i < cs; i++)
  {
    for (int k = 0; k < sums.size(); k++)
    {
      for (int l = 0; l < sums[k].includes.size(); l++)
      {
        if (sums[k].includes[l] == coords[i])
        {
          // #pragma omp critical
          which_sums_cell_exists[coords[i].first][coords[i].second].push_back(&sums[k]);
          break;
        }
      }
    }
  }
  return solve_kakuro(sol_mat, 0, coords, cs, which_sums_cell_exists, COORD(m, n));
}
