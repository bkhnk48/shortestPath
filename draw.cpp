double max_y,max_x,min_y,min_x;

void setMinMax(double x, double y){
	if(max_y<y){
		max_y=y;
	}
	if(max_x<x){
		max_x=x;
	}
	if(min_y>y){
		min_y=y;
	}
	if(min_x>x){
		min_x=x;
	}
}

double drawX(point &p){
	return p.x*10;
}

double drawY(point &p){
	return p.y*-10;
}

void drawRoute(vector<int> & route,vector<point> & points){
	cout << "<polyline stroke='red' stroke-width='0.2' fill='none' points='";
	int current = route.size()-1;
	while(current!=-1){
		point p =  points[current%points.size()];
		cout << drawX(p) << "," << drawY(p) << " ";
		current = route[current];
	}
	cout << "'/>" << endl;

}

void drawPoint(point &p,string color){
	cout << "<circle cx='" << drawX(p) << "' cy='"<< drawY(p) <<"' r='0.5' fill='"<< color << "' />" << endl;
}

void drawPolygon(vector<lineSegment> & polygon){
	cout << "<polygon stroke='black' stroke-width='0.1' fill='#D3D3D3'  points='";
	cout << drawX(polygon[0].p) << "," << drawY(polygon[0].p) << " ";
	cout << drawX(polygon[0].q) << "," << drawY(polygon[0].q) << " ";
	for(int i=1;i<polygon.size();i++){
		cout << drawX(polygon[i].q) << "," << drawY(polygon[i].q) << " ";
	}
	cout << "'/>" << endl;
}

void drawPolygons(vector <vector <lineSegment> > & polygons){
	for(int i=0;i<polygons.size();i++){
		drawPolygon(polygons[i]);
	}
}
void drawTitle(string testTitle, double distance){
	cout << "<text x='0' y='"<< -10*max_y-5 << "' font-family='Verdana' font-size='5'>"<< testTitle<< ", length: " << distance << "</text>" << endl;
}

void drawGraph(vector< vector< int> > &graph, vector<point>& points){
	size_t plane_start = ((points.size()*config.printLevel)/points.size())*points.size();
	size_t plane_end   = plane_start+points.size();
	for(size_t i=plane_start;i<plane_end;i++){
		for(size_t j=0;j<graph[i].size();j++){
			point from=points[i%points.size()], to=points[graph[i][j]%points.size()];
		cout << "<line x1='"<< drawX(from) << "' y1='"<< drawY(from)<<"' x2='"<<drawX(to)<<"' y2='"<<drawY(to)<<"' stroke-width='0.1' stroke='blue'/>" << endl;
		}
	}
}

void draw(string & testTitle, point & start, point & end, vector <vector < lineSegment > > & polygons,double & distance,vector<point> &points, vector<int> &route,vector< vector<int> >graph){
	cout << "<?xml version='1.0' encoding='UTF-8' ?>" << endl;
	cout << "<svg viewBox='"<< 
		10*min_x-5 << " " <<
		-10*max_y-15 << " " <<
		(abs(min_x)+abs(max_x))*10+10 << " " <<
		(abs(min_y)+abs(max_y))*10+20 <<"' xmlns='http://www.w3.org/2000/svg' version='1.1'>" << endl;

	drawPolygons(polygons);

	drawPoint(start,"#FFA500");
	drawPoint(end,"green");

	drawTitle(testTitle,distance);
	drawGraph(graph,points);
	if(distance!=-1 && config.drawRoute) drawRoute(route,points);
	cout << "</svg>" << endl;
}
