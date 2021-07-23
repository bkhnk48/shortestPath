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
#include <cstdlib>

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

        /**
         * Check point inside lineSegment?
         */
        bool isPointInsideLineSegment(lineSegment line, point p){
        	point p_1 = line.p;
        	point p_2 = line.q;

        	double dis_1 = sqrt((p_1.x - p.x) * (p_1.x - p.x) + (p_1.y - p.y) * (p_1.y - p.y));
        	double dis_2 = sqrt((p_2.x - p.x) * (p_2.x - p.x) + (p_2.y - p.y) * (p_2.y - p.y));
        	double dis = sqrt((p_1.x - p_2.x) * (p_1.x - p_2.x) + (p_1.y - p_2.y) * (p_1.y - p_2.y));

        	if(abs(dis_1 + dis_2 - dis) < 0.0001){ // AB + BC ~ AC => A,B,C straight
        		return true;
        	}

        	return false;
        }

        /**
         * Check point inside polygon?
         * Does 1 point lie on the contour of the polygon?
         */
        int isPointInsidePolygon(vector< vector<lineSegment> > polygons ,int polygonIndex, point p){
        	for(int i=0; i<polygons.at(polygonIndex).size(); i++){
        		lineSegment line = polygons.at(polygonIndex).at(i);
        		if(isPointInsideLineSegment(line, p)){
        			return i;
        		}
        	}

        	return -1;
        }

        /**
         * Get line include point, isPointInsideLineSegment check point have inside line segment and
         * The method return line segment include this point
         */
        lineSegment getLineSegmentIncludePoint(int polygonIndex, point p){
        	for(int i=0; i<polygons.at(polygonIndex).size(); i++){
				lineSegment line = polygons.at(polygonIndex).at(i);
				if(isPointInsideLineSegment(line, p)){
					return line;
				}
			}

        	return polygons.at(polygonIndex).at(0);
        }

        void removeLineSegment(point p, int polygonIndex){
            //cout <<"Remove all line segments which have p("<<p.x<<", "<<p.y<<")"<<endl;
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

        //return true iff p1 is between p0 and p2
        bool isTheMiddle(point p0, point p1, point p2){
            //vector u is a vector from p0 to p1
            double uX = p1.x - p0.x;
            double uY = p1.y - p0.y;

            //vector v is a vector from p1 to p2
            double vX = p2.x - p1.x;
            double vY = p2.y - p1.y;

            double product = uX*vY - uY*vX;
            double dotProduct = uX*vX + uY*vY;

            if(product == 0 && dotProduct > 0){
                return true;
            }
            return false;
        }

        int countSharedVertices(int indexOfStack, int row, int column, int *first, int *last, int *polygonIndex){
            this->getFourVertecies(indexOfStack, row, column);
            bool found = false;
            
            int count = 0;
            sharedVertices[0] = false;
            sharedVertices[1] = false;
            sharedVertices[2] = false;
            sharedVertices[3] = false;

            int indexOfZeroCase[4] = {-1, -1, -1, -1};
            
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
                    else{
                        if(indexOfZeroCase[0] == -1){
                            bool AisOnEdge = isTheMiddle(this->polygons.at(i).at(j).p, pA, this->polygons.at(i).at(j).q);
                            if(AisOnEdge){
                                indexOfZeroCase[0] = i;
                            }
                        }
                        if(indexOfZeroCase[1] == -1){
                            bool BisOnEdge = isTheMiddle(this->polygons.at(i).at(j).p, pB, this->polygons.at(i).at(j).q);
                            if(BisOnEdge){
                                indexOfZeroCase[1] = i;
                            }
                        }
                        if(indexOfZeroCase[2] == -1){
                            bool CisOnEdge = isTheMiddle(this->polygons.at(i).at(j).p, pC, this->polygons.at(i).at(j).q);
                            if(CisOnEdge){
                                indexOfZeroCase[2] = i;
                            }
                        }
                        if(indexOfZeroCase[3] == -1){
                            bool DisOnEdge = isTheMiddle(this->polygons.at(i).at(j).p, pD, this->polygons.at(i).at(j).q);
                            if(DisOnEdge){
                                indexOfZeroCase[3] = i;
                            }
                        }

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
                else{
                    if(indexOfZeroCase[0] == indexOfZeroCase[1] && indexOfZeroCase[1] == indexOfZeroCase[2]
                        && indexOfZeroCase[2] == indexOfZeroCase[3] && indexOfZeroCase[0] != -1
                    ){
                        *polygonIndex = indexOfZeroCase[0];
                        found = true;
                        break;
                    }
                }
            }
            
            return count;
        }

        /**
         * Calculate Euclid distance between two point
         * Improve : initialization class/interface distance and euclid_distance implement/extends distance class/interface
         */
        double euclid_distance(point a, point b){
            return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        }

        /**
         * Swap 2 point
         * Improve: replace by template
         */
        void swap_point(point &point1, point &point2){
            point tmp_point = point1;
            point1 = point2;
            point2 = tmp_point;

            return;
        }

        /**
         * Swap 2 double parameter
         * Improve: replace by template
         */
        void swap_double(double &dis_1, double &dis_2){
            double tmp_dis = dis_1;
            dis_1 = dis_2;
            dis_2 = tmp_dis;

            return;
        }

        /**
         * s_point is the first break found clockwise, and e_point is end point, them belong to lineSegment deleted
         * The method will sort p_o1 to p_o4, so that p_o1 to p_o4 order nearest and farthest compare to s_point follow clockwise
         * Improve: union with getVerticesAV
         */
        void sort_clockwise(point &s_point, point &e_point, point &p_o1, point &p_o2, point &p_o3, point &p_o4, int cnt){
        	vector<point> listVertices; // listVertices is list of point is vertices of polygon before considering this case
			if(cnt == 3){
				listVertices.push_back(p_o4);
			} else if(cnt == 2){
				listVertices.push_back(p_o4);
				listVertices.push_back(p_o3);
			} else if(cnt == 1){
				listVertices.push_back(p_o4);
				listVertices.push_back(p_o3);
				listVertices.push_back(p_o2);
			} else if(cnt == 0){
				listVertices.push_back(p_o4);
				listVertices.push_back(p_o3);
				listVertices.push_back(p_o2);
				listVertices.push_back(p_o1);
			}

			// sort 4 point p_o1->p_o4 such as: p_o1 nearest e_point and p_o4 nearest e_point
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
				swap_point(p_o4, p_o3);
				swap_double(dis_4, dis_3);
			} // after step p_o4 is nearest end point

			// next step, we must sort p_o2, p_o3 such as: p_o2 near p_o1 than p_o3 and p_o3 near p_o4 than p_o2
			dis_2 = euclid_distance(p_o1, p_o2);
			dis_3 = euclid_distance(p_o1, p_o3);

			if(dis_2 > dis_3){
				swap_point(p_o2, p_o3);
				swap_double(dis_2, dis_3);
			}

			// p_o1 -> p_o4 sorted, but we need take point in "points" and push back
			deque<point> queue;

			if(find(listVertices.begin(), listVertices.end(), p_o4) != listVertices.end()){
				queue.push_back(p_o4);
			} else {
				queue.push_front(p_o4);
			};

			if(find(listVertices.begin(), listVertices.end(), p_o3) != listVertices.end()){
				queue.push_back(p_o3);
			} else {
				queue.push_front(p_o3);
			};

			if(find(listVertices.begin(), listVertices.end(), p_o2) != listVertices.end()){
				queue.push_back(p_o2);
			} else {
				queue.push_front(p_o2);
			};

			if(find(listVertices.begin(), listVertices.end(), p_o1) != listVertices.end()){
				queue.push_back(p_o1);
			} else {
				queue.push_front(p_o1);
			};

			p_o1 = queue.at(0);
			p_o2 = queue.at(1);
			p_o3 = queue.at(2);
			p_o4 = queue.at(3);
        }

        /**
         * When delete lineSegment, polygon (in pylygons with index polygonIndex) was broken
         * Method find 2 point break in polygon
         */
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
        
        /**
         * Get 4 vertices of AV assign to p_o1, p_o2, p_o3, p_o4
         * If vertices in points push back to p_o4
         * If vertices not in points push head to p_o1
         */
        void getVerticesAV(point &p_o1, point &p_o2, point &p_o3, point &p_o4){
            deque<point> queue;
            
            if(find(points.begin(), points.end(), this->pA) != points.end()){
                queue.push_back(this->pA);
            } else {
                queue.push_front(this->pA);
            };

            if(find(points.begin(), points.end(), this->pB) != points.end()){
                queue.push_back(this->pB);
            } else {
                queue.push_front(this->pB);
            };

            if(find(points.begin(), points.end(), this->pC) != points.end()){
                queue.push_back(this->pC);
            } else {
                queue.push_front(this->pC);
            };

            if(find(points.begin(), points.end(), this->pD) != points.end()){
                queue.push_back(this->pD);
            } else {
                queue.push_front(this->pD);
            };

            p_o1 = queue.at(0);
            p_o2 = queue.at(1);
            p_o3 = queue.at(2);
            p_o4 = queue.at(3);
        }

        /**
         * Split them to two polygon
         * When after remove point and lineSegment, if polygon create to by two polygon
         */
        void splitPolygon(int polygonIndex){
        	vector<lineSegment> subPolygon_1;
        	vector<lineSegment> subPolygon_2;

        	lineSegment lastLineSegment = polygons.at(polygonIndex).at(0);
        	subPolygon_1.push_back(lastLineSegment); // add first element to sub polygon
        	for(int i=1; i<polygons.at(polygonIndex).size(); i++){ // check each element line segment
        		lineSegment line = polygons.at(polygonIndex).at(i);
        		if(lastLineSegment.q == line.p){ // add to sub polygon 1
        			subPolygon_1.push_back(line);
        			lastLineSegment = line;
        		}
        	}

        	// for each line again, if line not inside sub polygon 1, we add it to sub polygon 2
        	for(int i=0; i<polygons.at(polygonIndex).size(); i++){
        		lineSegment line = polygons.at(polygonIndex).at(i);
        		if(find(subPolygon_1.begin(), subPolygon_1.end(), line) == subPolygon_1.end()){
        			subPolygon_2.push_back(line);
        		}
        	}

        	polygons.erase(polygons.begin() + polygonIndex);
        	polygons.push_back(subPolygon_1);
        	polygons.push_back(subPolygon_2);
        }

        /**
         * Remove Edges and Vertices when AV move
         */
        void removeEdgesAndVertices(int indexOfStack, int row, int column) override{
            int first = -1, last = -1, polygonIndex = -1;
            point p_o1, p_o2, p_o3, p_o4; // initialization 4 vertices of AV
            int nmrSameVertices = this->countSharedVertices(indexOfStack, row, column, &first, &last, &polygonIndex);
//            int nmrSameVertices = 2;
//			polygonIndex = 0;
//			pA = point(8,4);
//			pB = point(10,4);
//			pC = point(8,1);
//			pD = point(10,1);
            
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


                } else {
                	// delete polygon when it no contains line segment
                	// case only when nmrSameVertices == 4
                	this->polygons.erase(this->polygons.begin()+polygonIndex);
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
                    sort_clockwise(s_point, e_point, p_o1, p_o2, p_o3, p_o4, 1);

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
                    sort_clockwise(s_point, e_point, p_o1, p_o2, p_o3, p_o4, 3);

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
            	getVerticesAV(p_o1, p_o2, p_o3, p_o4);

            	// find lineSegment contain point of AV, only find one
            	int breakIndex = isPointInsidePolygon(polygons, polygonIndex, this->pA); // find index line segment include point pA
            	int check_A = breakIndex;
            	int check_B = isPointInsidePolygon(polygons, polygonIndex, this->pB);
            	int check_C = isPointInsidePolygon(polygons, polygonIndex, this->pC);
            	int check_D = isPointInsidePolygon(polygons, polygonIndex, this->pD);

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
					line.q = p_common;
					line.p.x = x1;
					line.p.y = y1;

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
					sort_clockwise(s_point, e_point, p_o1, p_o2, p_o3, p_o4, 4);

					if(foundBreak){
						// This is a special case when the cars are on the same line, after the middle car is removed,
						// it will split the original polygons into 2 polygons.
						int con = (check_A != -1) && (check_B != -1) && (check_C != -1) && (check_D != -1);
						if(con){
							lineSegment line = getLineSegmentIncludePoint(polygonIndex, p_o2);
							point s_point1 = line.p;
							point e_point1 = line.q;

							for(int i=0; i<polygons.at(polygonIndex).size(); i++){
								lineSegment tmpLine = polygons.at(polygonIndex).at(i);
								if(tmpLine.p == s_point1 && tmpLine.q == e_point1){
									polygons.at(polygonIndex).at(i).q = p_o3;

									lineSegment line1;
									line1.p = p_o2;
									line1.q = e_point1;

									this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (i + 1), line1);
									break;
								}
							}

							lineSegment line1;
							lineSegment line2;
							lineSegment line3;
							lineSegment line4;

							line1.p = s_point;
							line1.q = p_o1;

							line2.p = p_o1;
							line2.q = p_o2;

							line3.p = p_o3;
							line3.q = p_o4;

							line4.p = p_o4;
							line4.q = e_point;

							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line1);
							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 3), line2);
							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 4), line3);
							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 5), line4);
							splitPolygon(polygonIndex); // when remove lineSegment and point, we need split polygon to two polygon
						} else { // not special case
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
							line5.q = e_point;

							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 1), line1);
							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 2), line2);
							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 3), line3);
							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 4), line4);
							this->polygons.at(polygonIndex).insert(this->polygons.at(polygonIndex).begin() + (breakIndex + 5), line5);
						}

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
