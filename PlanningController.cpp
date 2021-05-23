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

#include "preprocess.cpp"


#ifndef _PLANNING_CONTROLLER_
#define _PLANNING_CONTROLLER_

using namespace std;

class PlanningController{
    public:

        PlanningController(){

        }

        void echo(RawRoute* r, vector< vector< lineSegment> > polygons){
            vector<lineSegment> group;
            for(int i = 0; i < r->points.size(); i++){
                //cout<<"("<<r->points.at(i).x<<", "<<r->points.at(i).y<<") ";
                point p1 = r->points.at(i);

                for(int j = i + 2; j < r->points.size(); j++){
                    point p2 = r->points.at(j);
                    
                    lineSegment l;
                    l.p = p1;
                    l.q = p2;
                    
                    int crossing = numberOfCuttingThrough(polygons,l);
                    if(crossing == 0){
                        if(!insidePolygon(l, polygons)){
                            group.push_back(l);
                        }
                    }
                    else{
                        cout<<"("<<l.p.x<<", "<<l.p.y<<") => ("
                            <<l.q.x<<", "<<l.q.y<<") crosses "<<crossing<<endl;
                    }
                }
            }
            cout<<"size of group "<<group.size()<<endl;
            for(int  i = 0; i < group.size();  i++){
                cout<<"Line ("<<group.at(i).p.x<<", "<<group.at(i).p.y<<"=>("<<group.at(i).q.x<<", "<<group.at(i).q.y<<")\n";
            }
        }

        vector<point> getTrajectory(vector<point> points, vector< vector< lineSegment> > polygons, point start, point end){
            vector< vector < int > > graph;
            vector< vector < double > > graphDistance;

            points.insert(points.begin(), start);
            points.push_back(end);

            //Vector so we can backtrack the route
            vector<int> route;

            vector< vector < int > > crossesNumber;


            //Get how many points we have
            size_t numberOfPoints = points.size();

            //Create a two dimenstional vector for the graph
            
            size_t dimension = numberOfPoints*(config.k+1);
            graph.resize(dimension,vector<int>());
            graphDistance.resize(dimension,vector<double>());

            auto time1 = std::chrono::steady_clock::now();

            //Call function that calculate the distance
            calculateNumberOfCrossings(crossesNumber, polygons, points);

            auto time2 = std::chrono::steady_clock::now();

            makeVisabilityGraph(graph, graphDistance, crossesNumber, points);

            auto time3 = std::chrono::steady_clock::now();


            //The graph is constructed call dijksta to calculate the distance
            double distance = dijkstra(graphDistance,graph,route);

            auto time4 = std::chrono::steady_clock::now();

            //std:string title = "Automated valet parking v = 24km/h";
            //Output the distance
            RawRoute* rawRoute = new RawRoute();
            //if(config.printGraph)
            {
                draw("test/test.svg", rawRoute, 
                    start,end, polygons,distance,points,route,graph);
                echo(rawRoute, polygons);
                return vector<point>();	
            }
            /*else{
                std::cout<<points.size()<<std::endl;
                for(int i = 0; i < points.size(); i++)
                {
                    std::cout<<"("<<points.at(i).x<<", "<<points.at(i).y<<") ";
                }
                cout<<"\n";
                //cout << distance << endl;
                cout << getTime(time1,time2) << " " << getTime(time2,time3) << " " << getTime(time3,time4) << " " << distance << endl;
                //cout << "Dijkstra took : " << getTime(visibility_end,dijkstra_end) << endl;
                return points;
            }*/
        }

        point getMaxOfXY(point p1, point p2){
            double maxX = max(p1.x, p2.x);
            double maxY = max(p1.y, p2.y);

            point p;
            p.x = maxX; p.y = maxY;
            return p;
        }

        point getMinOfXY(point p1, point p2){
            double minX = min(p1.x, p2.x);
            double minY = min(p1.y, p2.y);

            point p;
            p.x = minX; p.y = minY;
            return p;
        }
};

#endif