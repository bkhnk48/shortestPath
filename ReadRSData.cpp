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
        float param;
        vector<point> possiblePaths;
};

//A path segment is a group of continuous section to move from one point to other one.
class PathSegment : public MovementPoint{
    public:
        vector<Section> sections;
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
        vector<PathSegment> segments;
        float Lmin;
        int index;

        Path(){
            this->Lmin = FLT_MAX;
            this->index = -1;
        }
};

vector<point> getSegmentOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, char steering);

vector<point> readRSFile(string fileName){

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
                        PathSegment *segment = new PathSegment();
                        segment->beganX = x;
                        segment->beganY = y;
                        segment->endedX = nextX;
                        segment->endedY = nextY;
                        getline(infile, line);
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
                                    
                                }
                                //prepare for build sections, to be continued
                                numberSections--;
                            }
                        }

                        numOfSegmentsInPath--;
                    }
                }
            }
            numPaths--;
        }
        
        
        
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