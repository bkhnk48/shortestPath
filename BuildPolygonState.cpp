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
        int WIDTH;
        int LENGTH; 
        point pA, pB, pC, pD;
        void getFourVertecies(int indexRow, int indexCol){
            int xCenter = indexCol*WIDTH + this->cordX0;
            int yCenter = -indexRow*LENGTH + this->cordY0;
            
            pA.x = xCenter - (WIDTH/2);
            pA.y = yCenter + (LENGTH/2);

            pB.x = xCenter + (WIDTH/2);
            pB.y = yCenter + (LENGTH/2);

            pC.x = xCenter + (WIDTH/2);
            pC.y = yCenter - (LENGTH/2);

            pD.x = xCenter - (WIDTH/2);
            pD.y = yCenter - (LENGTH/2);
        }

        void getNeighborByIndex(int** arrayOfAVs, int index, int r, int c){
            int x = r; int y = c;
            //
            switch(index){
                case 0: x--; y--; break;
                case 1: x--; break;
                case 2: x--; y++; break;
                case 3: y++; break;
                case 4: x++; y++; break;
                case 5: r++; break;
                case 6: x++; y--; break;
                case 7: y--; break;
            }
            if(arrayOfAVs[x][y] == 1){
                Slot s(x, y);
                ongoingCheckedSlots.push(s);
                arrayOfAVs[x][y] = -1;
            }
            
            //return p;
        }

        void insertNeighborSlots(int** arrayOfAVs, Slot slot){
            int row = slot.row;
            int col = slot.column;
            int neighbors[8] = {(row - 1) | (col - 1), //0
                                (row - 1) | col, //1
                                (row - 1) | (col + 1) | (Column - col - 2), //2
                                (row) | (col + 1) | (Column - col - 2),      //3               
                                (row + 1) | (col + 1) | (Column - col - 2) | (Row - row - 2),  //4
                                (row + 1) | (col) | (Row - row - 2), //5
                                (row + 1) | (col - 1) | (Row - row - 2), //6
                                (row + 1) | (col + 1) | (Column - col - 2) | (Row - row - 2) //7
                                };
            for(int i = 0; i < 8; i++){
                if(neighbors[i] >= 0){
                    getNeighborByIndex(arrayOfAVs, i, row, col);
                }  
            }
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
        stack<Slot> ongoingCheckedSlots;
        //vector<point> fourVertecies;
        
        void check(int** arrayOfAVs, int r0, int c0, int rowsInStack, int columnsInStack){
            
            for(int i = r0; i < rowsInStack; i++){
                for(int j = c0; j < columnsInStack; j++){
                    if(arrayOfAVs[i][j] == 1){//Tim thay mot diem chua thuoc polygon nao ca
                        Slot s;
                        s.row = i; s.column = j;
                        arrayOfAVs[i][j] = -1;
                        ongoingCheckedSlots.push(s);
                        while(!ongoingCheckedSlots.empty()){
                            Slot temp = ongoingCheckedSlots.top();
                            getFourVertecies(temp.row, temp.column);
                            insertNonExistedPoints();
                            insertNeighborSlots(arrayOfAVs, temp);
                            ongoingCheckedSlots.pop();
                        }
                    }
                }
            }
            int i = 1;
            for(auto p : checkedPoints){
                cout<<i<<") ("<<p.x<<", "<<p.y<<") ";
                i++;
            }
        }

        void insertNonExistedPoints(){
            int i = 0;
            //bool found = false;
            bool pA_Is_Existed = false;
            bool pB_Is_Existed = false;
            bool pC_Is_Existed = false;
            bool pD_Is_Existed = false;

            for(; i < checkedPoints.size(); i++){
                if(checkedPoints.at(i) == pA) 
                {
                   pA_Is_Existed = true;
                   break;
                }
            }
            if(pA_Is_Existed)
                checkedPoints.erase(checkedPoints.begin() + i);
            
            for(i = 0; i < checkedPoints.size(); i++){
                if(checkedPoints.at(i) == pB) 
                {
                   pB_Is_Existed = true;
                   break;
                }
            }
            if(pB_Is_Existed)
                checkedPoints.erase(checkedPoints.begin() + i);

            for(i = 0; i < checkedPoints.size(); i++){
                if(checkedPoints.at(i) == pC) 
                {
                   pC_Is_Existed = true;
                   break;
                }
            }

            if(pC_Is_Existed)
                checkedPoints.erase(checkedPoints.begin() + i);
            
            for(i = 0; i < checkedPoints.size(); i++){
                if(checkedPoints.at(i) == pD) 
                {
                   pD_Is_Existed = true;
                   break;
                }
            }
            if(pD_Is_Existed)
                checkedPoints.erase(checkedPoints.begin() + i);
            
            if(!pA_Is_Existed){
                point p(pA.x, pA.y);
                checkedPoints.push_back(p);
            }
            if(!pB_Is_Existed){
                point p(pB.x, pB.y);
                checkedPoints.push_back(p);
            }
            if(!pC_Is_Existed){
                point p(pC.x, pC.y);
                checkedPoints.push_back(p);
            }
            if(!pD_Is_Existed){
                point p(pD.x, pD.y);
                checkedPoints.push_back(p);
            }
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