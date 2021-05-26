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

//#include "preprocess.cpp"
#include "CollectingPoints.cpp"


#ifndef _HOMOTOPY_
#define _HOMOTOPY_

class Homotopy{
    public:
        vector<point> stayAwayFromPolygons(vector<point> route, vector< vector<lineSegment > > polygons, vector<point> points){
            vector<point> sideSteps;
            point start = route.at(0);
            for(int i = 1; i < route.size(); i++){

            }

            return sideSteps;
        }
};
#endif