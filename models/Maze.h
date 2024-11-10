#ifndef MAZE_H
#define MAZE_H
#include <wx/dc.h>

#include "states/MazeState.h"

using namespace std;

class WallState {
    public:
    bool top;
    bool left;
    bool bottom;
    bool right;
    WallState() {
        top = true;
        left = true;
        bottom = true;
        right = true;
    }
};

class Cell {

public:
    Cell(unsigned int x, unsigned int y, unsigned int rowSize, unsigned int colSize);

    static constexpr unsigned short int Size = 25;

    unsigned int x, y;
    unsigned int rowSize, colSize;

    bool rdfs_visited = false;
    bool dfs_visited = false;
    bool bfs_visited = false;
    bool selected = false;

    WallState wallState;
    vector<pair<int, int>> adjacentCells;

    void resetCell();
    void drawCell(wxDC &dc, unsigned int dx, unsigned int dy, unsigned short int selectionMargin) const;
    [[nodiscard]] vector<Cell*> get_adjacent_non_rdfs_visited_cells(const vector<vector<Cell*>> &cells);
    [[nodiscard]] Cell* get_random_non_rdfs_visited_cell(vector<vector<Cell*>> cells);

    [[nodiscard]] vector<Cell*> get_adjacent_non_dfs_visited_open_cells(const vector<vector<Cell*>> &cells) const;
    [[nodiscard]] vector<Cell*> get_adjacent_non_bfs_visited_open_cells(const vector<vector<Cell*>> &cells) const;
};


class Maze {
public:
    Maze(unsigned int gx, unsigned int gy, unsigned int width, unsigned int height);
    ~Maze();
    unsigned int gx, gy, width, height;

    MazeStateRandomDFS randomDFSState;
    MazeStateRandomDFSWalking randomDFSStateWalking{&this->width, &this->height};
    MazeStateDFS dfsState;
    MazeStateBFS bfsState;

    vector<vector<Cell*>> cells;

    void resetMaze() const;
    void drawMaze(wxDC& dc) const;
    void generateMazeRandomDFS();
    void generateMazeRandomDFSWalking();
    void generateDFSAnswer();
    void generateBFSAnswer();
};



#endif //MAZE_H
