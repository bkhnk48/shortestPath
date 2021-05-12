#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>


//So we don't need to write std:: everywhere
using namespace std;

struct {
	bool printGraph = false;
	bool drawRoute = true;
	int printLevel = 0;
	size_t k = 0;
} config;

//point type for holding a coordinate 
void setMinMax(double x, double y);
struct point {
	double x;
	double y;
	point(double x,double y){
		this->x = x;
		this->y = y;
		setMinMax( x, y);
	}

	point(){

	}
	bool operator==(const point &other) const {
		return (this->x==other.x && this->y==other.y);
	}
};
//Type used in the priority queue in the dijkstra function
typedef std::pair<double,std::pair<int,int> > pq_pair;

//linesegment type holding two points
struct lineSegment {
	point p;
	point q;
	lineSegment(point p,point q){
		this->p = p;
		this->q = q;
	};
	lineSegment(){

	}
	bool operator==(const lineSegment &other) const {
		return (this->p==other.p && this->q==other.q) ||
		(this->p==other.q && this->q==other.p);
  }
};

#include "draw.cpp"
#include "naive.cpp"
#include "preprocess.cpp"





int main(int argc, const char* argv[]){
	//local variables
	int numOfStacks = 2; 
	int rowsInStack = 3;
	int columnsInStack = 35;
	int widthOfLine = 5;
	int widthOfAV = 2;
	int lengthOfAV = 6;

	getData(&numOfStacks, &rowsInStack, &columnsInStack, 
				&widthOfLine, &widthOfAV, &lengthOfAV);

	

	//vector<point> trajectory = getTrajectory(argc, argv);
	return 0;
}