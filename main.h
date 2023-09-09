#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <string>
using std::vector;
using std::string;

enum class State;

//the functions relating to parsing, formating and printing out the board
vector<State> ParseLine(string);
vector<vector<State>> ReadBoardFile(string);
string CellString(State);
void PrintBoard(vector<vector<State>>);

//the functions related to the actual A* Search algorithm
vector<vector<State>> Search(vector<vector<State>> grid, int start[2], int goal[2]);
void ExpandNeighbors(const vector<int>&, vector<vector<int>>&, vector<vector<State>>&, int[2]);
bool CheckValidCell(int, int, vector<vector<State>>&);
void AddToOpen(int, int, int, int, vector<vector<int>>&, vector<vector<State>>&);
bool Compare(const vector<int>, const vector<int>);
void CellSort(vector<vector<int>>*);
int Heuristic(int, int, int, int);

#endif