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
    this->rowSize = rowSize;
    this->colSize = colSize;

    this->wallState = WallState();

    if (x != rowSize - 1) {
        pair right(x + 1, y);
        this->adjacentCells.emplace_back(right);
    }
    if (y != colSize - 1) {
        pair bottom(x, y + 1);
        this->adjacentCells.emplace_back(bottom);
    }
    if (x != 0) {
        pair left(x - 1, y);
        this->adjacentCells.emplace_back(left);
    }
    if (y != 0) {
        pair top(x, y - 1);
        this->adjacentCells.emplace_back(top);
    }
}

void Cell::resetCell() {
    dfs_visited = false;
    bfs_visited = false;
    selected = false;
}

vector<Cell *> Cell::get_adjacent_non_rdfs_visited_cells(const vector<vector<Cell *> > &cells) {
    vector<Cell *> adjacentUnvisitedCells;
    for (auto pair: adjacentCells) {
        Cell *cell = cells[pair.first][pair.second];
        if (!cell->rdfs_visited) {
            adjacentUnvisitedCells.emplace_back(cell);
        }
    }
    return adjacentUnvisitedCells;
}

Cell *Cell::get_random_non_rdfs_visited_cell(vector<vector<Cell *> > cells) {
    const vector<Cell *> adjacentUnvisitedCells = get_adjacent_non_rdfs_visited_cells(std::move(cells));
    if (adjacentUnvisitedCells.empty()) {
        return nullptr;
    }
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, adjacentUnvisitedCells.size() - 1);
    return adjacentUnvisitedCells[dist6(generator)];
}

vector<Cell *> Cell::get_adjacent_non_dfs_visited_open_cells(const vector<vector<Cell *> > &cells) const {
    vector<Cell *> adjacentOpenCells;
    for (auto pair: adjacentCells) {
        Cell *cell = cells[pair.first][pair.second];
        const int dx = cell->x - this->x;
        const int dy = cell->y - this->y;
        if (
            (dx == 1 && !this->wallState.right ||
             dx == -1 && !cell->wallState.right ||
             dy == 1 && !this->wallState.bottom ||
             dy == -1 && !cell->wallState.bottom) &&
            !cell->dfs_visited
        ) {
            adjacentOpenCells.emplace_back(cell);
        }
    }
    return adjacentOpenCells;
}

vector<Cell *> Cell::get_adjacent_non_bfs_visited_open_cells(const vector<vector<Cell *> > &cells) const {
    vector<Cell *> adjacentOpenCells;
    for (auto pair: adjacentCells) {
        Cell *cell = cells[pair.first][pair.second];
        const int dx = cell->x - this->x;
        const int dy = cell->y - this->y;
        if (
            (dx == 1 && !this->wallState.right ||
             dx == -1 && !cell->wallState.right ||
             dy == 1 && !this->wallState.bottom ||
             dy == -1 && !cell->wallState.bottom) &&
            !cell->bfs_visited
        ) {
            adjacentOpenCells.emplace_back(cell);
        }
    }
    return adjacentOpenCells;
}

void Cell::drawCell(wxDC &dc, const unsigned int dx, const unsigned int dy,
                    const unsigned short int selectionMargin = 10) const {
    const wxColor color = rdfs_visited ? Styles::White() : Styles::Grey10();

    unsigned short int x1 = dx + (x * Size);
    unsigned short int y1 = dy + (y * Size);

    dc.SetBrush(wxBrush(color));
    dc.SetPen(wxNullPen);

    dc.DrawRectangle(x1, y1, Size, Size);

    if (selected) {
        dc.SetBrush(wxBrush(Styles::LightRed()));
        const unsigned short int m = selectionMargin;
        dc.DrawRectangle(x1 + m, y1 + m, Size - 2 * m, Size - 2 * m);
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

Maze::Maze(unsigned int gx, unsigned int gy, unsigned int width, unsigned int height) {
    this->gx = gx;
    this->gy = gy;
    this->width = width;
    this->height = height;
    this->dfsState = MazeStateDFS();

    for (unsigned int i = 0; i < width; i++) {
        vector<Cell *> cellVec;
        for (unsigned int j = 0; j < height; j++) {
            Cell *cell = new Cell(i, j, width, height); //ignore
            cellVec.push_back(cell);
        }
        cells.push_back(cellVec);
    }
}

Maze::~Maze() {
    for (const auto &cellVec: cells) {
        for (const auto cell: cellVec) {
            delete cell;
        }
    }
}

void Maze::resetMaze() const {
    for(const auto& cellVec: cells) {
        for (const auto cell: cellVec) {
            cell->resetCell();
        }
    }
}

void Maze::drawMaze(wxDC &dc) const {
    for (const auto &cellVec: cells) {
        for (const auto &cell: cellVec) {
            cell->drawCell(dc, this->gx, this->gy);
        }
    }

    dc.SetPen(wxPen(Styles::Grey70(), 5));
    dc.SetBrush(wxNullBrush);
    constexpr int p = 0;
    dc.DrawRectangle(gx - p, gy - p, width * Cell::Size + 2 * p, height * Cell::Size + 2 * p);
}

void Maze::generateMazeRandomDFS() {
    if (
        this->randomDFSStateWalking.generated || this->randomDFSState.generated
    ) {
        this->randomDFSState.generated = false;
        return;
    }

    stack<Cell *> visitedCells;
    stack<Cell *> solution;
    bool flag = true;

    unsigned int num = height * width;
    Cell *current_cell = cells[0][0];
    while (num > 0) {
        if (!current_cell->rdfs_visited) {
            current_cell->rdfs_visited = true;
            if (flag) {
                solution.push(current_cell);
            }
            visitedCells.push(current_cell);
            if (current_cell->x == width - 1 && current_cell->y == height - 1) {
                flag = false;
            }
            num--;
        }

        Cell *next_cell = current_cell->get_random_non_rdfs_visited_cell(cells);

        if (next_cell != nullptr) {
            int dx = next_cell->x - current_cell->x;
            int dy = next_cell->y - current_cell->y;

            if (dx == 1) {
                current_cell->wallState.right = false;
            } else if (dy == 1) {
                current_cell->wallState.bottom = false;
            } else if (dx == -1) {
                next_cell->wallState.right = false;
            } else if (dy == -1) {
                next_cell->wallState.bottom = false;
            }

            current_cell = next_cell;
        } else {
            visitedCells.pop();
            if (flag) {
                solution.pop();
            }
            current_cell = visitedCells.top();
        }
    }
    this->randomDFSState.generate = false;
    this->randomDFSState.generated = true;
}

void Maze::generateMazeRandomDFSWalking() {
    auto &ms = this->randomDFSStateWalking;

    if (ms.generated || this->randomDFSState.generated) {
        ms.generate = false;
        return;
    }

    static int animationFlag = 0;

    Cell *current_cell = cells[ms.cIndex[0]][ms.cIndex[1]];

    if (ms.running()) {
        animationFlag = (animationFlag + 1) % 2;
        if (animationFlag > 0) {
            current_cell->selected = true;
            return;
        }

        if (!current_cell->rdfs_visited) {
            current_cell->rdfs_visited = true;
            if (!ms.distReached) {
                ms.solution.push(current_cell);
            }
            ms.visitedCells.push(current_cell);
            if (current_cell->x == width - 1 && current_cell->y == height - 1) {
                ms.distReached = true;
            }
            ms.count++;
        }

        Cell *next_cell = current_cell->get_random_non_rdfs_visited_cell(cells);

        if (next_cell != nullptr) {
            int dx = next_cell->x - current_cell->x;
            int dy = next_cell->y - current_cell->y;

            if (dx == 1) {
                current_cell->wallState.right = false;
            } else if (dy == 1) {
                current_cell->wallState.bottom = false;
            } else if (dx == -1) {
                next_cell->wallState.right = false;
            } else if (dy == -1) {
                next_cell->wallState.bottom = false;
            }
            ms.cIndex[0] = next_cell->x;
            ms.cIndex[1] = next_cell->y;
        } else {
            ms.visitedCells.pop();
            if (!ms.distReached) {
                ms.solution.pop();
            }
            Cell *temp = ms.visitedCells.top();
            ms.cIndex[0] = temp->x;
            ms.cIndex[1] = temp->y;
        }

        current_cell->selected = false;
    } else {
        ms.generate = false;
        ms.generated = true;
    }
}

void Maze::generateDFSAnswer() {

    if (dfsState.generated || bfsState.generated) {
        const auto temp = bfsState.message;
        bfsState = MazeStateBFS();
        bfsState.message = temp;

        dfsState = MazeStateDFS();
        dfsState.generate = true;
        resetMaze();
        return;
    }

    auto &ms = this->dfsState;


    static int animationFlag = 0;

    Cell *current_cell = cells[ms.cIndex[0]][ms.cIndex[1]];
    if (ms.running) {
        animationFlag = (animationFlag + 1) % 2;
        if (animationFlag > 0) {
            current_cell->selected = true;
            return;
        }

        if (!current_cell->dfs_visited) {
            current_cell->dfs_visited = true;
            ms.path.push(current_cell);
            if (current_cell->x == width - 1 && current_cell->y == height - 1) {
                ms.running = false;
                ms.message = "DFS: Found an answer at distance " + std::to_string(ms.path.size() - 1);
                return;
            }
        }

        auto adj_cells = current_cell->get_adjacent_non_dfs_visited_open_cells(cells);

        if (adj_cells.empty()) {
            current_cell->selected = false;
            ms.path.pop();

            if (ms.path.empty()) {
                // cout << "no answers" << endl;
                ms.running = false;
                ms.message = "DFS: couldn't find an answer";
                ms.generate = false;
                return;
            }

            Cell *temp = ms.path.top();
            ms.cIndex[0] = temp->x;
            ms.cIndex[1] = temp->y;
            return;
        }
        Cell *next_cell = adj_cells.front();

        ms.cIndex[0] = next_cell->x;
        ms.cIndex[1] = next_cell->y;
    } else {
        ms.generate = false;
        ms.generated = true;
    }
}

void Maze::generateBFSAnswer() {

    if (bfsState.generated || dfsState.generated) {
        const auto temp = dfsState.message;
        dfsState = MazeStateDFS();
        dfsState.message = temp;

        bfsState = MazeStateBFS();
        bfsState.generate = true;
        resetMaze();
        return;
    }

    auto &ms = bfsState;

    if (!ms.start) {
        Cell *temp = cells[ms.cIndex[0]][ms.cIndex[1]];
        ms.path.emplace(temp, ms.cIndex[2]);
        ms.start = true;
    }

    if (!ms.path.empty()) {
        auto current_cell = ms.path.front();
        ms.path.pop();

        current_cell.first->selected = true;
        current_cell.first->bfs_visited = true;

        if (
            current_cell.first->x == width - 1 && current_cell.first->y == height - 1
        ) {
            ms.generate = false;
            ms.generated = true;
            ms.message = "BFS: Found the answer at distance: " + std::to_string(current_cell.second);
            return;
        }

        auto adj = current_cell.first->get_adjacent_non_bfs_visited_open_cells(cells);

        for (const auto &adj_cell : adj) {
            ms.path.emplace(adj_cell, current_cell.second+1);
        }

    } else {
        ms.generate = false;
        ms.generated = true;
        ms.message = "BFS: couldn't find an answer";
    }
}
