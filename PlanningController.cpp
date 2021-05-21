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

        void echo(RawRoute* r){
            for(int i = 0; i < r->points.size(); i++){
                cout<<"("<<r->points.at(i).x<<", "<<r->points.at(i).y<<") ";
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
                echo(rawRoute);
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

        
};

#endif