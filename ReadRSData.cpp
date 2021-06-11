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
#include <fstream>
#include <float.h>


#include "generatePy.cpp"

#ifndef _READ_REEDS_SHEEP_
#define _READ_REEDS_SHEEP_

using namespace std;

//Each instance of movement point is the point which AV should reach before a time
class MovementPoint{
    public:
        double beganX;
        double beganY;
        double endedX;
        double endedY;
};

//A section is a small trajectory which routes AV to move (both forward and backward) from
//a point to other one. 
class Section : public MovementPoint{
    public:
        char typeOfTrajectory ;
        char steering;
        double param;
        vector<point> possiblePoints;
};

//A path segment is a group of continuous section to move from one point to other one.
class PathSegment : public MovementPoint{
    public:
        vector<Section*> sections;
        float L;
        //int index;

        PathSegment(){
            //this->index = -1;
            this->L = 0;
        }
};

//A path is a group of possible path segment and its best choice to move from
//one point to other one
class Path : public MovementPoint{
    public:
        vector<PathSegment*> segments;
        float Lmin;
        int index;

        Path(){
            this->Lmin = FLT_MAX;
            this->index = -1;
        }
};

class Range{
    public:
        double xMax;
        double xMin;
        double yMax;
        double yMin;

        Range(){
            this->xMax = FLT_MIN;
            this->yMax = FLT_MIN;
            this->xMin = FLT_MAX;
            this->yMin = FLT_MAX;
        }
};

vector<point> getSegmentOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, char steering);


//p1, p2: diem dau diem cuoi cua chuyen dong tron
//rotatedAngle: goc quay cua quy dao hinh tron (co the am hoac duong)
//steering: quay theo duong tron ben trai hay duong tron ben phai cua xe
int getPointsOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, char steering, 
                            vector<point> &points, vector<vector<lineSegment>> &polygons, vector<Range*> ranges)
{
    if(rotatedAngle == 0)
        return -1;

    double midPointX = (p1X + p2X)/2;
    double midPointY = (p1Y + p2Y)/2;
    double x = p2X - p1X;
    double y = p2Y - p1Y;

    double R = 1;

    int n = (int)(rotatedAngle/SMALL_ANGLE);

    if(n <= 1){
        return -1;//shortcut to prevent from wasting floating-point calculation
    }

    double xIn = 0, yIn = 0, xOut = 0, yOut = 0;
    getNormalInAndOut(p2X - p1X, p2Y - p1Y, &xIn, &yIn, &xOut, &yOut);
    double xNormal, yNormal;
    if(LEFT == steering)
    {
        xNormal = xOut;
        yNormal = yOut;
    }
    else{
        xNormal = xIn;
        yNormal = yIn;
    }

    double h = cos(rotatedAngle/2);
    double centerX = midPointX + h*xNormal;
    double centerY = midPointY + h*yNormal;

    #pragma region Comments for explaination of the below code    
    //sin(omega_T0) = (p1.y - centerY)/R;
    //cos(omega_T0) = (p1.x - centerX)/R;
    //sin(deltaOmega) = ...;
    //cos(deltaOmega) = ...;
    //for i = 1..n
    //  sin(omega_T) = sin(omega_T0)*cos(deltaOmega) + cos(omega_0)*sin(deltaOmega)
    //  cos(omega_T) = cos(omega_T0)*cos(deltaOmega) - sin(omega_0)*sin(deltaOmega)
    //  x(t) = centerX + R*cos(omega)
    //  y(t) = centerY + R*sin(omega)
    #pragma endregion
    double sin_omega_T0 = (p1Y - centerY)/R;
    double cos_omega_T0 = (p1X - centerX)/R;
    double sin_omega, cos_omega, x_t, y_t;
    double sin_omega_T = sin_omega_T0, cos_omega_T = cos_omega_T0;
    double sin_deltaOmega = SMALL_ANGLE;
    double cos_deltaOmega = 1 - (SMALL_ANGLE*SMALL_ANGLE/2);
    double xT, yT;
    for(int i = 1; i < n; i++){
        sin_omega_T = sin_omega_T*cos_deltaOmega + cos_omega_T*sin_deltaOmega;
        cos_omega_T = cos_omega_T*cos_deltaOmega - sin_omega_T*sin_deltaOmega;
        xT = centerX + R*cos_omega_T;
        yT = centerY + R*sin_omega_T;
        for(int j = 0; j < polygons.size(); j++){
            if(xT >= ranges.at(j)->xMin && xT <= ranges.at(j)->xMax &&
                yT >= ranges.at(j)->yMin && yT <= ranges.at(j)->yMax)
            {
                int check = pnpoly(polygons.at(j), xT, yT, true);
                if(check % 2 == 1){
                    return -1;//collide with one of the polygons
                }
                check = pnpoly(polygons.at(j), xT, yT, false);
                if(check % 2 == 1)
                        return -1;//collide with one of the polygons
            }
        }
        point p(xT, yT);
        points.push_back(p);
    }
    return 1;
}

PathSegment* readSegment(double x, double y, double nextX, double nextY, ifstream& infile, 
                            vector<vector<lineSegment>> &scaledPolygons, vector<Range*> ranges, int *error){
    PathSegment *segment = new PathSegment();
    segment->beganX = x;
    segment->beganY = y;
    segment->endedX = nextX;
    segment->endedY = nextY;
    string line;
    string strTemp1;
    getline(infile, line);
    *error = 1;
    istringstream allSections(line);
    int numberSections = 0;
    if(allSections >> strTemp1 >> numberSections){
        double startX = x; 
        double startY = y;
        while(numberSections > 0){
            Section *section = new Section();
            getline(infile, line);
            istringstream sectionInfo(line);
            if(sectionInfo >> strTemp1 >> section->endedX 
                    >> section->endedY >> section->param
                    >> section->typeOfTrajectory >> section->steering){
                int check = getPointsOfCircle(startX, startY, 
                                section->endedX, section->endedY, 
                                section->param, section->steering, 
                                section->possiblePoints, scaledPolygons, ranges);
                if(check == 1){
                    segment->sections.push_back(section);
                    *error = 0;
                }
                else{
                    *error = 1;
                    return segment;
                }
            }
            //prepare for build sections, to be continued
            numberSections--;
        }
    }
    return segment;
}

vector<Path*> readPath(ifstream& infile, int numPaths, vector<vector<lineSegment>> &polygons){
    string line;
    string strTemp, strTemp1, strTemp2, strTemp3;

    vector<vector<lineSegment>> scaledPolygons;
    vector<Range*> ranges;
    scaledPolygons.resize(polygons.size());
    for(int i = 0; i < polygons.size(); i++){
        vector<lineSegment> lines;
        Range *rangeOfAPolygon = new Range();
        for(int j = 0; j < polygons.at(i).size(); j++){
            
            point p(polygons.at(i).at(j).p.x/RATIO, polygons.at(i).at(j).p.y/RATIO);
            point q(polygons.at(i).at(j).q.x/RATIO, polygons.at(i).at(j).q.y/RATIO);

            if(rangeOfAPolygon->xMax < p.x)
                rangeOfAPolygon->xMax = p.x;
            if(rangeOfAPolygon->xMin > p.x)
                rangeOfAPolygon->xMin = p.x;

            if(rangeOfAPolygon->yMax < p.y)
                rangeOfAPolygon->xMax = p.y;
            if(rangeOfAPolygon->xMin > p.y)
                rangeOfAPolygon->xMin = p.y;

            lineSegment aLine(p, q);
            lines.push_back(aLine);
        }
        ranges.push_back(rangeOfAPolygon);
        scaledPolygons.push_back(lines);
    }
    
    double x, y, nextX, nextY, distance;
    int i;
    char typeOfTraj;
    vector<Path*> result;
    while(numPaths > 0){
        Path *path = new Path();
        getline(infile, line);
        istringstream segment(line);
        if(segment >> strTemp1 >> i >> strTemp2 >> x >> y >> strTemp3 >> nextX >> nextY)
        {
            path->beganX = x;
            path->beganY = y;
            path->endedX = nextX;
            path->endedY = nextY;
            int numOfSegmentsInPath = 0;
            getline(infile, line);
            istringstream possiblePathSegment(line);
            if(possiblePathSegment >> strTemp1 >> numOfSegmentsInPath){
                //path->segments.resize(numOfPathsInThisSegment);
                while(numOfSegmentsInPath > 0){
                    int error = 0;
                    PathSegment* segment = readSegment(x, y, nextX, nextY, infile, scaledPolygons, ranges, &error);
                    if(error == 0)
                        path->segments.push_back(segment);
                    numOfSegmentsInPath--;
                }

                
            }
        }
        numPaths--;
        result.push_back(path);
    }

    return result;
}

vector<point> readRSFile(string fileName, vector<vector<lineSegment>> &polygons){

    vector<point> discreteTrajectory;
     
    ifstream infile(fileName);
    string line;
    char steering;
    string strTemp, strTemp1, strTemp2, strTemp3;
    int numPaths;
    double x, y, nextX, nextY, distance;
    int i;
    char typeOfTraj;
    getline(infile, line);
    istringstream firstIss(line);
    if (firstIss >> strTemp >> numPaths) 
    { 
        cout<<"number of paths: "<<numPaths<<endl;
        
        readPath(infile, numPaths, polygons);
        
        
        double tempPointX = x;
        double tempPointY = y;
        double p2X; double p2Y;

        while (getline(infile, line))
        {
            istringstream iss(line);
            
            
            if (!(iss >> p2X >> p2Y >> distance >> typeOfTraj >> steering)) 
            { 
                break; 
            } // error
            //cout<<"X = "<<x<<" Y = "<<y<<" dis = "<<distance<<" "<<typeOfTraj<<" "<<steering<<endl;
            
            if(CIRCLE == typeOfTraj && distance != 0){
                double rotatedAngle = distance;
                vector<point> tempVector = getSegmentOfCircle(tempPointX, tempPointY, p2X, p2Y, rotatedAngle, steering);
                if(tempVector.size() > 0){
                    discreteTrajectory.insert(discreteTrajectory.end(), tempVector.begin(), tempVector.end());
                }
            }

            tempPointX = p2X;
            tempPointY = p2Y;
            point p2(p2X, p2Y);
            discreteTrajectory.push_back(p2);
            
        }
    }
    cout<<"Close file"<<endl;
    infile.close();

    return discreteTrajectory;
}


//p1, p2: diem dau diem cuoi cua chuyen dong tron
//rotatedAngle: goc quay cua quy dao hinh tron (co the am hoac duong)
//steering: quay theo duong tron ben trai hay duong tron ben phai cua xe
vector<point> getSegmentOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, char steering)
{
    vector<point> segment;
    if(rotatedAngle == 0)
        return segment;

    double midPointX = (p1X + p2X)/2;
    double midPointY = (p1Y + p2Y)/2;
    double x = p2X - p1X;
    double y = p2Y - p1Y;
    double distance = sqrt(x*x + y*y)/2;

    double R = distance/sin(abs(rotatedAngle)/2);
    cout<<"R = "<<R<<"as angle = "<<rotatedAngle<<" dis "<<distance<<endl;

    //angle velocity
    double omegaVelocity = MAX_VELOCITY/R;
    double t = rotatedAngle/omegaVelocity;
    int n = (int)abs(t);

    if(n <= 1){
        return segment;//shortcut to prevent wasted floating-point calculation
    }

    double xIn = 0, yIn = 0, xOut = 0, yOut = 0;
    getNormalInAndOut(p2X - p1X, p2Y - p1Y, &xIn, &yIn, &xOut, &yOut);
    double xNormal, yNormal;
    if(LEFT == steering)
    {
        xNormal = xOut;
        yNormal = yOut;
    }
    else{
        xNormal = xIn;
        yNormal = yIn;
    }

    double h = distance/tan(abs(rotatedAngle)/2);
    double centerX = midPointX + h*xNormal;
    double centerY = midPointY + h*yNormal;
    
    //sin(omega_T0) = (p1.y - centerY)/R;
    //cos(omega_T0) = (p1.x - centerX)/R;
    //sin(deltaOmega) = ...;
    //cos(deltaOmega) = ...;
    //for i = 1..n
    //  sin(omega_T) = sin(omega_T0)*cos(deltaOmega) + cos(omega_0)*sin(deltaOmega)
    //  cos(omega_T) = cos(omega_T0)*cos(deltaOmega) - sin(omega_0)*sin(deltaOmega)
    //  x(t) = centerX + R*cos(omega)
    //  y(t) = centerY + R*sin(omega)
    double sin_omega_T0 = (p1Y - centerY)/R;
    double cos_omega_T0 = (p1X - centerX)/R;
    double sin_omega, cos_omega, x_t, y_t;
    double sin_omega_T = sin_omega_T0, cos_omega_T = cos_omega_T0;
    double sin_deltaOmega = sin(rotatedAngle/n);
    double cos_deltaOmega = cos(rotatedAngle/n);
    double xT, yT;
    for(int i = 1; i < n; i++){
        sin_omega_T = sin_omega_T*cos_deltaOmega + cos_omega_T*sin_deltaOmega;
        cos_omega_T = cos_omega_T*cos_deltaOmega - sin_omega_T*sin_deltaOmega;
        xT = centerX + R*cos_omega_T;
        yT = centerY + R*sin_omega_T;
        point p(xT, yT);
        segment.push_back(p);
    }
    return segment;
}
#endif