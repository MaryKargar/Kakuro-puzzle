#ifndef KAKUROSOLVER_H
#define KAKUROSOLVER_H

#include <iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <vector>

#include <bits/stdc++.h>
#include <array>
#include <omp.h>
#include <chrono>

using namespace std;

enum direction
{
    d_down,
    d_right,
    none
  };

#define COORD std::pair<int, int>
// #define DEBUG


void display_arr(int *arr, int n);
void print_coords(COORD start, COORD end);
int find_length(COORD start, COORD end, direction dir);
int convert_sol(int **mat, int **&sol_mat, int m, int n);
void print_one_matrix(int **matrix, int m, int n);
void read_matrix(int **&matrix, std::ifstream &afile, int m, int n);


struct sum
{
    COORD start;
    COORD end;
    int hint;
    int dir;
    int length;
    int *arr;
    vector<COORD> includes;
    void print_sum()
    {
        cout << "############################" << endl;
        cout << "Creating sum with: " << endl;
        print_coords(start, end);
        cout << "Hint: " << hint << endl;
        cout << "Direction: " << dir << endl;
        cout << "Length: " << length << endl;
        cout << "############################" << endl;
    }
    sum(COORD _start, COORD _end, int _hint, direction _dir, vector<COORD> _includes) : start(_start), end(_end), hint(_hint), dir(_dir), includes(_includes)
    {
        length = find_length(_start, _end, _dir);
        arr = new int[length];
#ifdef DEBUG
        cout << "############################" << endl;
        cout << "Creating sum with: " << endl;
        print_coords(start, end);
        cout << "Hint: " << hint << endl;
        cout << "Direction: " << dir << endl;
        cout << "Length: " << length << endl;
        cout << "############################" << endl;
#endif
    }
    //~sum(){
    // delete arr;
    //}
};

COORD find_end(int **matrix, int m, int n, int i, int j, direction dir, std::vector<COORD> &includes);
std::vector<sum> get_sums(int **matrix, int m, int n);
int** deepCopy(int **&M, int m, int n);
bool verifyRemainingNumbers(int sum_needed, int cells_left, const std::bitset<9> &exists);
bool wrongSolution(int **sol_mat, const sum &curr_sum);
int** solve_kakuro_serial(int **sol_mat, int k, vector<COORD> &coords, const int &cs, const vector<vector<vector<sum *>>> &which_sums_cell_exists);
int** solution_serial(int **mat, int **sol_mat, vector<sum> sums, int m, int n, int size, int empty_cells);
int** solve_kakuro(int **sol_mat, int k, vector<COORD> &coords, const int &cs, const vector<vector<vector<sum *>>> &which_sums_cell_exists, COORD size);
int** solution(int **mat, int **sol_mat, vector<sum> sums, int m, int n, int size, int empty_cells);

#endif //KAKUROSOLVER_H
