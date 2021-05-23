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
#include <stdio.h>
#include <stdlib.h>       
#include <string.h>      
#include <time.h> 


#ifndef _COLLECT_POINTS_
#define _COLLECT_POINTS_

#define MALLOC(t,n)    (t *) malloc( n*sizeof(t) ) 
#define MALLOC2D(t,n)  (t **) malloc(n*sizeof(t) )
//#define CHECKPTR(p)    if (!p) Error("Out of memory")

#define MAX_INT 999999999

using namespace std;

class CollectingPoints{

    public:
        CollectingPoints(){

        }

        void Error (char *str)
        { 
            printf("\nERROR: %s\n", str);
            return;
        }

        void buildAdjMatrix(int N, int  **PATHS, int **ROUTE2, int  *SHORTEST_PATH, int  *VISITED){
            int vertexNum = N;
            //mallocations dynamicly and check for errors 

            SHORTEST_PATH = MALLOC( int , vertexNum );
            

            VISITED = MALLOC( int , vertexNum );
            

            PATHS = MALLOC2D( int , vertexNum );
            

            ROUTE2 = MALLOC2D( int , vertexNum );
            
            for(int i=0; i < vertexNum; i++)
            {
                PATHS[i] =  MALLOC( int , vertexNum );

                ROUTE2[i] = MALLOC( int , vertexNum );

                //say that Y is empty
                VISITED[i] = 0; 

                //set the paths as very long 
                SHORTEST_PATH[i] = MAX_INT;

            }

        }
};
#endif