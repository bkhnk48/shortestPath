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

#include "draw.cpp"

using namespace std;

class BuildingPolygons{
    private:
        int WIDTH;
        int LENGTH; 
        point pA, pB, pC, pD;
        vector<Coincidence> checkedPoints;
        int ROWS;
        int COLUMNS;
        int cordX0;
        int cordY0;
        
        queue<Slot> ongoingCheckedSlots;
        
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
                case 5: x++; break;//previous version gets r++ here which is totally wrong 
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
                                (row - 1) | (col + 1) | (COLUMNS - col - 2), //2
                                (row) | (col + 1) | (COLUMNS - col - 2),      //3               
                                (row + 1) | (col + 1) | (COLUMNS - col - 2) | (ROWS - row - 2),  //4
                                (row + 1) | (col) | (ROWS - row - 2), //5
                                (row + 1) | (col - 1) | (ROWS - row - 2), //6
                                (row + 1) | (col + 1) | (COLUMNS - col - 2) | (ROWS - row - 2) //7
                                };
            for(int i = 0; i < 8; i++){
                if(neighbors[i] >= 0){
                    getNeighborByIndex(arrayOfAVs, i, row, col);
                }  
            }
        }

    public:
        //int** checkedPoints;
        
        //vector<point> fourVertecies;
        
        void check(int** arrayOfAVs, int r0, int c0){
            
            for(int i = r0; i < ROWS; i++){
                for(int j = c0; j < COLUMNS; j++){
                    if(arrayOfAVs[i][j] == 1){//Tim thay mot diem chua thuoc polygon nao ca
                        Slot s;
                        s.row = i; s.column = j;
                        arrayOfAVs[i][j] = -1;
                        ongoingCheckedSlots.push(s);
                        while(!ongoingCheckedSlots.empty()){
                            Slot temp = ongoingCheckedSlots.front();
                            getFourVertecies(temp.row, temp.column);
                            insertNonExistedPoints(arrayOfAVs, temp);
                            insertNeighborSlots(arrayOfAVs, temp);
                            ongoingCheckedSlots.pop();
                        }
                    }
                }
            }
            
            int i = 1;
            for(auto coincidence : checkedPoints){
                cout<<i<<") ("<<coincidence.p.x<<", "<<coincidence.p.y<<") ";
                i++;
            }
        }

        void insertNonExistedPoints(int** arrayOfAVs, Slot center){
            //int i = 0;
            bool found = false;

            double xCenterGroup[4] = {0};  double yCenterGroup[4] = {0};

            point group[4] = {pA, pB, pC, pD};
            for(int i = 0; i < 4; i++){
                found = false;
                for(int j = 0; j < checkedPoints.size(); j++){
                    if(checkedPoints.at(j) == group[i]) 
                    {
                        found = true;
                        break;
                    }
                }

                if(!found){
                    xCenterGroup[0] = group[i].x - (WIDTH/2);  yCenterGroup[0] = group[i].y + (LENGTH/2);
                    xCenterGroup[1] = group[i].x + (WIDTH/2);  yCenterGroup[1] = group[i].y + (LENGTH/2);
                    xCenterGroup[2] = group[i].x + (WIDTH/2);  yCenterGroup[2] = group[i].y - (LENGTH/2);
                    xCenterGroup[3] = group[i].x - (WIDTH/2);  yCenterGroup[3] = group[i].y - (LENGTH/2);

                    int aroundAVs = 0;
                    for(int j = 0; j < 4; j++){
                        int row = ((int)(this->cordY0 - yCenterGroup[j])) / LENGTH;
                        int col = ((int)(xCenterGroup[j] - this->cordX0)) / WIDTH;
                        if((row | col | (COLUMNS - col - 1) | (ROWS - row - 1)) >= 0){
                            if(arrayOfAVs[row][col] != 0)
                                aroundAVs++;
                        }
                    }

                    if(aroundAVs < 4){
                        point p(group[i].x, group[i].y);
                        checkedPoints.push_back(p);
                    }
                }
            }
        }

        BuildingPolygons(int Row, int Column, int cordX, int cordY, int width, int length){
            this->ROWS = Row;
            this->COLUMNS = Column;
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