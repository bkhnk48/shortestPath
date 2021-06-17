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
#include "BuiltInMap.cpp"
#include <cassert>  
#include <stdio.h>

//So we don't need to write std:: everywhere
using namespace std;

void getPathPlanning(K_Stack *stacks, int numOfStacks, int rowsInStack, int columnsInStack, BuildingPolygons* generator);

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

	bool loadBuiltInMap = false;

	fflush(stdin);
	printf("Do you want to load built-in map? (Y/y/1 or N/n/0) ");
	char load;
	int input = scanf("%c", &load);
	if(input == EOF)
		return;

	if(load == 'Y' || load == 'y' || load == '1'){
		printf("Which scenario you want? 0-9:  ");
		int scenario;	
		input = scanf("%d", &scenario);
		switch (scenario)
		{
			case 0:
				twoSeperatePolygons(stacks[0].slotsOfAV);
				break;
		
			default:
				break;
		}

		printf("Which wall do you want? 0-9:   ");
		input = scanf("%d", &scenario);
		switch (scenario)
		{
			case 0:
				twoSeperatePolygons(stacks[0].slotsOfAV);
				break;
		
			default:
				break;
		}
	}
	else{
		/*stacks[0].slotsOfAV[0][0] = 0;*/	stacks[0].slotsOfAV[0][1] = 0;  stacks[0].slotsOfAV[0][3] = 0;  stacks[0].slotsOfAV[0][4] = 0;  
																				stacks[0].slotsOfAV[0][6] = 0;  stacks[0].slotsOfAV[0][7] = 0;

		/*stacks[0].slotsOfAV[1][3] = 0;*/	stacks[0].slotsOfAV[1][6] = 0;  stacks[0].slotsOfAV[1][7] = 0;	
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
	}

	
    //vector< vector< lineSegment> > polygons;

    //readKStacks(polygons, stacks, numOfStacks, rowsInStack, columnsInStack);
	BuildingPolygons* generator = new RefinePolygons(rowsInStack, columnsInStack, 1, 33, widthOfAV, lengthOfAV);
	generator->NUMBER_STACKS = numOfStacks;
	generator->getRawPolygons(0, stacks[0].slotsOfAV);
	
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

	getPathPlanning(stacks, numOfStacks, rowsInStack, columnsInStack, generator);
	//assert(ones == 0);

	//assert(generator->polygons.size() == 2);
	//cout<<"\nstart: "<<start.x<<", "<<start.y<<endl;
	
	//point end(8, 0);
	//cout<<"End: "<<end.x<<", "<<end.y<<endl;
	
}

void getPathPlanning(K_Stack *stacks, int numOfStacks, int rowsInStack, int columnsInStack, BuildingPolygons* generator){

	int i = 0, j = 0, indexOfStack = 0;
	int gateNumber = 0;
	bool stopInput = false;
	int input;
	int directionAtTheEnd = 0;
	PlanningController* plan = new PlanningController();
	while(!stopInput && countAllStacks(stacks, numOfStacks, rowsInStack, columnsInStack) > 0){
		stopInput = true;
		fflush(stdin);
		printf("Type the number of stack: ");
		input = scanf("%d", &indexOfStack);
		if(input != EOF && indexOfStack >= 0 && indexOfStack < numOfStacks){
			fflush(stdin);
			printf("Type the row index of AV: ");
			input = scanf("%d", &i);
			if(input != EOF && i >= 0 && i < rowsInStack){
				fflush(stdin);
				printf("Type the column index of AV: ");
				input = scanf("%d", &j);
				if(input != EOF && j >= 0 && j < columnsInStack){
					if(stacks[indexOfStack].slotsOfAV[i][j] == 1 ||
						stacks[indexOfStack].slotsOfAV[i][j] == -1
						){
						point slot = generator->getSlot(indexOfStack, i, j);
						fflush(stdin);
						printf("Type the gate number (zero - 0 - for pick up an arbitrary point): ");
						input = scanf("%d", &gateNumber);
						if(input != EOF && gateNumber >= 0 && gateNumber <= 4){
							if(gateNumber == 0){
								fflush(stdin);
								printf("The direction from rear to center (4/8/6/2)?: ");
								input = scanf("%d", &directionAtTheEnd);
								if(input == EOF || 
										(directionAtTheEnd != 4 && directionAtTheEnd != 8 && directionAtTheEnd != 6 && directionAtTheEnd != 2))
								{
									cout<<"Wrong direction (4-Left/8-Up/6-Right/2-Down)"<<endl;
									break;
								}
							}
							fflush(stdin);
							printf("Do you want to get in (I/i/1/l) or get out (O/o/0)?");
							char Mode;
							input = scanf("%c", &Mode);
							if(input != EOF && (Mode == 'I' || Mode == 'O' || Mode == '1' || Mode == 'l' 
									|| Mode == 'i' || Mode == 'o' || Mode == '0')){
								
								stopInput = false;
								if(Mode == 'I' || Mode == 'i' || Mode == '1' || Mode == 'l'){
									point gate = generator->getPositionInGate(gateNumber, numOfStacks, true);
									plan->getTrajectory(generator, gate, slot, &directionAtTheEnd);
								}
								else{
									point gate = generator->getPositionInGate(gateNumber, numOfStacks, false);
									generator->removeEdgesAndVertices(indexOfStack, i, j);
									plan->getTrajectory(generator, slot, gate, &directionAtTheEnd);
								}
							}
							else{
								cout<<"Wrong mode! (Only I or O)"<<endl;
								break;
							}
						}
						else{
							cout<<"Wrong number of gate (only 1-4)!"<<endl;
							break;
						}
						
					}
					else{
						cout<<"No AV over there"<<endl;
					}
				}
				else{
					cout<<"Wrong value of column!"<<endl;
					break;
				}
			}
			else{
				cout<<"Wrong value of row!"<<endl;
				break;
			}
		}
		else{
			cout<<"Wrong index of stack!"<<endl;
			break;
		}
	}

	cout<<"Finish choosing path of AVs"<<endl;
}


int main(int argc, const char* argv[]){
	testFullAVsInStacks();
	return 0;
}