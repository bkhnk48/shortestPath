#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>

#ifndef __NAIVE__
#define __NAIVE__
//So we don't need to write std:: everywhere
using namespace std;

string toString(point p){
	ostringstream os;
	os << "(" << p.x << "," << p.y << ")";
	string s = os.str();
	return s;
}

//Function that given a linesegment returns a string representation of it
string toString(lineSegment l){
	ostringstream os;
	os << toString(l.p) << " - " << toString(l.q);
	string s = os.str();
	return s;
}

//Function for reading the next point in stdin
point readPoint(){
	double x,y;
	int t = scanf("%lf,%lf\n",&x,&y);
	point p(x,y);
	return p;
}

void readInput(point &start, point &end, string &testTitle, vector< vector< lineSegment> > &polygons, vector<point> &points){

	//Get the test title
	getline(cin,testTitle);

	//Read start and end points
	start = readPoint();
	end  = readPoint();
	points.push_back(start);

	//Get the number of Polygons
	int numberOfPolygons;
	cin >> numberOfPolygons;

	polygons.resize(numberOfPolygons, vector<lineSegment>());

	//Iterate through the polygons
	for(int i=0;i<numberOfPolygons;i++){

		//Get the number of sides
		int numberOfSides;
		cin >> numberOfSides;

		//Get the first point and remember it 
		//so we can make the last linesegment after the loop
		point firstPoint = readPoint();

		//Create a variable for the last point we saw
		point lastPoint  = firstPoint;

		//Add the first point
		points.push_back(firstPoint);

		for(int j=1;j<numberOfSides;j++){

			//Get the next point
			point currentPoint = readPoint();

			//Add point to list of points
			points.push_back(currentPoint);

			//create linesegment
			lineSegment l;

			//Set the linesegment
			l.p = lastPoint;
			l.q = currentPoint;

			//push it to the list of linesegments
			polygons[i].push_back(l);

			//and update the lastPoint
			lastPoint = currentPoint;
		}

		//Construct the missing linesegment
		lineSegment l;	
		l.p = lastPoint;
		l.q = firstPoint;

		//and push it to the vector
		polygons[i].push_back(l);
	}

	points.push_back(end);

}

//Function for calculating the distance between two points
double dist(point p, point q){
	//calculate euclidean distance sqrt( (p.x-q.x)^2+(p.y-q.y)^2 )
	return (double) sqrt(pow(p.x-q.x,2.0)+pow(p.y-q.y,2.0));
}

double rightTurn(point p1, point p2, point p3){
	return (p1.y-p2.y)*(p2.x-p3.x)-(p2.y-p3.y)*(p1.x-p2.x);
}

int crosses(lineSegment l1, lineSegment l2){
	if(l1==l2) return -1;

	int returnValue = 0;
	if(l1.p == l2.p) returnValue++; 
	if(l1.p == l2.q) returnValue++;
	if(l1.q == l2.p) returnValue++;
	if(l1.q == l2.q) returnValue++;

	if(returnValue>0) return returnValue;

	double rt_1 = rightTurn(l1.p,l1.q,l2.p);
	double rt_2 = rightTurn(l1.p,l1.q,l2.q);
	double rt_3 = rightTurn(l2.p,l2.q,l1.p);
	double rt_4 = rightTurn(l2.p,l2.q,l1.q);

	double r1 = rt_1*rt_2;
	double r2 = rt_3*rt_4;

	if((r1==0 && r2<=0) || (r2==0 && r1<=0)){
		returnValue=10;
	}

	if((r1<=0) &&  (r2<=0)) returnValue = 10;
	return returnValue;
}

//Takes a line segment and returns the number of polygon edges it crosses
int numberOfCrossings(vector<vector<lineSegment> > &polygons, lineSegment l){
	int n=0;
	for(size_t i = 0; i < polygons.size();i++){
		int numberOfvaolation=0;
		for(size_t j=0;j<polygons[i].size();j++){
			int result = crosses(l,polygons[i][j]);
			if(result==-1){
				return 0;
			}
			else if(result==10){
				numberOfvaolation=10;
			}
			else{
				numberOfvaolation+=result;
			}
		}
		if(numberOfvaolation>2){
			n++;
		}
	}
	return n;
}


//Implementation of dijkstra
//Takes a graph and a start and end point in the graph
//returns the distance
double dijkstra(vector< vector< double > > &graphDistance, vector<vector< int> > &graph ,vector<int> &route){
	int start = 0;
	size_t end = graph.size()-1;

	route.resize(graph.size());

	//Create a vector to see if we already visited the point
	vector<bool> visited(graph.size());

	//Create a priority queue where pq_pair consists of 
	//the distance to the point and the point index in points vector
	priority_queue< tuple<double,int,int> > pq;

	//Put the start point in the queue
	//TODO: tuple instead of pair?
	pq.push(make_tuple(0,start,-1));

	//While there a still points we haven't visited we run
	while(!pq.empty()){

		//Get the top point
		tuple<double,int,int> t = pq.top();	

		//Remove it
		pq.pop();

		//How far have we travelled until now
		double distanceSoFar = -1*get<0>(t);

		//What point are we at
		int current = get<1>(t);

		int whereFrom = get<2>(t);

		//If we already visited the current continue
		if(visited[current]) continue;

		route[current] = whereFrom;

		//We we have reached the distination return the distance
		if(current == end) return distanceSoFar;

		//Set the current to true in the visited vector
		visited[current] = true;

		//Go through every current we have an edge to and haven't visited
		for(size_t i = 0; i < graph[current].size() ; i++){
			int next = graph[current][i];
			if(visited[next]) continue;

			//calculate the complete distance to that current
			double newdistance = distanceSoFar + graphDistance[current][i];

			//And push it to the queue
			tuple<double,int,int> newTuple = make_tuple(-1*newdistance,next,current);

			pq.push(newTuple);
		}
	}
	return -1;
}

int makeVisabilityGraph(vector< vector < int > > &graph, vector< vector < double > > &graphDistance,vector< vector<int> > &crossesNumber, vector<point> &points){

	//Get how many points we have
	size_t numberOfPoints = points.size();

	//Go through all pairs of points and calculate the distance
	for(size_t i=0;i<graph.size();i++){
		for(size_t j=0;j<numberOfPoints;j++){

			size_t from = i;

			size_t from_point_index = from%numberOfPoints;

			size_t to_point_index = j;

			if(from_point_index == 2 && j == 9){
				cout<<"DEBUG "<<crossesNumber[from_point_index][j]<<endl;
			}

			size_t to = (i/numberOfPoints)*numberOfPoints+crossesNumber[from_point_index][j]*numberOfPoints+j;


			//If it is the same point don't make an edge
			if(graph.size()>to){
			//if(crossesNumber[from][to]==0){
				//Call dist function to calculate the distance
				double distance = dist(points[from_point_index],points[to_point_index]);
				//cout<<"Dist from "<<from_point_index<<" ("<<points[from_point_index].x<<", "<<points[from_point_index].y<<") to ("<<points[to_point_index].x<<", "<<points[to_point_index].y<<") = "<<distance<<endl;

				graphDistance[from].push_back(distance);
				graph[from].push_back(to);
			}
		}
	}

	return 0;
}

bool samePolygonButNotNeighbor(lineSegment l, vector<vector<lineSegment> > polygons){
	bool found_P = false, found_Q = false, found_both = false;
	for(int i = 0; i < polygons.size(); i++){
		found_P = false; found_Q = false; found_both = false;
		for(int j = 0; j < polygons.at(i).size(); j++)
		{
			if(!found_P && !found_both)
			{
				if(l.p == polygons.at(i).at(j).p || l.p == polygons.at(i).at(j).q)
				{
					found_P = true;
					if((l.q == polygons.at(i).at(j).q) || (l.q == polygons.at(i).at(j).p))
					{
						found_both = true;
						return false;
					}
				}
			}
			
			if(!found_Q && !found_both){
				if(l.q == polygons.at(i).at(j).p || l.q == polygons.at(i).at(j).q)
				{
					found_Q = true;
					if((l.p == polygons.at(i).at(j).q) || (l.p == polygons.at(i).at(j).p))
					{
						found_both = true;
						return false;
					}
				}
			}
			if(found_P && found_Q && !found_both)
				return true;
		}
	}

	return false;
	
}

int numberOfVirtualCrossing(vector < vector < int > > &crossesNumber,vector<vector<lineSegment> > &polygons, vector<point> &points){
	crossesNumber.resize(points.size(),vector<int>(points.size()));
	int c, d;
	for(size_t i=0;i<points.size();i++){
		for(size_t j=0;j<points.size();j++){
				if(i == j){
					crossesNumber[i][j] = (i == 0) ? 0 : 2;
				}
				else{
					lineSegment l;
					l.p = points[i];
					l.q = points[j];
					c = numberOfCuttingThrough(polygons, l);

					if(c == 0){
						if(samePolygonButNotNeighbor(l, polygons)){
							c = 1;
						}
					}	
					crossesNumber[i][j] = c;
				}

				//Call numberOfCrossings, which 
				//suprise suprise counts the number of crossings
				//d = numberOfCrossings(polygons,l);
				
				//c = numberOfCuttingThrough(polygons, l);
				//if(d != c){
				//cout<<"("<<points[i].x<<", "<<points[i].y<<") => ("<<points[j].x<<", "<<points[j].y<<") c = "<<crossesNumber[i][j]<<endl;
				//}
		}
	}
	return 0;	

}

int calculateNumberOfCrossings(vector < vector < int > > &crossesNumber,vector<vector<lineSegment> > &polygons, vector<point> &points){
	crossesNumber.resize(points.size(),vector<int>(points.size()));
	int c, d;
	for(size_t i=0;i<points.size();i++){
		for(size_t j=0;j<points.size();j++){
				lineSegment l;
				l.p = points[i];
				l.q = points[j];

				if(i == 2 && j == 9)
				{
					cout<<"DEBUG 282"<<endl;	
				}

				//Call numberOfCrossings, which 
				//suprise suprise counts the number of crossings
				d = numberOfCrossings(polygons,l);
				crossesNumber[i][j] = d;
				c = numberOfCuttingThrough(polygons, l);
				//if(d != c)
				{
					cout<<"("<<l.p.x<<", "<<l.p.y<<") => ("<<l.q.x<<", "<<l.q.y<<") d = "<<d<<", c = "<<c<<endl;
				}
		}
	}
	return 0;	
}

void print(int numOfStacks, int rowsInStack, int columnsInStack, 
				int widthOfLine, int widthOfAV, int lengthOfAV){
	cout<<"In conclusion: #Stacks "<<numOfStacks<<", #rows in Stack "<<rowsInStack;
	cout<<", #cols in Stack "<<columnsInStack<<endl;
	cout<<"               Width of line "<<widthOfLine<<", width of AV "<<widthOfAV;
	cout<<", length of AV "<<lengthOfAV<<endl;
}

void getData(int *numOfStacks, int *rowsInStack, int *columnsInStack, 
				int *widthOfLine, int *widthOfAV, int *lengthOfAV){
	//input strings
	string strStacks, strRowsStack, strColsStack;
	string strWidthLine, strWidthAV, strLengthAV;
	
	do{
		cout<<"Type values for some variables (P/p for using all default values)"<<endl;
		cout<<"Or S/s for skipping the current variable"<<endl;
		cout<<"\t\t keep in mind: all of them should be positive and WL > 2WAV, LAV > 2*WAV"<<endl;
		cout<<"Num of Stacks in parking lot: "; cin >> strStacks;
		
		if(strStacks.compare("P") == 0 || strStacks.compare("p") == 0)
			break;
		if(strStacks.compare("S") != 0 && strStacks.compare("s") != 0)
			//numOfStacks = stoi(strStacks);
			istringstream(strStacks) >> *numOfStacks;

		cout<<"Num of rows in each Stack: "; cin >> strRowsStack;
		if(strRowsStack.compare("P") == 0 || strRowsStack.compare("p") == 0)
			break;
		if(strRowsStack.compare("S") != 0 && strRowsStack.compare("s") != 0)
			istringstream(strRowsStack) >> *rowsInStack;
		
		cout<<"Num of columns in each Stack: "; cin >> strColsStack;
		if(strColsStack.compare("P") == 0 || strColsStack.compare("p") == 0)
			break;
		if(strColsStack.compare("S") != 0 && strColsStack.compare("s") != 0)
			istringstream(strColsStack) >> *columnsInStack;

		cout<<"Width of line in parking lot: "; cin >> strWidthLine;
		if(strWidthLine.compare("P") == 0 || strWidthLine.compare("p") == 0)
			break;
		if(strWidthLine.compare("S") != 0 && strWidthLine.compare("s") != 0)
			istringstream(strWidthLine) >> *widthOfLine;

		cout<<"Width of AV: "; cin >> strWidthAV;
		if(strWidthAV.compare("P") == 0 || strWidthAV.compare("p") == 0)
			break;
		if(strWidthAV.compare("S") != 0 && strWidthAV.compare("s") != 0)
			istringstream(strWidthAV) >> *widthOfAV;

		cout<<"Length of AV: "; cin >> strLengthAV;
		if(strLengthAV.compare("P") == 0 || strLengthAV.compare("p") == 0)
			break;
		if(strLengthAV.compare("S") != 0 && strLengthAV.compare("s") != 0)
			istringstream(strLengthAV) >> *lengthOfAV;
		cout<<"====================================="<<endl;
		print(*numOfStacks, *rowsInStack, *columnsInStack, 
				*widthOfLine, *widthOfAV, *lengthOfAV);
	}while(*numOfStacks <= 0 || 
			*rowsInStack <= 0 || 
			*columnsInStack <= 0 || 
			*widthOfLine <= 2*(*widthOfAV) ||
			*widthOfAV <= 0 ||
			*lengthOfAV <= 2*(*widthOfAV));
	
}

#endif