//
// Created by deboom on 11/9/24.
//

#ifndef MAZE_H
#define MAZE_H
#include <wx/dc.h>

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

    static constexpr unsigned short int Size = 30;

    unsigned int x, y;
    unsigned int rowSize, colSize;
    bool visited;
    bool selected;
    WallState wallState;
    vector<pair<int, int>> adjacentCells;

    void selectCell();
    void drawCell(wxDC &dc, unsigned int dx, unsigned int dy, unsigned short int selectionMargin) const;
    vector<Cell*> getAdjacentUnvisitedCells(vector<vector<Cell*>> cells);
    Cell* getRandomUnvisitedAdjacentCell(vector<vector<Cell*>> cells);
};


class Maze {
public:
    Maze(unsigned int gx, unsigned int gy, unsigned int width, unsigned int height);
    ~Maze();
    unsigned int gx, gy, width, height;

    vector<vector<Cell*>> cells;

    void drawMaze(wxDC& dc);
    void generateMaze_DFS();
};



#endif //MAZE_H
