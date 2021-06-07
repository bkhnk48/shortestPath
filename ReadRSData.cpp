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

vector<point> readRSFile(string fileName){

    vector<point> discreteTrajectory;
     
    ifstream infile(fileName);
    string line;
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
            
            if (!(iss >> x >> y >> distance >> typeOfTraj)) 
            { 
                break; 
            } // error
            cout<<"X = "<<x<<" Y = "<<y<<" dis = "<<distance<<" "<<typeOfTraj<<endl;
            // process pair (a,b)
        }
    }
    cout<<"Close file"<<endl;
    infile.close();

    return discreteTrajectory;
}
#endif