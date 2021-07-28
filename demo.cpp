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
	refine.points.push_back(point(20,20));
	refine.points.push_back(point(21,20));
	refine.points.push_back(point(21,17));
	refine.points.push_back(point(22,17));
	refine.points.push_back(point(22,20));
	refine.points.push_back(point(23,20));
	refine.points.push_back(point(23,17));
	refine.points.push_back(point(25,17));
	refine.points.push_back(point(25,20));
	refine.points.push_back(point(28,20));
	refine.points.push_back(point(28,17));
	refine.points.push_back(point(26,17));
	refine.points.push_back(point(26,14));
	refine.points.push_back(point(28,14));
	refine.points.push_back(point(28,2));
	refine.points.push_back(point(24,2));
	refine.points.push_back(point(24,5));
	refine.points.push_back(point(23,5));
	refine.points.push_back(point(23,8));
	refine.points.push_back(point(22,8));
	refine.points.push_back(point(22,2));
	refine.points.push_back(point(21,2));
	refine.points.push_back(point(21,8));
	refine.points.push_back(point(20,8));

	refine.polygons.resize(1);
	refine.polygons.at(0).push_back(lineSegment(point(20,20), point(21,20)));
	refine.polygons.at(0).push_back(lineSegment(point(21,20), point(21,17)));
	refine.polygons.at(0).push_back(lineSegment(point(21,17), point(22,17)));
	refine.polygons.at(0).push_back(lineSegment(point(22,17), point(22,20)));
	refine.polygons.at(0).push_back(lineSegment(point(22,20), point(23,20)));
	refine.polygons.at(0).push_back(lineSegment(point(23,20), point(23,17)));
	refine.polygons.at(0).push_back(lineSegment(point(23,17), point(25,17)));
	refine.polygons.at(0).push_back(lineSegment(point(25,17), point(25,20)));
	refine.polygons.at(0).push_back(lineSegment(point(25,20), point(28,20)));
	refine.polygons.at(0).push_back(lineSegment(point(28,20), point(28,17)));
	refine.polygons.at(0).push_back(lineSegment(point(28,17), point(26,17)));
	refine.polygons.at(0).push_back(lineSegment(point(26,17), point(26,14)));
	refine.polygons.at(0).push_back(lineSegment(point(26,14), point(28,14)));
	refine.polygons.at(0).push_back(lineSegment(point(28,14), point(28,2)));
	refine.polygons.at(0).push_back(lineSegment(point(28,2), point(24,2)));
	refine.polygons.at(0).push_back(lineSegment(point(24,2), point(24,5)));
	refine.polygons.at(0).push_back(lineSegment(point(24,5), point(23,5)));
	refine.polygons.at(0).push_back(lineSegment(point(23,5), point(23,8)));
	refine.polygons.at(0).push_back(lineSegment(point(23,8), point(22,8)));
	refine.polygons.at(0).push_back(lineSegment(point(22,8), point(22,2)));
	refine.polygons.at(0).push_back(lineSegment(point(22,2), point(21,2)));
	refine.polygons.at(0).push_back(lineSegment(point(21,2), point(21,8)));
	refine.polygons.at(0).push_back(lineSegment(point(21,8), point(20,8)));
	refine.polygons.at(0).push_back(lineSegment(point(20,8), point(20,20)));

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

	for(int i=0; i<refine.polygons.at(0).size(); i++){
		cout << refine.polygons.at(0).at(i).p.x << " " << refine.polygons.at(0).at(i).p.y << " | ";
		cout << refine.polygons.at(0).at(i).q.x << " " << refine.polygons.at(0).at(i).q.y << endl;
	}

	cout << "END" << endl;
	for(int i=0; i<refine.polygons.at(1).size(); i++){
		cout << refine.polygons.at(1).at(i).p.x << " " << refine.polygons.at(1).at(i).p.y << " | ";
		cout << refine.polygons.at(1).at(i).q.x << " " << refine.polygons.at(1).at(i).q.y << endl;
	}
}
