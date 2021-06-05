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

#include "draw.cpp"

#ifndef _BUILD_PYTHON_CODE_
#define _BUILD_PYTHON_CODE_

using namespace std;

string drawIntX(point &p){
	int r = (int)p.x*10;
	return to_string(r);
}

string drawIntY(point &p){
	int r = (int)p.y*10;
	return to_string(r);
}

void writePythonCode(string file_name, vector<point> &route){

    float scale = 0.5;
    float deltaX = 10;

	string str1 = "import turtle\n";
    str1 = str1 + "import reeds_shepp as rs\n";
    str1 = str1 + "import utils\n";
    str1 = str1 + "import draw\n";
    str1 = str1 + "import math\n";
    str1 = str1 + "import random as rd\n";

    str1 = str1 + "\n\n\n";
    str1 = str1 + "def main():\n";
    str1 = str1 + "\t# points to be followed\n";
    str1 = str1 + "\n\tpts = [";

    int current = route.size()-1;
    string strPoint = "(";
    strPoint.append(to_string((int)(scale*route.at(current).x - deltaX)));
    strPoint.append(",");
    strPoint.append(to_string((int)(scale*route.at(current).y)));
    strPoint.append(")");
    strPoint.append("] ");
    //cout<<"("<<route.at(current).x<<","<<route.at(current).y<<")";
    current--;

    
    

	while(current != -1){
		
		strPoint = "), " + strPoint;			
		//str1.append(drawIntX(route.at(current)));
        strPoint = to_string((int)(0.5*route.at(current).y)) + strPoint;
        strPoint = ", " + strPoint;
		//str1.append(",");
        strPoint = to_string((int)(scale*route.at(current).x - deltaX)) + strPoint;
		//str1.append(drawIntY(route.at(current)));
        //str1.append(to_string((int)route.at(current).y));
		//str1.append(")");
        strPoint = "(" + strPoint;
		//cout<<"("<<route.at(current).x<<","<<route.at(current).y<<")";

		current--;
	}

    str1.append(strPoint);
    

    cout<<endl;

    

    str1.append("\n\t# generate PATH so the vectors are pointing at each other\n");
    str1.append("\tPATH = []\n");
    str1.append("\tfor i in range(len(pts) - 1):\n");
    str1.append("\t    dx = pts[i+1][0] - pts[i][0]\n");
    str1.append("\t    dy = pts[i+1][1] - pts[i][1]\n");
    str1.append("\t    theta = math.atan2(dy, dx)\n");
    str1.append("\t    PATH.append((pts[i][0], pts[i][1], utils.rad2deg(theta)))\n");
    str1.append("\tPATH.append((pts[-1][0], pts[-1][1], 0))\n");

    str1.append("\n\t# or you can also manually set the angles:\n");
    str1.append("\t# PATH = [(-5,5,90),(-5,5,-90),(1,4,180), (5,4,0), (6,-3,90), (4,-4,-40),(-2,0,240), \n");
    str1.append("\t#         (-6, -7, 160), (-7,-1,80)]\n");

    str1.append("\n\t# or just generate a random route:\n");
    str1.append("\t# PATH = []\n");
    str1.append("\t# for _ in range(10):\n");
    str1.append("\t#     PATH.append((rd.randint(-7,7), rd.randint(-7,7), rd.randint(0,359)))\n");

    str1.append("\n\t# init turtle\n");
    str1.append("\ttesla = turtle.Turtle()\n");
    str1.append("\ttesla.speed(0) # 0: fast; 1: slow, 8.4: cool\n");
    str1.append("\ttesla.shape('arrow')\n");
    str1.append("\ttesla.resizemode('user')\n");
    str1.append("\ttesla.shapesize(1, 1)\n");

    str1.append("\n\tscreen = turtle.Screen()\n");
	str1.append("\tscreen.screensize(2500, 1500)\n\n");

    str1.append("\n\t# draw vectors representing points in PATH\n");
    str1.append("\tfor pt in PATH:\n");
    str1.append("\t    draw.goto(tesla, pt)\n");
    str1.append("\t    draw.vec(tesla)\n");

    str1.append("\n\t# draw all routes found\n");
    str1.append("\ttesla.speed(0)\n");
    str1.append("\tfor i in range(len(PATH) - 1):\n");
    str1.append("\t    paths = rs.get_all_paths(PATH[i], PATH[i+1])\n");

    str1.append("\n\t    for path in paths:\n");
    str1.append("\t        draw.set_random_pencolor(tesla)\n");
    str1.append("\t        draw.goto(tesla, PATH[i])\n");
    str1.append("\t        draw.draw_path(tesla, path)\n");

    str1.append("\n\t# draw shortest route\n");
    str1.append("\ttesla.pencolor(1, 0, 0)\n");
    str1.append("\ttesla.pensize(3)\n");
    str1.append("\ttesla.speed(10)\n");
    str1.append("\tdraw.goto(tesla, PATH[0])\n");
    str1.append("\tpath_length = 0\n");
    str1.append("\tfor i in range(len(PATH) - 1):\n");
    str1.append("\t    path = rs.get_optimal_path(PATH[i], PATH[i+1])\n");
    str1.append("\t    path_length += rs.path_length(path)\n");
    str1.append("\t    draw.draw_path(tesla, path)\n");

    str1.append("\n\tprint(\"Shortest path length: {} px.\".format(int(draw.scale(path_length))))\n");

    str1.append("\n\tturtle.done()\n");


    str1.append("\n\nif __name__ == '__main__':\n");
    str1.append("\t    main()\n");

	std::ofstream ofs(file_name.c_str());
	if (!ofs.good())
		cout<<"Could not write data to "<<file_name;

	ofs << str1;
	ofs.close();
}
#endif