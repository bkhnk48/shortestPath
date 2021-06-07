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
#include <fstream>


#include "generatePy.cpp"

#ifndef _READ_REEDS_SHEEP_
#define _READ_REEDS_SHEEP_

using namespace std;

const string LEFT("Left");
//const string RIGHT("Right");

vector<point> readRSFile(string fileName){

    vector<point> discreteTrajectory;
     
    ifstream infile(fileName);
    string line;
    string steering;
    double x, y, distance;
    string typeOfTraj;
    getline(infile, line);
    istringstream firstIss(line);
    if (firstIss >> x >> y >> typeOfTraj) 
    { 
        cout<<"X = "<<x<<" Y = "<<y<<" "<<typeOfTraj<<endl;
        point p0(x, y);
        discreteTrajectory.push_back(p0);

        while (getline(infile, line))
        {
            istringstream iss(line);
            
            if (!(iss >> x >> y >> distance >> typeOfTraj >> steering)) 
            { 
                break; 
            } // error
            cout<<"X = "<<x<<" Y = "<<y<<" dis = "<<distance<<" "<<typeOfTraj<<" "<<steering<<endl;
            // process pair (a,b)
        }
    }
    cout<<"Close file"<<endl;
    infile.close();

    return discreteTrajectory;
}

vector<point> getSegmentOfCircle(point &p1, point &p2, double rotatedAngle, string steering)
{
    vector<point> segment;
    double centerX = (p1.x + p2.x)/2;
    double centerY = (p1.y + p2.y)/2;
    double xIn = 0, yIn = 0, xOut =0, yOut = 0;
    getNormalInAndOut(p2.x - p1.x, p2.y - p1.y, &xIn, &yIn, &xOut, &yOut);
    double xNormal, yNormal;
    if(LEFT.compare(steering))
    {
        xNormal = xOut;
        yNormal = yOut;
    }
    else{
        xNormal = xIn;
        yNormal = yIn;
    }
    return segment;
}
#endif