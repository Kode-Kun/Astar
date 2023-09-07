#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::sort;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State {kEmpty, kObstacle, kClosed, kPath};
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line){
    istringstream stream(line);
    vector<State> v;
    int n;
    char a;
    while(stream >> n >> a && a == ','){
        if(n == 0){
            v.push_back(State::kEmpty);
        } else{
            v.push_back(State::kObstacle);
        }
    }
    return v;
}

vector<vector<State>> ReadBoardFile(string filename){
    ifstream file(filename);
    vector<vector<State>> vr;
    if(file){
        string line = "";
        while(getline(file, line)){
            vr.push_back(ParseLine(line));
        }
    }
    return vr;
}

bool Compare(const vector<int> a, const vector<int> b){
    int f1 = a[2] + a[3];
    int f2 = b[2] + b[3];
    return f1 > f2;
}

void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}


int Heuristic(int x1, int y1, int x2, int y2){
    return abs(x2 - x1) + abs(y2 - y1);
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid){
    bool on_grid_x = (x >= 0 && x < grid.size());
    bool on_grid_y = (y >= 0 && y < grid[0].size());
    if(on_grid_x && on_grid_y){
        return grid[x][y] == State::kEmpty;
    }
    return false;
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &open, vector<vector<State>> &grid){
    vector<int> node{x, y, g, h};
    open.push_back(node);
    grid[x][y] = State::kClosed;
}

void ExpandNeighbors(const vector<int> &current, vector<vector<int>> &open, vector<vector<State>> &grid, int goal[2]){
    int x = current[0];
    int y = current[1];
    int g = current[2];

    for(int i = 0; i < 4; i++){
        int x2 = x+delta[i][0];
        int y2 = y+delta[i][1];
        if(CheckValidCell(x2, y2, grid)){
            int g2 = g + 1;
            AddToOpen(x2, y2, g2, Heuristic(x2, y2, goal[0], goal[1]), open, grid);
        }
    }
}

vector<vector<State>> Search(vector<vector<State>> grid, int start[2], int goal[2]){
    vector<vector<int>> open{};
    int x = start[0];
    int y = start[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0], goal[1]);
    AddToOpen(x, y, g, h, open, grid);

    while(open.size() > 0){
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        grid[x][y] = State::kPath;

        if(x == goal[0] && y == goal[1]){
            return grid;
        }
        ExpandNeighbors(current, open, grid, goal);
    }
    
    cout << "No path found!\n";
    return vector<vector<State>> {};
}

string CellString(State cell){
    switch(cell){
        case State::kObstacle: return "X  ";
        case State::kPath: return "1  ";
        default: return "0  ";
    }
}

void PrintBoard(vector<vector<State>> v){
    for(int i = 0; i < v.size(); i++){
        for(int j = 0; j < v[i].size(); j++){
            cout << CellString(v[i][j]);
        }
        cout << "\n";
    }
}

int main(){
   int start[2]{0, 0};
   int goal[2]{4, 5};
   vector<vector<State>> board = ReadBoardFile("files/1.board");
   vector<vector<State>> solution = Search(board, start, goal);
   PrintBoard(solution);
}