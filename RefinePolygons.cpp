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
                //removeByIndex(this->points, index);

        }

        int countSharedVertices(int indexOfStack, int row, int column, int *first, int *last, int *polygonIndex){
            this->getFourVertecies(indexOfStack, row, column);
            bool found = false;
            //for(int i = 0; i < this->NUMBER_STACKS; i++)
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

        void removeEdgesAndVertices(int indexOfStack, int row, int column){
            int first = -1, last = -1, polygonIndex = -1;
            int nmrSameVertices = this->countSharedVertices(indexOfStack, row, column, &first, &last, &polygonIndex);
            if(nmrSameVertices == 4){
                removePoint(this->pA);
                removePoint(this->pB);
                removePoint(this->pC);
                removePoint(this->pD);
            }
        }
};
#endif