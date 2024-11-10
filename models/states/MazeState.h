//
// Created by deboom on 11/10/24.
//

#ifndef MAZESTATE_H
#define MAZESTATE_H
#include <queue>
#include <stack>
#include <string>
#include <vector>

class Cell;

class MazeStateRandomDFS {
public:
    bool generate = false;
    bool generated = false;
};

class MazeStateRandomDFSWalking {
public:
    bool generate = false;
    bool generated = false;

    unsigned int *height, *width;
    unsigned int count;
    std::stack<Cell*> visitedCells;
    std::stack<Cell*> solution;
    unsigned int cIndex[2] = {0, 0};
    bool distReached;


    MazeStateRandomDFSWalking(unsigned int *width, unsigned int *height);

    [[nodiscard]] bool running() const;
};

class MazeStateDFS {
public:
    bool generate = false;
    bool generated = false;

    std::stack<Cell*>  path;
    unsigned int cIndex[2] = {0, 0};
    std::string message;
    bool running = true;
};

class MazeStateBFS {
    public:
    bool generate = false;
    bool generated = false;

    bool start = false;
    std::queue<std::pair<Cell*, int>> path;
    unsigned int cIndex[3] = {0, 0, 0};
    std::string message;
    bool running = true;
};



#endif //MAZESTATE_H
