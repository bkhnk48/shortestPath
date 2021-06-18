#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
#include "Constant.cpp"

#include <cassert>  
#include <stdio.h>

//So we don't need to write std:: everywhere
using namespace std;

void twoSeperatePolygons(int** slots){
    int ROWS = 6;
    int COLS = 21;

    printf("Two independent polygons in both sides of stack[0]:\n");

    int arr[6][21] 
    = {
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0,
    };

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            slots[i][j] = arr[i][j];
        }
    }
}

int*** loadWall(int ROWS, int COLS, int WIDTH_OF_GATE, int WIDTH_SLOT, int LENGTH_SLOT, int numOfStacks = 1, int WIDTH_LINE = 10){
    int numOfWalls = 3;
    int numOfVertices = 6;
    int*** walls = (int ***)malloc(sizeof * walls * numOfWalls);
    for(int i = 0; i < numOfWalls; i++){
        walls[i] = (int **)malloc(sizeof * walls[i] * numOfVertices);
        for(int j = 0; j < numOfVertices; j++){
            walls[i][j] = (int *)malloc(sizeof * walls[i][j] * 3);
            walls[i][j][0] = 0;
            walls[i][j][1] = 0;
            walls[i][j][2] = HENCE;
        }
    }

     
    walls[0][0][0] = -WIDTH_LINE;   walls[0][0][1] = -WIDTH_LINE;
    walls[0][1][0] = (COLS*WIDTH_SLOT - WIDTH_OF_GATE*WIDTH_SLOT + WIDTH_LINE)/2;    walls[0][1][1] = -WIDTH_LINE;
    walls[0][2][0] = walls[0][1][0];    walls[0][2][1] = walls[0][1][1] - 1;
    walls[0][3][0] = walls[0][0][0] - 1;   walls[0][3][1] = walls[0][0][1] - 1;
    walls[0][4][0] = walls[0][3][0];   walls[0][4][1] = walls[0][0][1] + (numOfStacks*ROWS*LENGTH_SLOT - WIDTH_OF_GATE*WIDTH_SLOT + (numOfStacks + 1)*WIDTH_LINE)/2;
    walls[0][5][0] = walls[0][4][0] + 1;   walls[0][5][1] = walls[0][4][1];
    /*Clockwise
 (0, 4) __   (0, 5)
       | |
       | |
       | |(0, 0)
       | |___________ (0, 1)
       |_____________| 
    (0, 3)            (0, 2)
    */

    
    walls[1][0][0] = -WIDTH_LINE - 1;    walls[1][0][1] = numOfStacks*ROWS*LENGTH_SLOT + (numOfStacks)*WIDTH_LINE + 1;
    walls[1][1][0] = -WIDTH_LINE + (COLS*WIDTH_SLOT - WIDTH_OF_GATE*WIDTH_SLOT + 2*WIDTH_LINE)/2;    walls[1][1][1] = walls[1][0][1];
    walls[1][2][0] = walls[1][1][0];    walls[1][2][1] = walls[1][1][1] - 1;
    walls[1][3][0] = -WIDTH_LINE;   walls[1][3][1] = walls[1][2][1];
    walls[1][4][0] = -WIDTH_LINE;   walls[1][4][1] = walls[0][5][1] + WIDTH_OF_GATE*WIDTH_SLOT;
    walls[1][5][0] = walls[1][4][0] - 1;   walls[1][5][1] = walls[1][4][1] ;
    
    /*
        Clockwise
       (1, 0)
        _____________  (1, 1)
        |  __________|
        | |(1, 3)     (1, 2)
        | |
        | |
        |_|
  (1, 5)   (1, 4)
    */

    walls[2][0][0] = (COLS*WIDTH_SLOT + WIDTH_OF_GATE*WIDTH_SLOT)/2;    walls[2][0][1] = walls[1][0][1];
    walls[2][1][0] = WIDTH_LINE + (COLS*WIDTH_SLOT) + 1;    walls[2][1][1] = walls[2][0][1];
    walls[2][2][0] = walls[2][1][0];    walls[2][2][1] = walls[1][4][1];
    walls[2][3][0] = walls[2][2][0] - 1;   walls[2][3][1] = walls[2][2][1];
    walls[2][4][0] = walls[2][3][0];   walls[2][4][1] = walls[1][3][1];
    walls[2][5][0] = walls[2][0][0];   walls[2][5][1] = walls[2][4][1] ;
    
    /*
        Clockwise
                  
  (2, 0) _____________  (2, 1)
        |__________  |
  (2, 5)     (2, 4)| |
                   | |
                   | |
                   |_|
             (2, 3)   (2, 2)
    */

    return walls;
}

