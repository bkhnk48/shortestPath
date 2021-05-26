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
        Homotopy(){

        }
        vector<point> sideStepRouting(vector<point> route, vector< vector<lineSegment > > polygons, vector<point> points){
            vector<point> sideSteps;
            point start = route.at(0);
            sideSteps.push_back(start);
            point prev = route.at(0);
            lineSegment normalIn, normalOut;
            lineSegment temp1, temp2;
            normalIn.p.x = 0; normalIn.p.y = 0; 
            normalOut.p.x = 0; normalOut.p.y = 0; 
            for(int i = 1; i < route.size(); i++){
                point p = route.at(i);
                #pragma region Calculate the normal vector
                if(p.x == prev.x){
                    normalIn.q.x = 1; normalIn.q.y = 0;
                    
                    normalOut.q.x = -1; normalOut.q.y = 0;
                }
                else if(p.y == prev.y){
                    normalIn.q.x = 0; normalIn.q.y = 1;

                    normalOut.q.x = 0; normalOut.q.y = -1;
                }
                else{
                    double deltaY = p.y - prev.y;
                    double deltaX = prev.x - p.x;

                    double length = sqrt(deltaX*deltaX + deltaY*deltaY);
                    
                    normalIn.q.x = deltaY/length; normalIn.q.y = deltaX/length;
                    normalOut.q.x = -deltaY/length; normalOut.q.y = -deltaX/length;
                }
                #pragma endregion
                
                temp1.p.x = prev.x ;
                temp1.p.y = prev.y ;

                temp2.p.x = prev.x ;
                temp2.p.y = prev.y ;

                if(i != 1){
                    temp1.p.x += (normalIn.q.x - normalIn.p.x);
                    temp1.p.y += (normalIn.q.y - normalIn.p.y);

                    temp2.p.x += (normalOut.q.x - normalOut.p.x);
                    temp2.p.y += (normalOut.q.y - normalOut.p.y);
                }
                
                temp1.q.x = p.x;
                temp1.q.y = p.y;

                temp2.q.x = p.x;
                temp2.q.y = p.y;

                if(i != route.size() - 1){
                    temp1.q.x += (normalIn.q.x - normalIn.p.x);
                    temp1.q.y += (normalIn.q.y - normalIn.p.y);

                    temp2.q.x += (normalOut.q.x - normalOut.p.x);
                    temp2.q.y += (normalOut.q.y - normalOut.p.y);
                }

                //if(!insidePolygon(temp1, polygons)){
                if(numberOfCuttingThrough(polygons, temp1) == 0){
                    addNewPoints(temp1, i, route.size(), sideSteps);
                }
                //else if(!insidePolygon(temp2, polygons)){
                else if(numberOfCuttingThrough(polygons, temp2) == 0){
                    addNewPoints(temp2, i, route.size(), sideSteps);
                }

                prev = p;
            }
            //sideSteps.push_back(route.at(route.size() - 1));

            return sideSteps;
        }
    
    private:
        void addNewPoints(lineSegment line, int indexOfPoint, int size, vector<point> &route){
            if(indexOfPoint == 1){
                point q;
                q.x = line.q.x;
                q.y = line.q.y;

                route.push_back(q);
            }
            else if(indexOfPoint > 1 && indexOfPoint <= size - 1){
                point p;
                p.x = line.p.x;
                p.y = line.p.y;

                point q;
                q.x = line.q.x;
                q.y = line.q.y;

                route.push_back(p);
                route.push_back(q);
            }
        }
};
#endif