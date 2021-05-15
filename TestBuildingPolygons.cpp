#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
//#include "BuildPolygonState.cpp"
#include "draw.cpp"
#include "naive.cpp"
#include "preprocess.cpp"

//So we don't need to write std:: everywhere
using namespace std;

void testFullAVsInStacks(){
    int numOfStacks = 1; 
	int rowsInStack = 6;
	int columnsInStack = 8;
	int widthOfLine = 5;
	int widthOfAV = 2;
	int lengthOfAV = 6;

    K_Stack *stacks = NULL;
	stacks = (K_Stack *)malloc(numOfStacks*sizeof(K_Stack));
	stacks->k = numOfStacks;
	buildStack(stacks, numOfStacks, rowsInStack, columnsInStack);
	stacks[0].slotsOfAV[0][0] = 0;	stacks[0].slotsOfAV[0][1] = 0;  stacks[0].slotsOfAV[0][3] = 0;  stacks[0].slotsOfAV[0][4] = 0;  
																				stacks[0].slotsOfAV[0][6] = 0;  stacks[0].slotsOfAV[0][7] = 0;

	stacks[0].slotsOfAV[1][3] = 0;	stacks[0].slotsOfAV[1][6] = 0;  stacks[0].slotsOfAV[1][7] = 0;																		
	stacks[0].slotsOfAV[2][7] = 0;
	stacks[0].slotsOfAV[3][7] = 0;
	stacks[0].slotsOfAV[4][0] = 0;	stacks[0].slotsOfAV[4][2] = 0;  stacks[0].slotsOfAV[4][6] = 0;  stacks[0].slotsOfAV[4][7] = 0;
	stacks[0].slotsOfAV[5][0] = 0;	stacks[0].slotsOfAV[5][2] = 0;  stacks[0].slotsOfAV[5][3] = 0;  stacks[0].slotsOfAV[5][6] = 0;  
																				stacks[0].slotsOfAV[5][7] = 0;

    //vector< vector< lineSegment> > polygons;

    //readKStacks(polygons, stacks, numOfStacks, rowsInStack, columnsInStack);
	BuildingPolygons* polygons = new BuildingPolygons(rowsInStack, columnsInStack, 1, 33, widthOfAV, lengthOfAV);
	polygons->check(stacks[0].slotsOfAV, 0, 0, rowsInStack, columnsInStack);
	
	printStacks(stacks, numOfStacks, rowsInStack, columnsInStack);
}


int main(int argc, const char* argv[]){
	testFullAVsInStacks();
}