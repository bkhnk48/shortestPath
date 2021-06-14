//#define _USE_MATH_DEFINES

#include <iostream>
#include <sstream> //ostringstream
#include <fstream> 
#include <math.h>
#include <vector> //vector
#include <float.h>
#include <string>
#include <stdlib.h>
#include "Constant.cpp"

#ifndef _DRAW_
#define _DRAW_

using namespace std;

struct {
	bool printGraph = false;
	bool drawRoute = true;
	int printLevel = 0;
	size_t k = 0;
} config;

typedef struct Stacks{
	int** slotsOfAV;
	int k;
}K_Stack;

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

struct Trajectory{
	point position;
	double t;

	Trajectory(double x, double y, double t){
		this->position.x = x;
		this->position.y = x;
		this->t = t;
	}

};

class RawRoute{
    public:
        vector<point> points;
        
        RawRoute(){

        }

        void insert(point p){
            points.insert(points.begin(), p);
        }
};

struct Slot {
	int row;
	int column;
	Slot(int x, int y){
		this->row = x;
		this->column = y;
	}

	Slot(){

	}

	bool operator==(const Slot &other) const {
		return (this->row == other.row && this->column == other.column);
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

	bool contains(point c){
		return (c == p) || (c == q); 
	}
};

//Each instance of movement point is the point which AV should reach before a time
class MovementPoint{
    public:
        double beganX;
        double beganY;
        double endedX;
        double endedY;
};

//A section is a small trajectory which routes AV to move (both forward and backward) from
//a point to other one. 
class Section : public MovementPoint{
    public:
        char typeOfTrajectory ;
        char steering;
        double param;
        vector<point> possiblePoints;
		/*void setLastVelocity(double x1, double y1, double x2, double y2){
			lastVelocity.p.x = x1;
			lastVelocity.p.y = y1;
			lastVelocity.q.x = x2;
			lastVelocity.q.y = y2;
		}
		void setLastVelocity(double deltaX, double deltaY){
			lastVelocity.p.x = this->endedX;
			lastVelocity.p.y = this->endedY;
			lastVelocity.q.x = this->endedX + deltaX;
			lastVelocity.q.y = this->endedY + deltaY;
		}
		lineSegment getLastVelocity(){
			return lastVelocity;
		}

		void setFirstVelocity(double x1, double y1, double x2, double y2){
			firstVelocity.p.x = x1;
			firstVelocity.p.y = y1;
			firstVelocity.q.x = x2;
			firstVelocity.q.y = y2;
		}
		void setFirstVelocity(double deltaX, double deltaY){
			firstVelocity.p.x = this->beganX;
			firstVelocity.p.y = this->beganY;
			firstVelocity.q.x = this->beganX + deltaX;
			firstVelocity.q.y = this->beganY + deltaY;
		}
		lineSegment getFirstVelocity(){
			return firstVelocity;
		}*/
	private:
		//lineSegment firstVelocity;
		//lineSegment lastVelocity;
};

//A path segment is a group of continuous section to move from one point to other one.
class PathSegment : public MovementPoint{
    public:
        vector<Section*> sections;
        float L;
		//lineSegment firstVelocity;
		//lineSegment lastVelocity;
        //int index;

        PathSegment(){
            //this->index = -1;
            this->L = 0;
        }
};

//A path is a group of possible path segment and its best choice to move from
//one point to other one
class Path : public MovementPoint{
    public:
        vector<PathSegment*> segments;
        float Lmin;
        int index;

        Path(){
            this->Lmin = FLT_MAX;
            this->index = -1;
        }
};

class Range{
    public:
        double xMax;
        double xMin;
        double yMax;
        double yMin;

        Range(){
            this->xMax = FLT_MIN;
            this->yMax = FLT_MIN;
            this->xMin = FLT_MAX;
            this->yMin = FLT_MAX;
        }
};

double max_y,max_x,min_y,min_x;

void setMinMax(double x, double y){
	if(max_y<y){
		max_y=y;
	}
	if(max_x<x){
		max_x=x;
	}
	if(min_y>y){
		min_y=y;
	}
	if(min_x>x){
		min_x=x;
	}
}

string drawX(point &p){
	double r = p.x*10;
	return to_string(r);
}

string drawY(point &p){
	double r = p.y*-10;
	return to_string(r);
}

//we define the "normal in" vector v of vector u is a vector such that
//(u^v) has direction to go in the plan of (u, v)
//deltaX, deltaY is coords of vector u
void getNormalInAndOut(double deltaX, double deltaY, double *xIn, double *yIn, double *xOut = NULL, double *yOut = NULL)
{
	if(deltaX == 0){
		*xIn = (deltaY > 0 ? 1 : -1); *yIn = 0;
		
		if(xOut && yOut){
			*xOut = -(*xIn); *yOut = 0;
		}
	}
	else if(deltaY == 0){
		*xIn = 0; *yIn = (deltaX > 0 ? -1 : 1);

		if(xOut && yOut){
			*xOut = 0; *yOut = -(*yIn);
		}
	}
	else{
		
		double length = sqrt(deltaX*deltaX + deltaY*deltaY);
		
		*xIn = deltaY/length; *yIn = -deltaX/length;
		if(xOut && yOut){
			*xOut = -deltaY/length; *yOut = deltaX/length;
		}
	}
}

string drawRoute(vector<int> & route, vector<point> & points){
	string str = "<polyline stroke='red' stroke-width='2' fill='none' points='";
	int current = route.size()-1;
	while(current != -1){
		point p = points[current%points.size()];
		str.append(drawX(p));
		str.append(",");
		str.append(drawY(p));
		str.append(" ");
		//cout<<"\nroute["<<current<<"] = "<<route[current]<<" => ("<<p.x<<", "<<p.y<<")";
		current = route[current];
	}
	str.append("'/>\n");

	
	return str;
}

string drawShortestRoute(vector<point> & route//, vector<point> & sideSteps
							){
	string str = "<polyline stroke='red' stroke-width='2' fill='none' points='";
	int current = route.size()-1;
	while(current != -1){
		point p = //points[current%points.size()];
					route.at(current);
		str.append(drawX(p));
		str.append(",");
		str.append(drawY(p));
		str.append(" ");
		//cout<<"\nroute["<<current<<"] = ("<<p.x<<", "<<p.y<<")";
		//current = route[current];
		current--;
	}
	str.append("'/>\n");

	
	return str;
}



string drawPoint(point &p,string color){
	string str = "<circle cx='";
	str.append(drawX(p)); str.append("' cy='");
	str.append(drawY(p)); str.append("' r='0.8' fill='");
	str.append(color);
	str.append("' />\n");
	return str;
}

string drawPolygon(vector<lineSegment> & polygon){
	string str = "<polygon stroke='black' stroke-width='0.1' fill='#D3D3D3'  points='";
	str.append(drawX(polygon[0].p));
	str.append(","); str.append(drawY(polygon[0].p));
	str.append(" ");
	str.append(drawX(polygon[0].q));
	str.append(",");
	str.append(drawY(polygon[0].q));
	str.append(" ");
	for(int i = 1; i < polygon.size(); i++){
		str.append(drawX(polygon[i].q));
		str.append(",");
		str.append(drawY(polygon[i].q));
		str.append(" ");
	}
	str.append("'/>\n");
	return str;
}

string drawPolygons(vector <vector <lineSegment> > & polygons){
	string str = "";
	for(int i = 0; i < polygons.size(); i++){
		str.append(drawPolygon(polygons[i]));
	}
	return str;
}
string drawTitle(string testTitle, double distance){
	string str = "";
	str = str.append("<text x='0' y='");
	str = str + to_string(-10*max_y-5); 
	str = str + "' font-family='Verdana' font-size='5'>";
	str = str + testTitle;
	str = str + ", length: ";
	str = str + to_string(distance);
	str = str + "</text>\n";
	return str;
}

string drawGraph(vector< vector< int> > &graph, vector<point>& points){
	string str = "";
	size_t plane_start = ((points.size()*config.printLevel)/points.size())*points.size();
	size_t plane_end   = plane_start+points.size();
	for(size_t i = plane_start; i < plane_end; i++){
		for(size_t j = 0; j < graph[i].size(); j++){
			point from = points[i%points.size()], to=points[graph[i][j]%points.size()];
			str = str + "<line x1='";
			str = str + drawX(from);
			str = str + "' y1='";
			str = str + drawY(from);
			str = str + "' x2='";
			str = str + drawX(to);
			str = str + "' y2='";
			str = str + drawY(to);
			str = str + "' stroke-width='0.1' stroke='blue'/>\n";
		}
	}
	return str;
}


int cutThrough(lineSegment l1, lineSegment l2){
	//point A = l1.p; point B = l1.q;
	//point C = l2.p; point D = l2.q;
	point AC; 
	AC.x = l2.p.x - l1.p.x; 
	AC.y = l2.p.y - l1.p.y;
	point AB;
	AB.x = l1.q.x - l1.p.x; 
	AB.y = l1.q.y - l1.p.y;
	point AD;
	AD.x = l2.q.x - l1.p.x; 
	AD.y = l2.q.y - l1.p.y;
	double AC_AB = AC.x*AB.y - AC.y*AB.x;
	double AD_AB = AD.x*AB.y - AD.y*AB.x;
	if(AC_AB*AD_AB < 0){
		//point A = l2.p; point B = l2.q;
		//point C = l1.p; point D = l1.q;
		AC.x = l1.p.x - l2.p.x; 
		AC.y = l1.p.y - l2.p.y;
		
		AB.x = l2.q.x - l2.p.x; 
		AB.y = l2.q.y - l2.p.y;
		
		AD.x = l1.q.x - l2.p.x; 
		AD.y = l1.q.y - l2.p.y;
		AC_AB = AC.x*AB.y - AC.y*AB.x;
		AD_AB = AD.x*AB.y - AD.y*AB.x;
		if(AC_AB*AD_AB < 0){
			return 1; //cut through
		}
	}
	return 0;

}

//If the return value is an odd, the point definitely is inside polygon
int pnpoly(vector<lineSegment> polygon, double testx, double testy, bool OyDirection)
{
	int i, j, c = 0;
	int nEdges = polygon.size();
	double min = INT64_MAX;
	for(int i = 0; i < nEdges; i++){
		if(OyDirection){
			if(polygon[i].p.y < min){
					min = polygon[i].p.y;
			}
		}
		else{
			if(polygon[i].p.x < min){
					min = polygon[i].p.x;
			}
		}
	}
	min--;

	point pA(testx, testy);
	point pB(min, min);
	if(OyDirection){
		pB.x = testx;
	}
	else{
		pB.y = testy;
	}

	lineSegment line;
	line.p = pA;
	line.q = pB;

	int result = 0;

	for(size_t j = 0; j < polygon.size();j++){
		result += cutThrough(line,polygon[j]);
	}

	return result;
}


vector<point> middlePoints(point pA, point pB){
	vector<point> allMiddlePoints;
	int size = (int)round(abs(pA.x - pB.x));
	if(size <= 1){
		point p((pA.x + pB.x)/2, (pA.y + pB.y)/2);
		allMiddlePoints.push_back(p);
	}
	else{
		size--;
		double x = min(pA.x + 1, pB.x + 1); 
		double y = ((x - pA.x)*(pB.y - pA.y)/(pB.x - pA.x)) + pA.y;
		for(int i = 0; i < size; i++){
			point p(x + i, y + i);
			allMiddlePoints.push_back(p);
		}
	}
	return allMiddlePoints;
}


bool insidePolygon(lineSegment line, vector<vector<lineSegment> > &polygons){
	double x;
	double y;
	bool OyDirection = true;
	if(line.p.x == line.q.x){
		OyDirection = false;
	}

	vector<point> allMiddlePoints = middlePoints(line.p, line.q);

	for(int i = 0; i < polygons.size(); i++){

		for(int j = 0; j < allMiddlePoints.size(); j++){
			x = allMiddlePoints.at(j).x; 
			y = allMiddlePoints.at(j).y;
			if(x != FLT_MAX && y != FLT_MAX){
				int c = pnpoly(polygons[i], x, y, OyDirection);
				if(c % 2 == 1)
					return true;
			}
		}
	}
	return false;
}


//Take a line segment and returns 1 if the line cuts through any polygon
int numberOfCuttingThrough(vector<vector<lineSegment> > &polygons, lineSegment l){
	int result = 0;
	for(size_t i = 0; i < polygons.size();i++){
		int numberOfvaolation=0;
		for(size_t j=0;j<polygons[i].size();j++){
			result += cutThrough(l,polygons[i][j]);
		}
	}
	return result;
}

string drawCurverMovement(vector<Path*> trajectory){
	string str = "\n";
	//vector<Section> buildingLines;
	bool getFirstPoint = false;
	

	vector<char> typeOfTrajectory;
	lineSegment prev;
	///https://xuanthulab.net/anh-svg-trong-html5.html
	int numPaths = trajectory.size();
	char prevTypeOfTrajectory = ' ';
    for(int i = 0; i < numPaths; i++){
        Path* path = trajectory.at(i);
        //cout<<"Path "<<i<<endl;
        vector<PathSegment*> segments = path->segments;
        //int numPossibleSegments = segments.size();
        //for(int j = 0; j < numPossibleSegments; j++)
		{
            //cout<<"\tSegment "<<j<<endl;
            vector<Section*> sections = segments.at(0)->sections;
            int numSections = sections.size();
            for(int k = 0; k < numSections; k++)
			{
				
				if(sections.at(k)->typeOfTrajectory == 'C'){
					//if(prevTypeOfTrajectory != sections.at(k)->typeOfTrajectory)
					{
						//prevTypeOfTrajectory = sections.at(k)->typeOfTrajectory;
						str.append("<path d=\"M ");
				
						double x0 = RATIO*(sections.at(k)->beganX)*10;
						double y0 = RATIO*(sections.at(k)->beganY*(-10));
						str.append(to_string(x0));
						str.append(" ");
						str.append(to_string(y0));
						str.append(" ");

						str.append("A ");
						str.append(to_string(RATIO*10));
						str.append(" ");
						str.append(to_string(RATIO*10));
						str.append(" 0 ");
						int fA = sections.at(k)->param > 3.14 ? 1 : 0;
						int fB = ((sections.at(k)->param < 0 && sections.at(k)->steering == 'R')
										|| (sections.at(k)->param > 0 && sections.at(k)->steering == 'L')
										) ? 0 : 1;
						str.append(to_string(fA));
						str.append(" ");
						str.append(to_string(fB));
						str.append(" ");
						str.append(to_string(sections.at(k)->endedX*RATIO*(10)));
						str.append(" ");
						str.append(to_string(sections.at(k)->endedY*RATIO*(-10)));
						str.append("\"");
						str.append(" style=\"stroke:red; fill:transparent\" />\n");
						//}
					}
				}
				//str.append(to_string(RATIO*(sections.at(k)->getFirstVelocity().q.x)*(10)));
				//str.append(" ");
				//str.append(to_string(sections.at(k)->getFirstVelocity().q.y*RATIO*(-10)));
				//str.append(" A ");
				
				//str.append(to_string(sections.at(k)->getLastVelocity().q.x));
            }
        }
    }
	return str;
}

void drawShortestPath(string file_name, point & start, point & end, vector <vector < lineSegment > > & polygons,
						//vector<point> &sideSteps, 
						vector<point> &points, 
						//vector<point> &route,
						vector<Path*> trajectory,
						vector< vector<int> >graph){
	string str1 = "<?xml version='1.0' encoding='UTF-8' ?>\n";
	string str2 = "<svg viewBox='"; //+ 
	str2.append(std::to_string(10*min_x-5));
	str2.append(" ");
	str2.append(std::to_string(/*-10*max_y-45*/-455));
	str2.append(" ");
	str2.append(std::to_string((abs(min_x)+abs(max_x))*10+10));
	str2.append(" ");
	str2.append(std::to_string((455/*abs(min_y)+abs(max_y))*10 + 80)*/)));
	str2.append("' xmlns='http://www.w3.org/2000/svg' version='1.1'>\n");

	str1 = str1 + str2;

	str1 = str1 + drawPolygons(polygons);

	str1 = str1 + drawPoint(start,"#FFA500");
	str1 = str1 + drawPoint(end,"green");

	//str1 = str1 + drawTitle(testTitle,distance);
	str1 = str1 + drawGraph(graph,points);
	//if(distance!=-1 && config.drawRoute)
	{ 
		//str1 = str1 + drawShortestRoute(route);
		//str1 = str1 + drawShortestRoute(sideSteps);
		str1 = str1 + drawCurverMovement(trajectory);
	}
	str1 = str1 + "</svg>\n";
	std::ofstream ofs(file_name.c_str());
	if (!ofs.good())
		cout<<"Could not write data to "<<file_name;

	ofs << str1;
	ofs.close();
}
#endif