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

#include "BuildPolygonState.cpp"

#ifndef _REFINE_POLYGON_
#define _REFINE_POLYGON_

using namespace std;

class RefinePolygons : public BuildingPolygons{
    public:
        bool sharedVertices[4] = {false, false, false, false};
        RefinePolygons(int Row, int Column, int cordX, int cordY, int width, int length)
            :  BuildingPolygons(Row, Column, cordX, cordY, width, length)
        {

        }

        void removePoint(point p){
            int index = 0;
            bool found = false;
            for(int i = 0; i < this->points.size(); i++){
                if(this->points.at(i) == p)
                {
                    index = i;
                    found = true;
                    break;
                }
            }
            if(found){
                this->points.erase(this->points.begin() + index);
            }
                
        }

        void removeLineSegment(point p, int polygonIndex){
            //cout<<"Remove all line segments which have p("<<p.x<<", "<<p.y<<")"<<endl;
            int index1 = 0;
            int index2 = 0;
            bool found1 = false;
            bool found2 = false;
            for(int i = 0; i < this->polygons.at(polygonIndex).size(); i++){
                if(!found1 && this->polygons.at(polygonIndex).at(i).p == p)    
                {
                    index1 = i;
                    found1 = true;
                }
                else if(!found2 && this->polygons.at(polygonIndex).at(i).q == p){
                    index2 = i;
                    found2 = true;
                }
                if(found1 && found2)
                    break;
            }
            if(found1){
                this->polygons.at(polygonIndex).erase(this->polygons.at(polygonIndex).begin() + index1);
            }
            if(found2){
                if(index1 < index2 && found1)
                    index2--;
                this->polygons.at(polygonIndex).erase(this->polygons.at(polygonIndex).begin() + index2);
            }

            
        }

        int countSharedVertices(int indexOfStack, int row, int column, int *first, int *last, int *polygonIndex){
            this->getFourVertecies(indexOfStack, row, column);
            bool found = false;
            
            int count = 0;
            sharedVertices[0] = false;
            sharedVertices[1] = false;
            sharedVertices[2] = false;
            sharedVertices[3] = false;
            
            int prev = 0;
            
            for(int i = 0; i < this->polygons.size(); i++){
                for(int j = 0; j < this->polygons.at(i).size(); j++){
                    if(!sharedVertices[0] && this->polygons.at(i).at(j).p == pA)
                    {
                        sharedVertices[0] = true;
                        count++;
                        
                    }
                    else if(!sharedVertices[1] && this->polygons.at(i).at(j).p == pB)
                    {
                        sharedVertices[1] = true; 
                        count++;
                    }
                    else if(!sharedVertices[2] && this->polygons.at(i).at(j).p == pC)
                    {
                        sharedVertices[2] = true;
                        count++;
                    }
                    else if(!sharedVertices[3] && this->polygons.at(i).at(j).p == pD)
                    {
                        sharedVertices[3] = true;
                        count++;
                    }
                    if(prev != count){
                        if(*first == -1)
                            *first = j;
                        else
                            *last = j;
                        prev = count;
                    }
                    if(count == 4)
                        break;
                }
                if(count > 0){
                    *polygonIndex = i;
                    found = true;
                    break;
                }
            }
            
            return count;
        }

        void removeEdgesAndVertices(int indexOfStack, int row, int column) override{
            int first = -1, last = -1, polygonIndex = -1;
            int nmrSameVertices = this->countSharedVertices(indexOfStack, row, column, &first, &last, &polygonIndex);
            
            #pragma region
            if(nmrSameVertices == 4){
                removePoint(this->pA);
                removePoint(this->pB);
                removePoint(this->pC);
                removePoint(this->pD);
                
                removeLineSegment(this->pA, polygonIndex);
                removeLineSegment(this->pB, polygonIndex);
                removeLineSegment(this->pC, polygonIndex);
                removeLineSegment(this->pD, polygonIndex);

                if(!this->polygons.at(polygonIndex).empty()){
                    bool foundBreak = false;
                    int breakIndex = 0;
                    double x1, x2, y1, y2;
                    for(int i = 0; i < this->polygons.at(polygonIndex).size(); i++){
                        int j = (i + 1) % this->polygons.at(polygonIndex).size();
                        if(!(this->polygons.at(polygonIndex).at(i).q == this->polygons.at(polygonIndex).at(j).p))
                        {
                            x1 = this->polygons.at(polygonIndex).at(i).q.x;
                            y1 = this->polygons.at(polygonIndex).at(i).q.y;

                            x2 = this->polygons.at(polygonIndex).at(j).p.x;
                            y2 = this->polygons.at(polygonIndex).at(j).p.y;
                            breakIndex = i;
                            foundBreak = true;
                            break;
                        }
                    }

                    if(foundBreak){
                        lineSegment line ;
                        line.p.x = x1;
                        line.p.y = y1;

                        line.q.x = x2;
                        line.q.y = y2;

                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line);
                    }


                }
            } else if(nmrSameVertices == 3){
                point p_o; // if nmrSameVertices == 3, so have 1 point in AV is not belong points, it is p_o
                // p_o is pA, pB, pC or pD. if pX not find in points, it is p_o
                if(find(points.begin(), points.end(), this->pA) == points.end()){
                    // p_o.x = this->pA.x;
                    // p_o.y = this->pA.y;
                    p_o = this->pA;
                } else if(find(points.begin(), points.end(), this->pB) == points.end()){
                    // p_o.x = this->pB.x;
                    // p_o.y = this->pB.y;
                    p_o = this->pB;
                } else if(find(points.begin(), points.end(), this->pC) == points.end()){
                    // p_o.x = this->pC.x;
                    // p_o.y = this->pC.y;
                    p_o = this->pC;
                } else if(find(points.begin(), points.end(), this->pD) == points.end()){
                    // p_o.x = this->pD.x;
                    // p_o.y = this->pD.y;
                    p_o = this->pD;
                }

                // remove point in AV, only points that are vertices of the polygons
                if(!(this->pA == p_o)){
                    removePoint(this->pA);
                    removeLineSegment(this->pA, polygonIndex);
                }

                if(!(this->pB == p_o)){
                    removePoint(this->pB);
                    removeLineSegment(this->pB, polygonIndex);
                }

                if(!(this->pC == p_o)){
                    removePoint(this->pC);
                    removeLineSegment(this->pC, polygonIndex);
                }

                if(!(this->pD == p_o)){
                    removePoint(this->pD);
                    removeLineSegment(this->pD, polygonIndex);
                }

                if(!this->polygons.at(polygonIndex).empty()){
                    bool foundBreak = false;
                    int breakIndex = 0;
                    double x1, x2, y1, y2;
                    for(int i = 0; i < this->polygons.at(polygonIndex).size(); i++){
                        int j = (i + 1) % this->polygons.at(polygonIndex).size();
                        if(!(this->polygons.at(polygonIndex).at(i).q == this->polygons.at(polygonIndex).at(j).p))
                        {
                            x1 = this->polygons.at(polygonIndex).at(i).q.x;
                            y1 = this->polygons.at(polygonIndex).at(i).q.y;

                            x2 = this->polygons.at(polygonIndex).at(j).p.x;
                            y2 = this->polygons.at(polygonIndex).at(j).p.y;
                            breakIndex = i;
                            foundBreak = true;
                            break;
                        }
                    }

                    if(foundBreak){
                        lineSegment line1;
                        lineSegment line2;
                        line1.p.x = x1;
                        line1.p.y = y1;

                        line1.q = p_o;

                        line2.p = p_o; // line2.p == line1.q

                        line2.q.x = x2;
                        line2.q.y = y2;

                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
                    }


                }
            } else if(nmrSameVertices == 2){
                point p_o1;
                point p_o2; 
                
                // if nmrSameVertices == 2, so have 2 point in AV is not belong points, it is p_o1, p_o2
                // p_o1, p_o2 is pA, pB, pC or pD. if pX not find in points, it is p_o1 or p_o2
                if(find(points.begin(), points.end(), this->pA) == points.end()){
                    p_o1 = this->pA;
                } else if(find(points.begin(), points.end(), this->pB) == points.end()){
                    if(p_o1 == this->pA){
                        p_o2 = this->pB;
                    } else {
                        p_o1 = this->pB;
                    }
                } else if(find(points.begin(), points.end(), this->pC) == points.end()){
                    if(p_o1 == this->pA || p_o1 == this->pB){
                        p_o2 = this->pC;
                    } else {
                        p_o1 = this->pC;
                    }
                } else if(find(points.begin(), points.end(), this->pD) == points.end()){
                    p_o2 = this->pD;
                }

                // remove point in AV, only points that are vertices of the polygons
                if(!(this->pA == p_o1) && !(this->pA == p_o2)){
                    removePoint(this->pA);
                    removeLineSegment(this->pA, polygonIndex);
                }

                if(!(this->pB == p_o1) && !(this->pB == p_o2)){
                    removePoint(this->pB);
                    removeLineSegment(this->pB, polygonIndex);
                }

                if(!(this->pC == p_o1) && !(this->pC == p_o2)){
                    removePoint(this->pC);
                    removeLineSegment(this->pC, polygonIndex);
                }

                if(!(this->pD == p_o1) && !(this->pD == p_o2)){
                    removePoint(this->pD);
                    removeLineSegment(this->pD, polygonIndex);
                }

                if(!this->polygons.at(polygonIndex).empty()){
                    bool foundBreak = false;
                    int breakIndex = 0;
                    double x1, x2, y1, y2;
                    for(int i = 0; i < this->polygons.at(polygonIndex).size(); i++){
                        int j = (i + 1) % this->polygons.at(polygonIndex).size();
                        if(!(this->polygons.at(polygonIndex).at(i).q == this->polygons.at(polygonIndex).at(j).p))
                        {
                            x1 = this->polygons.at(polygonIndex).at(i).q.x;
                            y1 = this->polygons.at(polygonIndex).at(i).q.y;

                            x2 = this->polygons.at(polygonIndex).at(j).p.x;
                            y2 = this->polygons.at(polygonIndex).at(j).p.y;
                            breakIndex = i;
                            foundBreak = true;
                            break;
                        }
                    }

                    if(foundBreak){
                        lineSegment line1;
                        lineSegment line2;
                        lineSegment line3;

                        line1.p.x = x1;
                        line1.p.y = y1;

                        line1.q = p_o1;

                        line2.p = p_o1; // line2.p == line1.q

                        line2.q = p_o2;

                        line3.p = p_o2; // line3.p == line2.q

                        line3.q.x = x2;
                        line3.q.y = y2;

                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 3), line3);
                    }


                }

            } else if(nmrSameVertices == 1){
                point p_o1;
                point p_o2;
                point p_o3;

                // if nmrSameVertices == 1, so have 3 point in AV is not belong points, it is p_o1, p_o2, p_o3
                // p_o1, p_o2, p_o3 is pA, pB, pC or pD. if pX not find in points, it is p_o1 or p_o2, p_o3
                if(find(points.begin(), points.end(), this->pA) == points.end()){
                    p_o1 = this->pA;
                } else if(find(points.begin(), points.end(), this->pB) == points.end()){
                    if(p_o1 == this->pA){
                        p_o2 = this->pB;
                    } else {
                        p_o1 = this->pB;
                    }
                } else if(find(points.begin(), points.end(), this->pC) == points.end()){
                    if(p_o1 == this->pA && p_o2 == this->pB){
                        p_o3 = this->pC;
                    } else if(p_o1 == this->pA && !(p_o2 == this->pB)){
                        p_o2 = this->pC;
                    } else if(!(p_o1 == this->pA) && p_o2 == this->pB){
                        p_o2 = this->pC;
                    }
                } else if(find(points.begin(), points.end(), this->pD) == points.end()){
                    p_o3 = this->pD;
                }

                // remove point in AV, only points that are vertices of the polygons
                if(!(this->pA == p_o1) && !(this->pA == p_o2) && !(this->pA == p_o3)){
                    removePoint(this->pA);
                    removeLineSegment(this->pA, polygonIndex);
                }

                if(!(this->pB == p_o1) && !(this->pB == p_o2) && !(this->pB == p_o3)){
                    removePoint(this->pB);
                    removeLineSegment(this->pB, polygonIndex);
                }

                if(!(this->pC == p_o1) && !(this->pC == p_o2) && !(this->pC == p_o3)){
                    removePoint(this->pC);
                    removeLineSegment(this->pC, polygonIndex);
                }

                if(!(this->pD == p_o1) && !(this->pD == p_o2) && !(this->pD == p_o3)){
                    removePoint(this->pD);
                    removeLineSegment(this->pD, polygonIndex);
                }

                if(!this->polygons.at(polygonIndex).empty()){
                    bool foundBreak = false;
                    int breakIndex = 0;
                    double x1, x2, y1, y2;
                    for(int i = 0; i < this->polygons.at(polygonIndex).size(); i++){
                        int j = (i + 1) % this->polygons.at(polygonIndex).size();
                        if(!(this->polygons.at(polygonIndex).at(i).q == this->polygons.at(polygonIndex).at(j).p))
                        {
                            x1 = this->polygons.at(polygonIndex).at(i).q.x;
                            y1 = this->polygons.at(polygonIndex).at(i).q.y;

                            x2 = this->polygons.at(polygonIndex).at(j).p.x;
                            y2 = this->polygons.at(polygonIndex).at(j).p.y;
                            breakIndex = i;
                            foundBreak = true;
                            break;
                        }
                    }

                    if(foundBreak){
                        lineSegment line1;
                        lineSegment line2;
                        lineSegment line3;
                        lineSegment line4;

                        line1.p.x = x1;
                        line1.p.y = y1;

                        line1.q = p_o1;

                        line2.p = p_o1; // line2.p == line1.q
                        line2.q = p_o2;

                        line3.p = p_o2; // line3.p == line2.q
                        line3.q = p_o3;

                        line4.p = p_o3; // line4.p == line3.q

                        line4.q.x = x2;
                        line4.q.y = y2;

                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 3), line3);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 4), line4);
                    }


                }
            } else if(nmrSameVertices == 0){
                int breakIndex = 0;
                // find lineSegment contain point of AV
                for(int i = 0; i < this->polygons.at(polygonIndex).size(); i++){
                    if(isPointInsideLineSegment(polygons.at(polygonIndex).at(i), this->pA)
                        || isPointInsideLineSegment(polygons.at(polygonIndex).at(i), this->pB)
                        || isPointInsideLineSegment(polygons.at(polygonIndex).at(i), this->pC)
                        || isPointInsideLineSegment(polygons.at(polygonIndex).at(i), this->pD)){
                            breakIndex = i;
                            break;
                        }
                }

                
            }
            #pragma endregion 
        
            

        }
};
#endif