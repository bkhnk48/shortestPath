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

class LeftForwardingState : public virtual InitState
{

    public:
        char nextState(int **array, int ROW, int COL){
            if(c < COL - 1){
                if(array[r][c + 1] == 1){
                    return 'S';//Still
                }
            }
            else{//reach right boundary
                if(r < ROW - 1){
                    if(array[r + 1][c] == 1){
                        return 'D';//Down
                    }
                    else{
                        return 'R'; //Quay lui lại
                        //Ô trống ở dưới hàng này
                    }
                }
                else{//đang duyệt hàng cuối cùng
                    return 'R';//quay lui lại
                }
            }
        }
};