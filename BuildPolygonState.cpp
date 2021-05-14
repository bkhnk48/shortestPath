#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>

using namespace std;

class InitState{
    public:
        int r;
        int c;
        InitState(int r, int c){
            this->r = r;
            this->c = c;
        }
        virtual char nextState(int **array, int R, int C);
    
};


class RightForwardingState : public virtual InitState
{

    public:
        RightForwardingState(int r, int c) : InitState{r, c}{

        }
        char nextState(int **array, int ROW, int COL){
            if(c < COL - 1){
                if(array[r][c + 1] == 1){
                    return 'S';//Still
                }
                else{
                    return 'D';//Down
                }
            }
            else{//reach right boundary
                if(r < ROW - 1){
                    if(array[r + 1][c] == 1){
                        return 'D';//Down
                    }
                    else{
                        return 'L'; //Quay lui lại
                        //tức turn left
                        //Ô trống ở dưới hàng này
                    }
                }
                else{//đang duyệt hàng cuối cùng
                    return 'L';//quay lui lại
                }
            }
        }
};

class Factory
{
    public:
        static InitState* getNextState(InitState* s, char option){
            if(option == 'S')
                return s;
            int r = s->r; int c = s->c;
            delete s;
            if(option == 'R')
            {
                RightForwardingState* newState = new RightForwardingState(r, c);
                return newState;
            }
            return NULL;
        }
};