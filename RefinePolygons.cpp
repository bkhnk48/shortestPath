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

        // calc euclid distance between two point
        // imporve : init class/interface distance and euclid_distance implement/extends distance class/interface
        double euclid_distance(point a, point b){
            return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        }

        // swap 2 point, improve: replace by template
        void swap_point(point &point1, point &point2){
            point tmp_point = point1;
            point1 = point2;
            point2 = tmp_point;

            return;
        }

        // swap 2 double parameter, improve: replace by template
        void swap_double(double &dis_1, double &dis_2){
            double tmp_dis = dis_1;
            dis_1 = dis_2;
            dis_2 = tmp_dis;

            return;
        }

        // s_point is the first break found clockwise, and e_point is end point
        // the method will sort p_o1 to p_o4, so that p_o1 to p_o4 order nearest and furthest compare to s_point follow clockwise
        void sort_clockwise(point &s_point, point &e_point, point &p_o1, point &p_o2, point &p_o3, point &p_o4, int cnt){
            if(cnt == 1){ // if cnt == 1, we only have p_o1, so no need sort_clockwise
                return;
            }

            if(cnt == 2){ // we have two point is p_o1 and p_o2
                double dis_1 = euclid_distance(s_point, p_o1);
                double dis_2 = euclid_distance(s_point, p_o2);

                if(dis_1 < dis_2){
                    return;
                } else { // if 2 point are reversed, we just reposition them
                    swap_point(p_o1, p_o2);
                    return;
                }
            }

            if(cnt == 3){
                double dis_1 = euclid_distance(s_point, p_o1);
                double dis_2 = euclid_distance(s_point, p_o2);
                double dis_3 = euclid_distance(s_point, p_o3);

                if(dis_1 > dis_2){
                    swap_point(p_o1, p_o2);
                    swap_double(dis_1, dis_2);
                }

                if(dis_1 > dis_3){
                    swap_point(p_o1, p_o3);
                    swap_double(dis_1, dis_3);
                } // after step, p_o1 is true location, we need replace p_o2 and p_o3 if need

                dis_2 = euclid_distance(e_point, p_o2);
                dis_3 = euclid_distance(e_point, p_o3);

                if(dis_2 < dis_3){
                    swap_point(p_o2, p_o3);
                    // swap_double(dis_2, dis_3);
                }

                return;
            }

            if(cnt == 4){
                double dis_1 = euclid_distance(s_point, p_o1);
                double dis_2 = euclid_distance(s_point, p_o2);
                double dis_3 = euclid_distance(s_point, p_o3);
                double dis_4 = euclid_distance(s_point, p_o4);

                if(dis_1 > dis_2){
                	swap_point(p_o1, p_o2);
                	swap_double(dis_1, dis_2);
                }

                if(dis_1 > dis_3){
                	swap_point(p_o1, p_o3);
                	swap_double(dis_1, dis_3);
                }

                if(dis_1 > dis_4){
                	swap_point(p_o1, p_o4);
                	swap_double(dis_1, dis_4);
                } // after step, p_o1 is point nearest start point
                // next step, we need swap p_o4 is nearest end point

                dis_2 = euclid_distance(e_point, p_o2);
                dis_3 = euclid_distance(e_point, p_o3);
                dis_4 = euclid_distance(e_point, p_o4);

                if(dis_4 > dis_2){
                	swap_point(p_o4, p_o2);
                	swap_double(dis_4, dis_2);
                }

                if(dis_4 > dis_3){
                	swap_point(p_o4, p_o2);
                	swap_double(dis_4, dis_4);
                } // after step p_o4 is nearest end point

                // next step, we must sort p_o2, p_o3 such as: p_o2 near p_o1 than p_o3 and p_o3 near p_o4 than p_o2
                dis_2 = euclid_distance(p_o1, p_o2);
                dis_3 = euclid_distance(p_o1, p_o3);

                if(dis_2 > dis_3){
                	swap_point(p_o2, p_o3);
                	swap_double(dis_2, dis_3);
                }

            }
        }

        // when delete lineSegment, polygon (in pylygons with index polygonIndex) was broken
        // method find 2 point break in polygon
        void findBreakPoint(double &x1, double &y1, double &x2, double &y2, int polygonIndex, int &breakIndex, bool &foundBreak){
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
        }
        
        // get 4 vertices of AV assign to p_o1, p_o2, p_o3, p_o4
        // if vertice in points push back to p_o4
        // if vertice not in points push head to p_o1
        void getVerticesAV(point &p_o1, point &p_o2, point &p_o3, point &p_o4){
            deque<point> queue;
            
            if(find(points.begin(), points.end(), this->pA) != points.end()){
                queue.push_back(this->pA);
            } else {
                queue.push_front(this->pA);
            }

            if(find(points.begin(), points.end(), this->pB) != points.end()){
                queue.push_back(this->pB);
            } else {
                queue.push_front(this->pB);
            }

            if(find(points.begin(), points.end(), this->pC) != points.end()){
                queue.push_back(this->pC);
            } else {
                queue.push_front(this->pC);
            }

            if(find(points.begin(), points.end(), this->pD) != points.end()){
                queue.push_back(this->pD);
            } else {
                queue.push_front(this->pD);
            }

            p_o1 = queue.at(0);
            p_o2 = queue.at(1);
            p_o3 = queue.at(2);
            p_o4 = queue.at(3);
        }

        void removeEdgesAndVertices(int indexOfStack, int row, int column) override{
            int first = -1, last = -1, polygonIndex = -1;
            point p_o1, p_o2, p_o3, p_o4; // init 4 vertices of AV
            int nmrSameVertices = this->countSharedVertices(indexOfStack, row, column, &first, &last, &polygonIndex);
//            int nmrSameVertices = 0;
//            polygonIndex = 0;
//            pA = point(2,10);
//            pB = point(3,10);
//            pC = point(2,9);
//            pA = point(3,9);
            
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
                getVerticesAV(p_o1, p_o2, p_o3, p_o4);

                // remove vertices and edge with vertices in points
                removePoint(p_o4);
                removeLineSegment(p_o4, polygonIndex);

                removePoint(p_o3);
                removeLineSegment(p_o3, polygonIndex);

                removePoint(p_o2);
                removeLineSegment(p_o2, polygonIndex);

                if(!this->polygons.at(polygonIndex).empty()){
                    bool foundBreak = false;
                    int breakIndex = 0;
                    double x1, x2, y1, y2;
                    findBreakPoint(x1, y1, x2, y2, polygonIndex, breakIndex, foundBreak);

                    point s_point, e_point;
                    s_point.x = x1;
                    s_point.y = y1;
                    e_point.x = x2;
                    e_point.y = y2;

                    // sort p_o1, p_o2, p_o3, p_o4
                    sort_clockwise(s_point, e_point, p_o1, p_o2, p_o3, p_o4, 3);

                    if(foundBreak){
                        lineSegment line1;
                        lineSegment line2;
                        line1.p = s_point;
                        line1.q = p_o1;

                        line2.p = p_o1; // line2.p == line1.q
                        line2.q = e_point;

                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
                    }


                }

                points.push_back(p_o1);
            } else if(nmrSameVertices == 2){
                getVerticesAV(p_o1, p_o2, p_o3, p_o4);

                // remove vertices and edge with vertices in points
                removePoint(p_o4);
                removeLineSegment(p_o4, polygonIndex);

                removePoint(p_o3);
                removeLineSegment(p_o3, polygonIndex);

                if(!this->polygons.at(polygonIndex).empty()){
                    bool foundBreak = false;
                    int breakIndex = 0;
                    double x1, x2, y1, y2;
                    findBreakPoint(x1, y1, x2, y2, polygonIndex, breakIndex, foundBreak);

                    point s_point, e_point;
                    s_point.x = x1;
                    s_point.y = y1;
                    e_point.x = x2;
                    e_point.y = y2;

                    // sort p_o1, p_o2, p_o3, p_o4
                    sort_clockwise(s_point, e_point, p_o1, p_o2, p_o3, p_o4, 2);

                    if(foundBreak){
                        lineSegment line1;
                        lineSegment line2;
                        lineSegment line3;

                        line1.p = s_point;
                        line1.q = p_o1;

                        line2.p = p_o1; // line2.p == line1.q
                        line2.q = p_o2;

                        line3.p = p_o2; // line3.p == line2.q
                        line3.q = e_point;

                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 3), line3);
                    }


                }
                // push new point to points pool
                points.push_back(p_o1);
                points.push_back(p_o2);
            } else if(nmrSameVertices == 1){
                getVerticesAV(p_o1, p_o2, p_o3, p_o4);

                // remove vertices and edge with vertices in points
                removePoint(p_o4);
                removeLineSegment(p_o4, polygonIndex);

                if(!this->polygons.at(polygonIndex).empty()){
                    bool foundBreak = false;
                    int breakIndex = 0;
                    double x1, x2, y1, y2;
                    findBreakPoint(x1, y1, x2, y2, polygonIndex, breakIndex, foundBreak);

                    point s_point, e_point;
                    s_point.x = x1;
                    s_point.y = y1;
                    e_point.x = x2;
                    e_point.y = y2;

                    // sort p_o1, p_o2, p_o3, p_o4
                    sort_clockwise(s_point, e_point, p_o1, p_o2, p_o3, p_o4, 2);

                    if(foundBreak){
                        lineSegment line1;
                        lineSegment line2;
                        lineSegment line3;
                        lineSegment line4;

                        line1.p = s_point;
                        line1.q = p_o1;

                        line2.p = p_o1; // line2.p == line1.q
                        line2.q = p_o2;

                        line3.p = p_o2; // line3.p == line2.q
                        line3.q = p_o3;

                        line4.p = p_o3; // line4.p == line3.q
                        line4.q = e_point;

                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 3), line3);
                        this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 4), line4);
                    }


                }
                // push new point to points pool
                points.push_back(p_o1);
                points.push_back(p_o2);
                points.push_back(p_o3);
            } else if(nmrSameVertices == 0){
            	int check_A = isPointInsidePolygon(polygons, polygonIndex, this->pA); // find index line segment include point pA
            	int check_B = isPointInsidePolygon(polygons, polygonIndex, this->pB); // find index line segment include point pB
            	int check_C = isPointInsidePolygon(polygons, polygonIndex, this->pC); // find index line segment include point pC
            	int check_D = isPointInsidePolygon(polygons, polygonIndex, this->pD); // find index line segment include point pD

                // find lineSegment contain point of AV, only find one
            	int breakIndex = 0;
                if(check_A != -1){
                	breakIndex = check_A;
                } else if(check_B != -1){
                	breakIndex = check_B;
                } else if(check_C != -1){
                	breakIndex = check_C;
                } else if(check_D != -1){
                	breakIndex = check_D;
                }
                

                lineSegment lineCommon = polygons.at(polygonIndex).at(breakIndex);
                point p_common = lineCommon.p;

                removePoint(p_common);
                removeLineSegment(p_common, polygonIndex);

                // reconnect 1 line when delete
                if(!this->polygons.at(polygonIndex).empty()){
					bool foundBreak = false;
					int breakIndex = 0;
					double x1, x2, y1, y2;
					findBreakPoint(x1, y1, x2, y2, polygonIndex, breakIndex, foundBreak);

					lineSegment line;
					line.p = p_common;
					line.q.x = x1;
					line.q.y = y1;

					this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line);
					points.push_back(p_common);
                }

                // connect polygons again
                if(!this->polygons.at(polygonIndex).empty()){
					bool foundBreak = false;
					int breakIndex = 0;
					double x1, x2, y1, y2;
					findBreakPoint(x1, y1, x2, y2, polygonIndex, breakIndex, foundBreak);

					point s_point, e_point;
					s_point.x = x1;
					s_point.y = y1;
					e_point.x = x2;
					e_point.y = y2;

					// sort p_o1, p_o2, p_o3, p_o4
					sort_clockwise(s_point, e_point, p_o1, p_o2, p_o3, p_o4, 2);

					if(foundBreak){
						lineSegment line1;
						lineSegment line2;
						lineSegment line3;
						lineSegment line4;
						lineSegment line5;

						line1.p = s_point;
						line1.q = p_o1;

						line2.p = p_o1; // line2.p == line1.q
						line2.q = p_o2;

						line3.p = p_o2; // line3.p == line2.q
						line3.q = p_o3;

						line4.p = p_o3; // line4.p == line3.q
						line4.q = p_o4;

						line5.p = p_o4; // line5.p = line4.q
						line4.q = e_point;

						this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
						this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
						this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 3), line3);
						this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 4), line4);
						this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 5), line5);
					}
				}
                // push new point to points pool
                points.push_back(p_o1);
				points.push_back(p_o2);
				points.push_back(p_o3);
				points.push_back(p_o4);
            }
            #pragma endregion 
        }
};
#endif
