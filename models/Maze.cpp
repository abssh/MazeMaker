//
// Created by deboom on 11/9/24.
//

#include "Maze.h"

#include <iostream>
#include <utility>
#include <random>
#include <stack>

#include "Styles.h"

Cell::Cell(unsigned int x, unsigned int y, unsigned int rowSize, unsigned int colSize) {
    this->x = x;
    this->y = y;
    this->visited = false;
    this->selected = false;
    this->rowSize = rowSize;
    this->colSize = colSize;

    this->wallState = WallState();

    if (x != 0) {
        pair left(x-1, y);
        this->adjacentCells.emplace_back(left);
    }
    if (y != 0) {
        pair top(x, y - 1);
        this->adjacentCells.emplace_back(top);
    }
    if (x != rowSize - 1) {
        pair right(x+1, y);
        this->adjacentCells.emplace_back(right);
    }
    if (y != colSize - 1) {
        pair bottom(x, y + 1);
        this->adjacentCells.emplace_back(bottom);
    }
}

void Cell::selectCell() {
    this-> selected = true;
}

vector<Cell *> Cell::getAdjacentUnvisitedCells(vector<vector<Cell *>> cells) {
    vector<Cell *> adjacentUnvisitedCells;
    for (auto pair : adjacentCells) {
        Cell* cell = cells[pair.first][pair.second];
        if (!cell->visited) {
            adjacentUnvisitedCells.emplace_back(cell);
        }
    }
    return adjacentUnvisitedCells;
}

Cell * Cell::getRandomUnvisitedAdjacentCell(vector<vector<Cell *>> cells) {
    const vector<Cell *> adjacentUnvisitedCells = getAdjacentUnvisitedCells(std::move(cells));
    if (adjacentUnvisitedCells.empty()) {
        return nullptr;
    }
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,adjacentUnvisitedCells.size()-1);
    return adjacentUnvisitedCells[dist6(generator)];
}

Maze::Maze(unsigned int gx, unsigned int gy, unsigned int width, unsigned int height) {
    this->gx = gx;
    this->gy = gy;
    this->width = width;
    this->height = height;

    for (unsigned int i = 0; i < width; i++) {
        vector<Cell *> cellVec;
        for (unsigned int j = 0; j < height; j++) {
            Cell *cell = new Cell(i, j, width, height); //ignore
            cellVec.push_back(cell);
        }
        cells.push_back(cellVec);
    }
}

void Cell::drawCell(wxDC &dc, const unsigned int dx, const unsigned int dy,
                    const unsigned short int selectionMargin = 10) const {

    const wxColor color = visited ? Styles::White() : Styles::Grey10();

    unsigned short int x1 = dx + (x * Size);
    unsigned short int y1 = dy + (y * Size);

    dc.SetBrush(wxBrush(color));
    dc.SetPen(wxNullPen);

    dc.DrawRectangle(x1, y1, Size, Size);

    if (selected) {
        dc.SetBrush(wxBrush(Styles::LightRed()));
        const unsigned short int m = selectionMargin;
        dc.DrawRectangle(x1+m, y1+m, Size - 2*m, Size - 2*m);
    }

    wxPen pen(Styles::Grey70(), 5);
    pen.SetCap(wxCAP_BUTT);
    dc.SetPen(pen);

    if (wallState.bottom && y != colSize - 1) {
        dc.DrawLine(x1, y1 + Size, x1 + Size, y1 + Size);
    }
    if (wallState.right && x != rowSize - 1) {
        dc.DrawLine(x1 + Size, y1, x1 + Size, y1 + Size);
    }
}

Maze::~Maze() {
    for (const auto& cellVec : cells) {
        for (const auto cell : cellVec) {
            delete cell;
        }
    }
}

void Maze::drawMaze(wxDC &dc) {
    for (const auto &cellVec: cells) {
        for (const auto &cell: cellVec) {
            cell->drawCell(dc, this->gx, this->gy);
        }
    }

    dc.SetPen(wxPen(Styles::Grey70(), 5));
    dc.SetBrush(wxNullBrush);
    const int p = 0;
    dc.DrawRectangle(gx- p, gy -p, width*Cell::Size + 2*p, height*Cell::Size +2*p);
}

void Maze::generateMaze_DFS() {
    stack<Cell*> visitedCells;
    stack<Cell*> solution;
    bool flag = true;

    int num = height * width;
    Cell* current_cell = cells[0][0];
    while (num > 0) {
        if (!current_cell->visited) {
            current_cell->visited = true;
            if (flag) {
                solution.push(current_cell);
            }
            visitedCells.push(current_cell);
            if (current_cell->x == width - 1 && current_cell->y == height - 1) {
                flag = false;
            }
            num--;
        }

        Cell* next_cell = current_cell->getRandomUnvisitedAdjacentCell(cells);

        if  (next_cell != nullptr) {

            int dx = next_cell->x - current_cell->x;
            int dy = next_cell->y - current_cell->y;

            if (dx == 1) {
                current_cell->wallState.right = false;
            }
            else if (dy == 1) {
                current_cell->wallState.bottom = false;

            }
            else if (dx == -1) {

                next_cell->wallState.right = false;
            }
            else if (dy == -1) {

                next_cell->wallState.bottom = false;
            }

            current_cell = next_cell;
        }
        else {
            visitedCells.pop();
            if (flag) {
                solution.pop();
            }
            current_cell = visitedCells.top();
        }
    }
    while (solution.empty() == false) {
        Cell* cell = solution.top();
        cell->selected = true;
        solution.pop();
    }
}
