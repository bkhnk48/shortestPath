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
#include "Homotopy.cpp"

#ifndef _PLANNING_CONTROLLER_
#define _PLANNING_CONTROLLER_



using namespace std;

class PlanningController{
    public:

        PlanningController(){

        }

        
        vector<point> getTrajectory(BuildingPolygons* generator, point start, point end){
            vector<point> points = generator->points;
            vector< vector< lineSegment> > polygons = generator->polygons;
            
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
                getRawRoute(rawRoute, route, points);
                for(int i = 0; i < route.size(); i++){
                    cout<<route.at(i)<<" ";
                }
                vector<point> shortestPath = echo(rawRoute, polygons, route, points);
                Homotopy* homotopy = new Homotopy();
                vector<point> sideSteps = homotopy->sideStepRouting(shortestPath, polygons, points);
                string fileName = "test/test";
                fileName += to_string(nmrMovement);
                fileName += ".svg";
                drawShortestPath(fileName, start, end, polygons, distance, points, sideSteps, graph);
                //drawShortestPath(fileName, start, end, polygons, distance, points, shortestPath, graph);
                //drawShortestPath(fileName, start, end, polygons, distance, points, rawRoute->points, graph);
                nmrMovement++;
                return vector<point>();	
            }
            
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

        void getRawRoute(RawRoute* rawRoute, vector<int> & route,vector<point> & points){
            int current = route.size()-1;
            while(current != -1){
                point p = points[current%points.size()];
                current = route[current];
                rawRoute->insert(p);
            }
        }

        vector<point> echo(RawRoute* r, vector< vector< lineSegment> > polygons, vector<int> route, vector<point> & points){
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
                        //cout<<"("<<l.p.x<<", "<<l.p.y<<") => ("
                        //	<<l.q.x<<", "<<l.q.y<<") crosses "<<crossing<<endl;
                    }
                }
            }
            //cout<<"\nsize of group "<<group.size()<<endl;
            //for(int  i = 0; i < group.size();  i++){
            //    cout<<"Line ("<<group.at(i).p.x<<", "<<group.at(i).p.y<<")=>("<<group.at(i).q.x<<", "<<group.at(i).q.y<<")\n";
            //}
            
            double  **PATHS     = (double**)  NULL;  //edgelerin uzunluklari
            int **ROUTE2     = (int**) NULL;   //ROUTE mizi belirleriz
            double  *SHORTEST_PATH       = (double*)   NULL;  //o vertex in baslangica en kisa yolu
            double  *VISITED = (double*)   NULL;   

            CollectingPoints* collectionPoints = new CollectingPoints();
            
            vector<point> result = collectionPoints->assignValueToMatrix(PATHS, ROUTE2, SHORTEST_PATH, VISITED, route, points, group);


            return result;
        }

    private:
        int nmrMovement = 0;

        void stayAwayFromEdges(vector< vector< lineSegment> > polygons, vector<point> result){
            
        }

        //bool couldReachLine()
        
};




#endif