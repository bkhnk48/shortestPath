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
    int numOfStacks = 2; 
	int rowsInStack = 3;
	int columnsInStack = 35;
	int widthOfLine = 5;
	int widthOfAV = 2;
	int lengthOfAV = 6;

    K_Stack *stacks = NULL;
	stacks = (K_Stack *)malloc(numOfStacks*sizeof(K_Stack));
	stacks->k = numOfStacks;
	buildStack(stacks, numOfStacks, rowsInStack, columnsInStack);

    vector< vector< lineSegment> > polygons;

    readKStacks(polygons, stacks, numOfStacks, rowsInStack, columnsInStack);
}


int main(int argc, const char* argv[]){
    
}