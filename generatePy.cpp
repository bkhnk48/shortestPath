#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
#include <queue>
#include <bits/stdc++.h>
#include <algorithm>
//#include <string.h> 


#include "draw.cpp"

#ifndef _BUILD_PYTHON_CODE_
#define _BUILD_PYTHON_CODE_

using namespace std;


string drawIntX(point &p){
	int r = (int)p.x*10;
	return to_string(r);
}

string drawIntY(point &p){
	int r = (int)p.y*10;
	return to_string(r);
}

void writeRoute(string file_name, vector<point> &route, float firstAngle, float lastAngle){
    string str = to_string(firstAngle);
    str.append("\n");
    str.append(to_string(lastAngle));
    str.append("\n");
    for(int i = 0; i < route.size(); i++){
        str.append(to_string(route.at(i).x));
        str.append("\n");
        str.append(to_string(route.at(i).y));
        str.append("\n");
    }

    std::ofstream ofs(file_name.c_str());
	if (!ofs.good())
		cout<<"Could not write data to "<<file_name;

	ofs << str;
	ofs.close();
}

void writeFile(string file_name, vector< vector< lineSegment> > &polygons){
    if(polygons.size() == 0)
        return;
    
    string str1 = to_string(polygons.size());
    str1.append("\n");
    //str1.append("\t");

    for(int i = 0; i < polygons.size(); i++){
        int numEdges = polygons.at(i).size();
        str1.append(to_string(numEdges));
        str1.append("\n");
        for(int j = 0; j < polygons.at(i).size(); j++){
            str1.append(to_string(polygons.at(i).at(j).p.x));
            str1.append(" ");
            str1.append(to_string(polygons.at(i).at(j).p.y));
            str1.append(" ");
            str1.append(to_string(polygons.at(i).at(j).q.x));
            str1.append(" ");
            str1.append(to_string(polygons.at(i).at(j).q.y));
            str1.append("\n");
        }
    }

    std::ofstream ofs(file_name.c_str());
	if (!ofs.good())
		cout<<"Could not write data to "<<file_name;

	ofs << str1;
	ofs.close();

    
}

void runPythonCode(string file_name, vector<point> &route, int* directionAtTheEnd, vector< vector< lineSegment> > &polygons){

    //writeFile("test/polygons.txt", polygons);
    
    float scale = 1;
    float deltaX = 0; //22;
    float deltaY = 0; //18;

	char cmd[50];
    float lastAngle = 0;
    switch(*directionAtTheEnd){
        case 8:
            lastAngle = 90;
        case 4:
            lastAngle = 180;
        case 6:
            lastAngle = 0;
        case 2:
            lastAngle = 270;
    }

    writeRoute("route.txt", route, 90, lastAngle);

    strcpy(cmd, "test\\AllPossibleTrajectories.py");
    system(cmd);

}
#endif