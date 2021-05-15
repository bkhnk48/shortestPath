#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
#include <stack>

#include "draw.cpp"

using namespace std;

class BuildingPolygons{
    private:
        static int WIDTH;
        static int LENGTH; 
        void getFourVertecies(int indexRow, int indexCol, vector<point> fourVertecies){
            int xCenter = indexRow*WIDTH + this->cordX0;
            int yCenter = indexCol*LENGTH + this->cordY0;
            fourVertecies[0].x = xCenter - (WIDTH/2);
            fourVertecies[0].y = yCenter + (LENGTH/2);

            fourVertecies[1].x = xCenter + (WIDTH/2);
            fourVertecies[1].y = yCenter + (LENGTH/2);

            fourVertecies[2].x = xCenter + (WIDTH/2);
            fourVertecies[2].y = yCenter - (LENGTH/2);

            fourVertecies[3].x = xCenter - (WIDTH/2);
            fourVertecies[3].y = yCenter - (LENGTH/2);
        }
    public:
        //int** checkedPoints;
        vector<point> checkedPoints;
        int Row;
        int Column;
        int cordX0;
        int cordY0;
        int width;
        int length;
        stack<point> ongoingCheckedSlots;
        void check(int x0, int y0){
            //if()
        }

        BuildingPolygons(int Row, int Column, int cordX, int cordY, int width, int length){
            this->Row = Row;
            this->Column = Column;
            this->cordX0 = cordX;
            this->cordY0 = cordY;
            WIDTH = width;
            LENGTH = length;
        }

        
       


};

class InitState{
    public:
        int r;
        int c;
        char decision = 'S';
        InitState(int r, int c){
            this->r = r;
            this->c = c;
        }

        virtual void makeDecision(int **array, int ROW, int COL, int *checkedAVs){
            if(c < COL - 1){
                if(array[r][c + 1] == 1){
                    decision = 'R';
                    return; //the direction is RIGHT
                }
                else{
                    decision = 'S';
                    return;//Still find the first non-empty cell
                }
            }
            else{//reach the boundary
                decision = 'C';
                return;//change row
            }
        }

        virtual void move(){
            if(decision == 'S')
                this->c++;
            else if(decision == 'D'){
                this->r++;
            }
            else if(decision == 'L'){
                this->r--;
            }
            else if(decision == 'C'){
                this->r++;
                this->c = 0;
            }
        }
    
};


class RightForwardingState : public virtual InitState
{

    public:
        RightForwardingState(int r, int c) : InitState{r, c}{

        }
        void makeDecision(int **array, int ROW, int COL){
            if(c < COL - 1){
                if(array[r][c + 1] == 1){
                    decision = 'S';
                    return;//Still
                }
                else{
                    decision = 'D';//Down
                    return;
                }
            }
            else{//reach right boundary
                if(r < ROW - 1){
                    if(array[r + 1][c] == 1){
                        decision = 'D';//Down
                        return;
                    }
                    else{
                        decision = 'L';//Left
                        return;//Quay lui lại
                        //tức turn left
                        //Ô trống ở dưới hàng này
                    }
                }
                else{//đang duyệt hàng cuối cùng
                    decision = 'L';//Left
                    return;//Quay lui lại
                }
            }
        }

        
};

class LeftForwardingState : public virtual InitState
{

    public:
        LeftForwardingState(int r, int c) : InitState{r, c}{

        }
        void makeDecision(int **array, int ROW, int COL){
            if(c > 0){
                if(array[r][c - 1] == 1){
                    decision = 'S';
                    return;//Still
                }
                else{
                    decision = 'D';//Down
                    return;
                }
            }
            else{//reach right boundary
                if(r < ROW - 1){
                    if(array[r + 1][c] == 1){
                        decision = 'D';//Down
                        return;
                    }
                    else{
                        decision = 'L';//Left
                        return;//Quay lui lại
                        //tức turn left
                        //Ô trống ở dưới hàng này
                    }
                }
                else{//đang duyệt hàng cuối cùng
                    decision = 'L';//Left
                    return;//Quay lui lại
                }
            }
        }
};

class Factory
{
    public:
        static InitState* getNextState(InitState* s){
            s->move();
            if(s->decision == 'S')
            {
                return s;
            }
            int r = s->r; int c = s->c;
            char option = s->decision;
            delete s;
            if(option == 'R')
            {
                RightForwardingState* newState = new RightForwardingState(r, c);
                return newState;
            }
            return NULL;
        }
};