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
//#include "preprocess.cpp"
#include "MovementController.cpp"
#include "PathPlanning.cpp"
#include <cassert>  
#include <stdio.h>

//So we don't need to write std:: everywhere
using namespace std;

void testFullAVsInStacks(){
    int numOfStacks = 1; 
	int rowsInStack = 6;
	int columnsInStack = 21;
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
	

					//stacks[0].slotsOfAV[5][1] = 0;  stacks[0].slotsOfAV[5][4] = 0;  stacks[0].slotsOfAV[5][5] = 0;

	//stacks[0].slotsOfAV[1][0] = 0;  stacks[0].slotsOfAV[2][0] = 0;  stacks[0].slotsOfAV[3][0] = 0;


	for(int i = 0; i < 6; i++){
		for(int j = 7; j < columnsInStack; j++){
			stacks[0].slotsOfAV[i][j] = 0;
		}
	}
	stacks[0].slotsOfAV[0][9] = 1;
    //vector< vector< lineSegment> > polygons;

    //readKStacks(polygons, stacks, numOfStacks, rowsInStack, columnsInStack);
	BuildingPolygons* generator = new BuildingPolygons(rowsInStack, columnsInStack, 1, 33, widthOfAV, lengthOfAV);
	generator->check(stacks[0].slotsOfAV, 0, 0);
	
	printStacks(stacks, numOfStacks, rowsInStack, columnsInStack);

	int ones = 0;
	for(int k = 0; k < numOfStacks; k++){
		for(int i = 0; i < rowsInStack; i++){
			for(int j = 0; j < columnsInStack; j++){
				if(stacks[k].slotsOfAV[i][j] == 1)
					ones++;
			}
		}
	}
	assert(ones == 0);

	//assert(generator->polygons.size() == 2);

	point start = generator->getSlot(0, 0);
	cout<<"\nstart: "<<start.x<<", "<<start.y<<endl;
	point end = generator->getPositionInGate(3, numOfStacks, false);
	//point end(8, 0);
	cout<<"End: "<<end.x<<", "<<end.y<<endl;

	PlanningController* plan = new PlanningController();
	plan->getTrajectory(generator->points, generator->polygons, start, end);

	
}


int main(int argc, const char* argv[]){
	testFullAVsInStacks();
}