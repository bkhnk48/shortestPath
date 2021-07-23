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

//#include "draw.cpp"
//#include "naive.cpp"
//#include "preprocess.cpp"
#include "RefinePolygons.cpp"

int main(int argc, const char* argv[]){
	RefinePolygons refine =  RefinePolygons(10,20,1,1,1,3);
	refine.points.push_back(point(5,10));
	refine.points.push_back(point(8,10));
	refine.points.push_back(point(8,4));
	refine.points.push_back(point(10,4));
	refine.points.push_back(point(10,10));
	refine.points.push_back(point(16,10));
	refine.points.push_back(point(16,1));
	refine.points.push_back(point(5,1));

	refine.polygons.resize(1);
	refine.polygons.at(0).push_back(lineSegment(point(5,10), point(8,10)));
	refine.polygons.at(0).push_back(lineSegment(point(8,10), point(8,4)));
	refine.polygons.at(0).push_back(lineSegment(point(8,4), point(10,4)));
	refine.polygons.at(0).push_back(lineSegment(point(10,4), point(10,10)));
	refine.polygons.at(0).push_back(lineSegment(point(10,10), point(16,10)));
	refine.polygons.at(0).push_back(lineSegment(point(16,10), point(16,1)));
	refine.polygons.at(0).push_back(lineSegment(point(16,1), point(5,1)));
	refine.polygons.at(0).push_back(lineSegment(point(5,1), point(5,10)));

	cout << "START____________:" << endl;
	cout << "Number of point : " << refine.points.size() << endl;
	cout << "Number of line  : " << refine.polygons.at(0).size() << endl;
	cout << "-----------------processing----------------------" << endl;

	refine.removeEdgesAndVertices(0,0,0);

	if(refine.polygons.size() > 0){
		cout << "Number of point : " << refine.points.size() << endl;
		cout << "Number of line  : " << refine.polygons.at(0).size() << endl;
		cout << "-----------------processing.....----------------------" << endl;
	} else {
		cout << "Clean all" << endl;
	}

	cout << "Details: " << endl;
	cout << refine.polygons.size() << endl;
	for(int i=0; i<refine.polygons.at(0).size(); i++){
		cout << refine.polygons.at(0).at(i).p.x << " " << refine.polygons.at(0).at(i).p.y << " | ";
		cout << refine.polygons.at(0).at(i).q.x << " " << refine.polygons.at(0).at(i).q.y << endl;
	}
}
