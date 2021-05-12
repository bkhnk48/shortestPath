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


void setConfig(int argc, const char* argv[]){
	for(int i = 1 ; i < argc ; i++ ){
		string s(argv[i]);	
		if(s.compare("-k")==0){
			config.k = atoi(argv[i+1]);
			//std::cout<<"In ra k: "<<config.k<<std::endl;
		}
		if(s.compare("-p") == 0){
			config.printGraph = true;
			//std::cout<<"Ve graph: "<<std::endl;
			if(argc-1>i){
				string temp(argv[i+1]);

				if(temp.find_first_not_of( "0123456789" ) == string::npos){
					config.printLevel = atoi(argv[i+1]);
				}
			}
		}
	}
}

int getTime(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end){
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

vector<point> getTrajectory(int argc, const char* argv[]){
	max_x=max_y=min_y=min_x=0;

	setConfig(argc,argv);

	//Create variables for holding start and endpoint plus the test title
	point start, end;
	string testTitle;

	//Create vector for containing the linesegments of the polygons
	vector< vector < lineSegment> > polygons;

	//Create vector for containing the points of the polygons
	vector<point> points;

	//Call function that parses the file input
	readInput(start, end, testTitle, polygons, points);
	//std::cout<<"File: "<<testTitle<<std::endl;

	vector< vector < int > > graph;
	vector< vector < double > > graphDistance;

	//Vector so we can backtrack the route
	vector<int> route;

	vector< vector < int > > crossesNumber;


	//Get how many points we have
	size_t numberOfPoints = points.size();

	//Create a two dimenstional vector for the graph
	
	size_t dimension = numberOfPoints*(config.k+1);
	graph.resize(dimension,vector<int>());
	graphDistance.resize(dimension,vector<double>());

   auto time1 = std::chrono::steady_clock::now();

	//Call function that calculate the distance
	calculateNumberOfCrossings(crossesNumber, polygons, points);

   auto time2 = std::chrono::steady_clock::now();

	makeVisabilityGraph(graph, graphDistance, crossesNumber, points);

   auto time3 = std::chrono::steady_clock::now();


	//The graph is constructed call dijksta to calculate the distance
	double distance = dijkstra(graphDistance,graph,route);

   auto time4 = std::chrono::steady_clock::now();
	//Output the distance
	
	if(config.printGraph){
		draw(testTitle,start,end, polygons,distance,points,route,graph);
		return vector<point>();	
	}
	else{
		std::cout<<points.size()<<std::endl;
		for(int i = 0; i < points.size(); i++)
		{
			std::cout<<"("<<points.at(i).x<<", "<<points.at(i).y<<") ";
		}
		cout<<"\n";
		//cout << distance << endl;
		cout << getTime(time1,time2) << " " << getTime(time2,time3) << " " << getTime(time3,time4) << " " << distance << endl;
		//cout << "Dijkstra took : " << getTime(visibility_end,dijkstra_end) << endl;
		return points;
	}	
}

void getData(int *numOfStacks, int *rowsInStack, int *columnsInStack, 
				int *widthOfLine, int *widthOfAV, int *lengthOfAV){
	//input strings
	string strStacks, strRowsStack, strColsStack;
	string strWidthLine, strWidthAV, strLengthAV;
	
	do{
		cout<<"Type values for some variables (P/p for using all default values)"<<endl;
		cout<<"Or S/s for skipping the current variable"<<endl;
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
	}while(*numOfStacks <= 0 || 
			*rowsInStack <= 0 || 
			*columnsInStack <= 0 || 
			*widthOfLine <= 2*(*widthOfAV) ||
			*widthOfAV <= 0 ||
			*lengthOfAV <= 2*(*widthOfAV));
	
}

void print(int numOfStacks, int rowsInStack, int columnsInStack, 
				int widthOfLine, int widthOfAV, int lengthOfAV){
	cout<<"In conclusion: #Stacks "<<numOfStacks<<", #rows in Stack "<<rowsInStack;
	cout<<", #cols in Stack "<<columnsInStack<<endl;
	cout<<"               Width of line "<<widthOfLine<<", width of AV "<<widthOfAV;
	cout<<", length of AV "<<lengthOfAV<<endl;
}

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

	print(numOfStacks, rowsInStack, columnsInStack, 
				widthOfLine, widthOfAV, lengthOfAV);

	//vector<point> trajectory = getTrajectory(argc, argv);
	return 0;
}