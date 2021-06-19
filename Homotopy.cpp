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

        vector<SIDE> sides;
        vector<Clockwise> clockwises;

        int WIDTH;
        //bool isClockWise = false;
        //int LENGTH;

        Homotopy(int WIDTH){
            this->WIDTH = WIDTH;
            //this->isClockWise = isClockWise;
            //this->LENGTH = LENGTH;
        }

        vector<point> sideStepRouting(vector<point> route, vector< vector<lineSegment > > polygons, vector<point> points){
            vector<point> sideSteps;
            point start;
            start.x = route.at(0).x - WIDTH;
            start.y = route.at(0).y;
            
            sideSteps.push_back(start);
            point prev = route.at(0);
            lineSegment normalIn, normalOut;
            lineSegment temp1, temp2;
            normalIn.p.x = 0; normalIn.p.y = 0; 
            normalOut.p.x = 0; normalOut.p.y = 0; 
            for(int i = 1; i < route.size(); i++){
                point p = route.at(i);
                #pragma region Calculate the normal vector
                double deltaY = p.y - prev.y;
                //double deltaX = prev.x - p.x;
                double deltaX = p.x - prev.x;
                getNormalInAndOut(deltaX, deltaY, &normalIn.q.x, &normalIn.q.y
                                        , &normalOut.q.x, &normalOut.q.y);
                #pragma endregion
                
                temp1.p.x = prev.x ;
                temp1.p.y = prev.y ;

                temp2.p.x = prev.x ;
                temp2.p.y = prev.y ;

                //if(i != 1)
                {
                    temp1.p.x += (normalIn.q.x - normalIn.p.x);
                    temp1.p.y += (normalIn.q.y - normalIn.p.y);

                    temp2.p.x += (normalOut.q.x - normalOut.p.x);
                    temp2.p.y += (normalOut.q.y - normalOut.p.y);
                }
                
                //temp1.q.x = p.x;
                //temp1.q.y = p.y;

                //temp2.q.x = p.x;
                //temp2.q.y = p.y;

                //if(i != route.size() - 1)
                {
                    temp1.q.x = p.x + (normalIn.q.x - normalIn.p.x);
                    temp1.q.y = p.y + (normalIn.q.y - normalIn.p.y);

                    temp2.q.x = p.x + (normalOut.q.x - normalOut.p.x);
                    temp2.q.y = p.y + (normalOut.q.y - normalOut.p.y);
                }

                //if(!insidePolygon(temp1, polygons)){
                if(numberOfCuttingThrough(polygons, temp1) == 0){
                    addNewPoints(temp1, i, route.size(), sideSteps, normalIn);
                }
                //else if(!insidePolygon(temp2, polygons)){
                else if(numberOfCuttingThrough(polygons, temp2) == 0){
                    addNewPoints(temp2, i, route.size(), sideSteps, normalOut);
                }

                prev = p;
            }
            //sideSteps.push_back(route.at(route.size() - 1));

            return sideSteps;
        }


        void checkDirectionOfMovement(vector<point> &route, vector< vector<lineSegment > > &polygons){
            clockwises.push_back(Unknown);

            double prevX, prevY, currX, currY, nextX, nextY;
            prevX = route.at(0).x;
            prevY = route.at(0).y;
            currX = route.at(1).x;
            currY = route.at(1).y;
            double uX, uY, vX, vY;
            Clockwise prevClock = Unknown;
            lineSegment normalIn, normalOut;
            lineSegment temp1, temp2;
            lineSegment rightMovement;
            normalIn.p.x = 0; normalIn.p.y = 0; 
            normalOut.p.x = 0; normalOut.p.y = 0; 
            double deltaNormalInX, deltaNormalInY, deltaNormalOutX, deltaNormalOutY;
            string s = "";

            for(int i = 1; i < route.size(); i++){
                #pragma region Calculate the normal vector
                double deltaY = route.at(i).y - prevY;
                //double deltaX = prev.x - p.x;
                double deltaX = route.at(i).x - prevX;
                getNormalInAndOut(deltaX, deltaY, &normalIn.q.x, &normalIn.q.y
                                        , &normalOut.q.x, &normalOut.q.y);
                temp1.p.x = prevX ;
                temp1.p.y = prevY ;

                temp2.p.x = prevX ;
                temp2.p.y = prevY ;

                deltaNormalInX = WIDTH*(normalIn.q.x - normalIn.p.x);
                deltaNormalInY = WIDTH*(normalIn.q.y - normalIn.p.y);

                deltaNormalOutX = WIDTH*(normalOut.q.x - normalOut.p.x);
                deltaNormalOutY = WIDTH*(normalOut.q.y - normalOut.p.y);

                temp1.p.x += deltaNormalInX;
                temp1.p.y += deltaNormalInY;

                temp2.p.x += deltaNormalOutX;
                temp2.p.y += deltaNormalOutY;
                
                temp1.q.x = route.at(i).x + deltaNormalInX;
                temp1.q.y = route.at(i).y + deltaNormalInY;

                temp2.q.x = route.at(i).x + deltaNormalOutX;
                temp2.q.y = route.at(i).y + deltaNormalOutY;

                #pragma endregion

                if(numberOfCuttingThrough(polygons, temp1) == 0){
                    //temp1 means normalIn (+)
                    sides.push_back(LeftSide);
                    //rightMovement.p.x = temp1.p.x + deltaNormalInX;   rightMovement.p.y = temp1.p.y + deltaNormalInY;
                    //rightMovement.q.x = temp1.q.x + deltaNormalInX;   rightMovement.q.y = temp1.q.y + deltaNormalInY;

                    


                    s = "RightSide";
                }
                else if(numberOfCuttingThrough(polygons, temp2) == 0){
                    //temp2 means normalOut (-)
                    sides.push_back(RightSide);
                    s = "LeftSide";
                }
                else{
                    sides.push_back(NotAnswer);
                    s = "NA";
                }
                cout<<sides.at(sides.size() - 1)<<" "<<s<<endl;
                
                prevX = route.at(i).x;
                prevY = route.at(i).y;
            }
        
            for(int i = 1; i < sides.size(); i++){
            
                if(sides.at(i) == LeftSide //&& clockwises.at(i-1) != NA
                    ){
                    cout<<"WRONG DIRECTION from ("<<route.at(i).x<<", "<<route.at(i).y<<") to ("<<route.at(i+1).x<<", "<<route.at(i+1).y<<") "<<endl;
                }
                else if(sides.at(i) == NotAnswer){
                    cout<<"UNKNOWN from ("<<route.at(i).x<<", "<<route.at(i).y<<") to ("<<route.at(i+1).x<<", "<<route.at(i+1).y<<") "<<endl;
                }
                else{
                    cout<<"CORRECT DIRECTION from ("<<route.at(i).x<<", "<<route.at(i).y<<") to ("<<route.at(i+1).x<<", "<<route.at(i+1).y<<") "<<endl;
                }
            }
        }

    

    private:

        int getStepsAlongNormalVector(lineSegment temp1, lineSegment normalIn, vector< vector<lineSegment > > &polygons){
            int result;
            double lastY, lastX;
            double deltaY;
            point lastPoint;
            double deltaNormalInX = (normalIn.q.x - normalIn.p.x);
            double deltaNormalInY = (normalIn.q.y - normalIn.p.y);
            if(deltaNormalInY != 0){
                lastY = deltaNormalInY < 0 ? min_y - 1 : max_y + 1;
                deltaY = lastY - temp1.p.y;
                lastX = temp1.p.x + deltaNormalInX*deltaY/deltaNormalInY;
            }
            else{
                lastX = deltaNormalInX < 0 ? min_x - 1 : max_x + 1;
                lastY = temp1.p.y;
            }

            lastPoint.x = lastX; lastPoint.y = lastY; 
            
            lineSegment scaleNormal(temp1.p, lastPoint);

            vector<int> collisions;
            for(int i = 0; i < polygons.size(); i++)
            {
                for(int j = 0; j < polygons.at(i).size(); j++){
                    if(cutThrough(polygons.at(i).at(j), scaleNormal)){
                        int t = 0 | (i << 16) | j;
                        collisions.push_back(t); 
                    }
                }
            }

            return result;
        }

        void calculateClockwise(vector<point> &route){

            double startX = route.at(0).x;
            if(sides.at(0) == LeftSide)
                startX -= WIDTH;
            else
                startX += WIDTH;
            double startY = route.at(0).y;

            double prevX = startX; 
            double prevY = startY;
            double currX = route.at(1).x;
            double currY = route.at(1).y;
            double nextX, nextY;

            for(int i = 2; i < route.size(); i++){
                nextX = route.at(i).x;
                nextY = route.at(i).y;
                double uX = (currX - prevX);
                double uY = currY - prevY;
                double vX = nextX - currX;
                double vY = nextY - currY;

                double uv = uX*vY - uY*vX;
                if(i == 2 && clockwises.size() == 1){
                    clockwises.erase(clockwises.begin());
                }

                if(uv < 0){
                    if(clockwises.size() == 0)
                        clockwises.push_back(YES);//Clockwise
                    clockwises.push_back(YES);//Clockwise
                }
                else if(uv > 0){//the case uv == 0 never happens
                    if(clockwises.size() == 0)
                        clockwises.push_back(NO);//Counter clockwise
                    clockwises.push_back(NO);//Counter clockwise
                }
                prevX = currX;
                prevY = currY;

                currX = nextX;
                currY = nextY;
            }
        }

        void addNewPoints(lineSegment line, int indexOfPoint, int size, vector<point> &route, lineSegment normal
                            ){
            /*if(indexOfPoint == 1){
                point q;
                q.x = line.q.x;
                q.y = line.q.y;

                route.push_back(q);
            }
            else*/ if(indexOfPoint >= 1 && indexOfPoint <= size - 1){
                point p;
                p.x = line.p.x + normal.q.x*(this->WIDTH - 1);
                p.y = line.p.y + normal.q.y*(this->WIDTH - 1);

                point q;
                q.x = line.q.x + normal.q.x*(this->WIDTH - 1);
                q.y = line.q.y + normal.q.y*(this->WIDTH - 1);

                route.push_back(p);
                route.push_back(q);
            }
        }
};
#endif