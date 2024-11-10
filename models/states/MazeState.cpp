//
// Created by deboom on 11/10/24.
//

#include "MazeState.h"

MazeStateRandomDFSWalking::MazeStateRandomDFSWalking(unsigned int *width, unsigned int *height) {
    this->height = height;
    this->width = width;
    count = 0;
    distReached = false;
}


bool MazeStateRandomDFSWalking::running() const {
    unsigned int num =  *width * *height - count;
    return num > 0;
}
