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
            start.x = route.at(0).x + WIDTH;
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
                //if(numberOfCuttingThrough(polygons, temp1) == 0){
                if(!cutThroughPolygons(temp1, polygons)){
                    addNewPoints(temp1, i, route.size(), sideSteps, normalIn);
                }
                //else if(!insidePolygon(temp2, polygons)){
                //else if(numberOfCuttingThrough(polygons, temp2) == 0){
                else if(!cutThroughPolygons(temp2, polygons)){
                    addNewPoints(temp2, i, route.size(), sideSteps, normalOut);
                }

                prev = p;
            }
            //sideSteps.push_back(route.at(route.size() - 1));

            return sideSteps;
        }


        vector<point> 
        //void 
        checkDirectionOfMovement(vector<point> &route, vector< vector<lineSegment > > &polygons){
            //clockwises.push_back(Unknown);
            vector<point> rightDirectionRoute;

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

                //if(numberOfCuttingThrough(polygons, temp1) == 0){
                if(!cutThroughPolygons(temp1, polygons)){
                    //temp1 means normalIn (+)
                    sides.push_back(LeftSide);
                    
                    if(i >= 2){
                        int step = getStepsAlongNormalVector(temp1, normalIn, polygons);
                        point p(route.at(i).x + (step + 1)*deltaNormalInX, route.at(i).y + (step + 1)*deltaNormalInY);
                        rightDirectionRoute.push_back(p);
                        cout<<"Move "<<step<<" steps"<<endl;
                    }
                    else if(i == 1){
                        point p(route.at(0).x, route.at(0).y);
                        rightDirectionRoute.push_back(p);
                        rightDirectionRoute.push_back(route.at(1));
                    }
                    
                    s = "RightSide";
                }
                //else if(numberOfCuttingThrough(polygons, temp2) == 0){
                else if(!cutThroughPolygons(temp2, polygons)){
                    //temp2 means normalOut (-)
                    sides.push_back(RightSide);
                    s = "LeftSide";
                    rightDirectionRoute.push_back(route.at(i));
                    
                }
                else{
                    sides.push_back(NotAnswer);
                    s = "NA";
                    rightDirectionRoute.push_back(route.at(i));
                }

                //cout<<sides.at(sides.size() - 1)<<" "<<s<<endl;
                
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

            return rightDirectionRoute;
        }

        double getMABCOfLine(lineSegment line, double *A, double *B, double *C){
            double x1 = line.p.x;
            double x2 = line.q.x;
            double y1 = line.p.y;
            double y2 = line.q.y;
            *A = y2 - y1;
            *B = x1 - x2;
            *C = y1*x2 - x1*y2;
            double M = sqrt((*A)*(*A) + (*B)*(*B));
            return M;
        }

        double minDistanceFromPointToPolygon(point &p, vector<lineSegment> &polygon){
            double M = 0, A = 0, B = 0, C = 0;
            double minD = FLT_MAX, D = 0;
            for(int i = 0; i < polygon.size(); i++){
                lineSegment line = polygon.at(i);
                M = getMABCOfLine(line, &A, &B, &C);
                D = std::abs(A*p.x + B*p.y + C)/M;
                minD = minD > D ? D : minD;
            }
            return minD;
        }

        bool shortestDistanceRequirement(/*point &p*/ lineSegment line, vector< vector<lineSegment>> &polygons, double minD){
            //point p = line.p;
            double d1, d2, d;
            for(int i = 0; i < polygons.size(); i++){
                d1 = minDistanceFromPointToPolygon(line.p, polygons.at(i));
                d2 = minDistanceFromPointToPolygon(line.q, polygons.at(i));
                d = d1 + d2;
                if(d < 2*minD){
                    cout<<"The min = "<<d<<". The point: "<<line.p.x <<" "<<line.p.y;
                    cout<<" as well as ("<<line.q.x <<", "<<line.q.y<<") ";
                    cout<<" Polygon ("<<i<<") "<<endl;
                    return false;
                }
            }
            return true;
        }
        

        int getStepsAlongNormalVector(lineSegment temp1, lineSegment normalIn, vector< vector<lineSegment > > &polygons){
            int result = 0, result1 = 0, result2 = 0;
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

            //vector<int> collisions;
            double //x1, x2, y1, y2, 
                        A, B, C;
            double x_P = temp1.p.x, y_P = temp1.p.y;
            double x_Q = temp1.q.x, y_Q = temp1.q.y;
            //A = y2 - y1;
            //B = x1 - x2;
            //C = -x1*y2 + y1*x2
            double minD_P = FLT_MAX, minD_Q = FLT_MAX;
            double d_P = 0, d_Q = 0;
            int polygonID_P = -1, polygonID_Q = -1, segmentID_P = -1, segmentID_Q = -1;
            for(int i = 0; i < polygons.size(); i++)
            {
                for(int j = 0; j < polygons.at(i).size(); j++){
                    if(cutThrough(polygons.at(i).at(j), scaleNormal)){
                        double M = //sqrt(A*A + B*B);
                                    getMABCOfLine(polygons.at(i).at(j), &A, &B, &C);
                        d_P = std::abs(A*x_P + B*y_P + C);
                        d_Q = std::abs(A*x_Q + B*y_Q + C);
                        //d = min(d_P, d_Q)/M;
                        d_P /= M;
                        d_Q /= M;
                        if(d_P < minD_P){
                            minD_P = d_P;
                            polygonID_P = i;
                            segmentID_P = j;
                        }
                        if(d_Q < minD_Q){
                            minD_Q = d_Q;
                            polygonID_Q = i;
                            segmentID_Q = j;
                        }
                    }
                }
            }

            double scaleX = (scaleNormal.q.x - scaleNormal.p.x);
            double scaleY = (scaleNormal.q.y - scaleNormal.p.y);

            if(polygonID_P != -1 && //minD > WIDTH &&
                                 segmentID_P != -1){
                lineSegment line = polygons.at(polygonID_P).at(segmentID_P);
                result1 = getSingleStep(line, scaleX, scaleY, minD_P);
            }

            if(polygonID_Q != -1 && //minD > WIDTH &&
                                 segmentID_Q != -1){
                lineSegment line = polygons.at(polygonID_Q).at(segmentID_Q);
                result2 = getSingleStep(line, scaleX, scaleY, minD_Q);
            }

            result = (result2 << 16) | result1;
            return result;
        }

        int getSingleStep(lineSegment line, double scaleX, double scaleY, double min){
            int result = 0;
            double lineX = (line.q.x - line.p.x);
            double lineY = (line.q.y - line.p.y);
            
            double dotProduct = lineX*scaleX + lineY*scaleY;
            double movement = min;
            if(dotProduct != 0){
                double cos = dotProduct/(sqrt(lineX*lineX + lineY*lineY)*sqrt(scaleX*scaleX + scaleY*scaleY));
                movement = min/std::abs(cos);
            }
            result = floor(movement/WIDTH);
            return result;
        }

        bool checkAvailableAtLast(vector<point> rightDirectionRoute, point p){
            if(rightDirectionRoute.size() == 0)
                return false;
            int last = rightDirectionRoute.size() - 1;
            if(rightDirectionRoute.at(last).x == p.x && rightDirectionRoute.at(last).y == p.y)
            {
                return true;
            }
            return false;
        }

        void insertTwoFirstPoints(vector<point> &route, vector<point> &rightDirectionRoute){
            double startX = route.at(0).x;
            double startY = route.at(0).y;

            double prevX = startX; 
            double prevY = startY;

            double currX = startX;
            double currY = startY + 1;
            double nextX = route.at(1).x, nextY = route.at(1).y;
            double uX, uY, vX, vY;

            rightDirectionRoute.push_back(route.at(0));
            lineSegment normalIn;
            normalIn.p.x = 0; normalIn.p.y = 0;
            
            uX = (currX - prevX);
            uY = currY - prevY; 
            vX = nextX - currX;
            vY = nextY - currY;
            double uv;
            if(nextX == currX - WIDTH/2){//vi currX la quy dao cua trong tam
                point p(nextX + WIDTH, nextY);
                rightDirectionRoute.push_back(p);
            }
            else if(nextX == currX + WIDTH/2){
                rightDirectionRoute.push_back(route.at(1));
            }
            else{
                uv = uX*vY - uY*vX;
                if(uv > 0){
                    double deltaY = nextY - startY;
                    double deltaX = nextX - (startX + WIDTH/2);
                    getNormalInAndOut(deltaX, deltaY, &normalIn.q.x, &normalIn.q.y);
                    point temp1;
                    temp1.x = nextX;
                    temp1.y = nextY;
                        
                    temp1.x += WIDTH*(normalIn.q.x - normalIn.p.x);
                    temp1.y += WIDTH*(normalIn.q.y - normalIn.p.y);
                    rightDirectionRoute.push_back(temp1);
                }
                else{
                    rightDirectionRoute.push_back(route.at(1));
                }
            }
        }

        void moveBothPoints(lineSegment tempLine, lineSegment &normalIn, vector< vector<lineSegment > > &polygons, vector<point> &rightDirectionRoute){
            /*int steps = getStepsAlongNormalVector(tempLine, normalIn, polygons);
            int stepP = steps & 65535;
            stepP = (stepP > 1) ? stepP - 1 : 0;
            int stepQ = steps >> 16;
            stepQ = (stepQ > 1) ? stepQ - 1 : 0;
            tempLine.p.x += stepP*WIDTH*(normalIn.q.x - normalIn.p.x);
            tempLine.p .y += stepP*WIDTH*(normalIn.q.y - normalIn.p.y);

            tempLine.q.x += stepQ*WIDTH*(normalIn.q.x - normalIn.p.x);
            tempLine.q.y += stepQ*WIDTH*(normalIn.q.y - normalIn.p.y);

            //tempLine.p = temp1;  tempLine.q = temp2;
            bool isValid = true;
            //while(numberOfCuttingThrough(polygons, tempLine) != 0)
            while(cutThroughPolygons(tempLine, polygons))
            {
                isValid = false;
                tempLine.p.x -= WIDTH*(normalIn.q.x - normalIn.p.x);
                tempLine.p.y -= WIDTH*(normalIn.q.y - normalIn.p.y);

                tempLine.q.x -= WIDTH*(normalIn.q.x - normalIn.p.x);
                tempLine.q.y -= WIDTH*(normalIn.q.y - normalIn.p.y);

                stepQ--;
                stepP--;
                if(stepQ >= 0 && stepP >= 0)
                {
                    isValid = true;
                }
                else{
                    break;
                }
            }
            if(isValid){
                if(!checkAvailableAtLast(rightDirectionRoute, tempLine.p))
                    rightDirectionRoute.push_back(tempLine.p);
                rightDirectionRoute.push_back(tempLine.q);    
            }*/
           /*
           To move both points, we need some steps:
                (1): find out the right roadside of two points: P, Q
                    A right roadside of two point P, Q: an edge E of a polygon which is in the right of AV
                    such that the maximum lines 
                        (generated from two points P, Q to two points of the edge E respectively) cut through no other edges
                    If there are several right roadsides exist, we choose the nearest line to P, Q.
                (2): pinpoint new coord of the first point: P (move the point along normal vector)
                    the new coord has a smallest distance (>= WIDTH) from the chosen right roadside
                (3): pinpoint new coord of the second point: Q (move the point along normal vector)
                    At here, we have two cases:
                        (a) the Q point is in the RIGHT side of the chosen roadside (an edge of a polygon by natural). 
                            In this case we simply reuse the coord of Q
                        (b) the Q point is in the LEFT side of the chosen roadside (an edge of a polygon by natural). 
                            the new coord has a smallest distance (>= WIDTH) from the chosen right roadside
           */
        }


        void checkInside(lineSegment tempLine, vector< vector<lineSegment > > &polygons, bool* pIsInside, bool* qIsInside){
            bool OyDirection = true;
            if(tempLine.p.x == tempLine.q.x){
                OyDirection = false;
            }

            int cP, cQ;
            for(int i = 0; i < polygons.size(); i++){
                if(!(*pIsInside)){
                    //cP = pnpoly(polygons[i], tempLine.p.x, tempLine.p.y, OyDirection);
                    cP = wn_PnPoly(tempLine.p, polygons[i]);
                    if(cP != 0)
                        *pIsInside = true;
                }
                if(!(*qIsInside)){
                    //cQ = pnpoly(polygons[i], tempLine.q.x, tempLine.q.y, OyDirection);
                    cQ = wn_PnPoly(tempLine.q, polygons[i]);
                    if(cQ != 0)
                        *qIsInside = true;
                }
                if((*pIsInside) && (*qIsInside))
                    break;
            }
        }


        void addMidPoint(point temp1, point temp2, vector<point> &rightDirectionRoute){
            if(std::abs(temp1.x - temp2.x) >= WIDTH)
            {
                if(std::abs(temp1.y - temp2.y) >= 6*WIDTH){
                    point temp3(temp1.x + WIDTH/2, (temp2.y + temp1.y)/2 - 6);
                    rightDirectionRoute.push_back(temp3);
                }
            }
        }


        int findRightRoadSide(point p, point q, vector<vector<lineSegment>> &polygons){
            int indexPolygon = -1;
            int indexLine = -1;
            int totalNumberCuttingThrough = 5;
            double uX = q.x - p.x;
            double uY = q.y - p.y;

            lineSegment line1, line2, line3, line4;
            lineSegment arr[4];
            for(int i = 0; i < polygons.size(); i++){
                for(int j = 0; j < polygons.at(i).size(); j++){
                    double v1X = polygons.at(i).at(j).p.x - p.x ;
                    double v1Y = polygons.at(i).at(j).p.y - p.y ;

                    double v2X = polygons.at(i).at(j).q.x - p.x ;
                    double v2Y = polygons.at(i).at(j).q.y - p.y ;

                    double u_v1 = uX*v1Y - uY*v1X;
                    double u_v2 = uX*v2Y - uY*v2X;
                    if(!(u_v1 > 0 && u_v2 > 0)){
                        line1.p = p;
                        line1.q = polygons.at(i).at(j).p;

                        line2.p = p;
                        line2.q = polygons.at(i).at(j).q;

                        line3.p = q;
                        line3.q = polygons.at(i).at(j).p;

                        line4.p = q;
                        line4.q = polygons.at(i).at(j).q;

                        arr[0] = line1;  arr[1] = line2;
                        arr[2] = line2;  arr[3] = line3;
                        int c = 0;
                        for(int k = 0; k < 4; k++){
                            for(int m = 0; m <j ; m++){
                                c += cutThrough(arr[k],polygons[i][m]);
                            }

                            for(int m = j + 1; m < polygons.at(i).size() ; m++){
                                c += cutThrough(arr[k],polygons[i][m]);
                            }

                        }

                        if(c < totalNumberCuttingThrough){
                            totalNumberCuttingThrough = c;
                            indexPolygon = i;
                            indexLine = j;
                        }
                    }
                }
            }

            int result = (indexPolygon << 16) | indexLine;
            return result;
        }
        

        void rotateOnePoint(point currentPoint, bool pIsInside, bool qIsInside, point temp1, point temp2, 
                            double arr[], vector<vector<lineSegment>> &polygons, vector<point> &rightDirectionRoute)
        {
            double nextX = arr[0];
            double nextY = arr[1];
            double currX = arr[2];
            double currY = arr[3];

            double anchorX = pIsInside ? currX : nextX;
            double anchorY = pIsInside ? currY : nextY;
            double moveX = pIsInside ? temp2.x : temp1.x;
            double moveY = pIsInside ? temp2.y : temp1.y;
            double deltaX = pIsInside ? temp2.x - currX : nextX - temp1.x;
            double deltaY = pIsInside ? temp2.y - currY : nextY - temp1.y;
            lineSegment tempLine;
            tempLine.p.x = anchorX;
            tempLine.p.y = anchorY;
            tempLine.q.x = moveX;
            tempLine.q.y = moveY;

            lineSegment normalIn;
            normalIn.p.x = 0; normalIn.p.y = 0;

            getNormalInAndOut(deltaX, deltaY, &normalIn.q.x, &normalIn.q.y);    

            int step = 0;

            double changedX = WIDTH*normalIn.q.x;
            double changedY = WIDTH*normalIn.q.y;

            do{
                moveX += changedX;
                moveY += changedY;
                tempLine.q.x = moveX;
                tempLine.q.y = moveY;
                step++;
            }
            //while(numberOfCuttingThrough(polygons, tempLine) == 0);
            //while(!cutThroughPolygons(tempLine, polygons));
            while(checkCollisionRegardlessVirtualGate(tempLine, polygons) == 0);

            //step = 0;
            int backward = 0;
            //do
            {
                moveX -= changedX;
                moveY -= changedY;
                //tempLine.q.x -= backwardX;
                //tempLine.q.y -= backwardY;
                tempLine.q.x = moveX;
                tempLine.q.y = moveY;
                backward++;
            }
            //1 is the thickness of parking wall
            while(!(tempLine.q.x > min_x + 1 && tempLine.q.x < max_x - 1
                    && tempLine.q.y > min_y + 1 && tempLine.q.y < max_y - 1)
                    );

            backward = 0;
            int indexes = findRightRoadSide(temp1, temp2, polygons);
            int poly = indexes >> 16;
            int edge = (indexes & 0xFFFF);

            if(poly != -1 && edge != -1){
                lineSegment line = polygons.at(poly).at(edge);
                double A = 0, B = 0, C = 0;
                double M = getMABCOfLine(line, &A, &B, &C);
                double D = std::abs(A*tempLine.q.x + B*tempLine.q.y + C)/M;

                //bool enoughFar = shortestDistanceRequirement(tempLine, polygons, WIDTH);
                //while(!enoughFar) 
                while(D < WIDTH)
                {
                    moveX -= changedX;
                    moveY -= changedY;
                    //tempLine.q.x -= backwardX;
                    //tempLine.q.y -= backwardY;
                    tempLine.q.x = moveX;
                    tempLine.q.y = moveY;
                    backward++;
                    //enoughFar = shortestDistanceRequirement(tempLine, polygons, WIDTH);
                    D = std::abs(A*tempLine.q.x + B*tempLine.q.y + C)/M;
                    cout<<"Backward "<<backward<<" D = "<<D<<endl;

                }
                            //&& !cutThroughPolygons(tempLine, polygons) 
                //            && backward <= 1
                //            );
            }
        
            if(qIsInside){
                temp1.x = moveX;
                temp1.y = moveY;
                if(!checkAvailableAtLast(rightDirectionRoute, temp1)){
                    rightDirectionRoute.push_back(temp1);
                    addMidPoint(temp1, currentPoint, rightDirectionRoute);
                }
                rightDirectionRoute.push_back(currentPoint);
            }
            else{
                temp2.x = moveX;
                temp2.y = moveY;
                rightDirectionRoute.push_back(temp2);
            }
        }

        void getToRightSide(point currentPoint, lineSegment normalIn, vector<vector<lineSegment>> &polygons, double arr[], vector<point> &rightDirectionRoute){
            double nextX = arr[0];
            double nextY = arr[1];
            double currX = arr[2];
            double currY = arr[3];

            lineSegment tempLine;

            point temp1, temp2;
                    
            double deltaY = nextY - currY;
            double deltaX = nextX - currX;
            getNormalInAndOut(deltaX, deltaY, &normalIn.q.x, &normalIn.q.y);
                
            temp1.x = currX + WIDTH*(normalIn.q.x - normalIn.p.x);
            temp1.y = currY + WIDTH*(normalIn.q.y - normalIn.p.y);
            
            temp2.x = nextX + WIDTH*(normalIn.q.x - normalIn.p.x);
            temp2.y = nextY + WIDTH*(normalIn.q.y - normalIn.p.y);
            
            tempLine.p = temp1;  tempLine.q = temp2;
            if(numberOfTouchingPoints(polygons, tempLine) == 0){
                moveBothPoints(tempLine, normalIn, polygons, rightDirectionRoute);
            }
            else{
                bool pIsInside = false, qIsInside = false;
                checkInside(tempLine, polygons, &pIsInside, &qIsInside);
                
                if(!pIsInside && !qIsInside){
                    if(!checkAvailableAtLast(rightDirectionRoute, temp1))
                        rightDirectionRoute.push_back(temp1);
                    rightDirectionRoute.push_back(temp2);
                }
                
                if(pIsInside ^ qIsInside){
                    rotateOnePoint(currentPoint, pIsInside, qIsInside, temp1, temp2, arr, polygons, rightDirectionRoute);
                }

                if(pIsInside && qIsInside){
                    rightDirectionRoute.push_back(currentPoint);
                }
                    
            }
        }

        void rotateLastSegment(vector<point> route, vector<vector<lineSegment>> &polygons, vector<point> &rightDirectionRoute){
            int preLast = route.size() - 2;
            bool alreadyAddedLastPoint = false;
            if(preLast > 0){
                double currX = route.at(preLast).x;
                double currY = route.at(preLast).y;
                double nextX = route.at(preLast + 1).x;
                double nextY = route.at(preLast + 1).y;
                double biasX = 0, biasY = 0;
                getNormalInAndOut(nextX - currX, nextY - currY, &biasX, &biasY);

                lineSegment line;
                line.p.x = currX + biasX*WIDTH;
                line.p.y = currY + biasY*WIDTH;
                line.q.x = nextX;
                line.q.y = nextY;

                //if(numberOfCuttingThrough(polygons, line) == 0)
                int collision = checkCollisionRegardlessVirtualGate(line, polygons);
                    
                if(collision == 0)                
                {
                    double arr[4] = {nextX, nextY, currX, currY};
                    rotateOnePoint(route.at(preLast + 1), false, true, route.at(preLast), route.at(preLast + 1), arr, polygons, rightDirectionRoute);
                    alreadyAddedLastPoint = true;
                }
                
            }

            if(!alreadyAddedLastPoint){
                rightDirectionRoute.push_back(route.at(preLast + 1));
            }

            
        }

        vector<point> calculateClockwise(vector<point> &route, vector< vector<lineSegment > > &polygons){
            vector<point> rightDirectionRoute;
            insertTwoFirstPoints(route, rightDirectionRoute);
            double prevX = route.at(0).x;           double prevY = route.at(0).y;

            double currX = route.at(1).x;           double currY = route.at(1).y;
            double nextX, nextY;
            double uX, uY, vX, vY, uv;
            lineSegment normalIn;
            normalIn.p.x = 0; normalIn.p.y = 0;
            lineSegment tempLine;

            for(int i = 2; i < route.size() - 1; i++){
                nextX = route.at(i).x;             nextY = route.at(i).y;
                uX = (currX - prevX);              uY = currY - prevY;
                vX = nextX - currX;                vY = nextY - currY;
                uv = uX*vY - uY*vX;
                if(uv < 0){
                    rightDirectionRoute.push_back(route.at(i));  //clockwises.push_back(YES);//Clockwise
                }
                else if(uv >= 0){
                    //Assuming that the AV never moves straight backward (Không đi giật lùi)
                    double arr[4] = {nextX, nextY, currX, currY};
                    getToRightSide(route.at(i), normalIn, polygons, arr, rightDirectionRoute);
                    
                }
                cout<<endl;
                for(int j = 0; j < rightDirectionRoute.size(); j++){
                    cout<<"("<<rightDirectionRoute.at(j).x<<", "<<rightDirectionRoute.at(j).y<<") ";
                }
                prevX = currX;             prevY = currY;
                currX = nextX;             currY = nextY;
                
            }

            rotateLastSegment(route, polygons, rightDirectionRoute);

            return rightDirectionRoute;
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

        vector<point> collisionFreeOfAVHead(vector<point> &route, vector< vector<lineSegment > > &polygons){
            vector<point> rightDirectionRoute;

            return rightDirectionRoute;
        }
};
#endif