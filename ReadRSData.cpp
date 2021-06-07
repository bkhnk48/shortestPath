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


#include "generatePy.cpp"

#ifndef _READ_REEDS_SHEEP_
#define _READ_REEDS_SHEEP_

using namespace std;

vector<point> getSegmentOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, string steering);

vector<point> readRSFile(string fileName){

    vector<point> discreteTrajectory;
     
    ifstream infile(fileName);
    string line;
    string steering;
    double x, y, distance;
    string typeOfTraj;
    getline(infile, line);
    istringstream firstIss(line);
    if (firstIss >> x >> y >> typeOfTraj) 
    { 
        cout<<"X = "<<x<<" Y = "<<y<<" "<<typeOfTraj<<endl;
        point p0(x, y);
        discreteTrajectory.push_back(p0);
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
            
            if(CIRCLE.compare(typeOfTraj)){
                double rotatedAngle = distance;
                vector<point> tempVector = getSegmentOfCircle(tempPointX, tempPointY, p2X, p2Y, rotatedAngle, steering);
                if(tempVector.size() > 0){
                    discreteTrajectory.insert(discreteTrajectory.end(), tempVector.begin(), tempVector.end());
                }
            }

            tempPointX = p2X;
            tempPointY = p2Y;
            // process pair (a,b)
        }
    }
    cout<<"Close file"<<endl;
    infile.close();

    return discreteTrajectory;
}

//p1, p2: diem dau diem cuoi cua chuyen dong tron
//rotatedAngle: goc quay cua quy dao hinh tron (co the am hoac duong)
//steering: quay theo duong tron ben trai hay duong tron ben phai cua xe
vector<point> getSegmentOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, string steering)
{
    vector<point> segment;
    if(rotatedAngle == 0)
        return segment;

    double midPointX = (p1X + p2X)/2;
    double midPointY = (p1Y + p2Y)/2;
    double x = p2X - p1X;
    double y = p2Y - p1Y;
    double distance = sqrt(x*x + y*y)/2;

    double xIn = 0, yIn = 0, xOut = 0, yOut = 0;
    getNormalInAndOut(p2X - p1X, p2Y - p1Y, &xIn, &yIn, &xOut, &yOut);
    double xNormal, yNormal;
    if(LEFT.compare(steering))
    {
        xNormal = xOut;
        yNormal = yOut;
    }
    else{
        xNormal = xIn;
        yNormal = yIn;
    }

    double R = distance/sin(abs(rotatedAngle)/2);
    double h = distance/tan(abs(rotatedAngle)/2);
    double centerX = midPointX + h*xNormal;
    double centerY = midPointY + h*yNormal;

    //angle velocity
    double omegaVelocity = MAX_VELOCITY/R;
    double t = rotatedAngle/omegaVelocity;
    int n = (int)abs(t);
    
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