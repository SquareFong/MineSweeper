#ifndef MINESWEEP_H
#define MINESWEEP_H
#include<vector>
#include<queue>
#include<time.h>
#include<stdlib.h>
using namespace std;
class mineSweeper{
private:
    //-1 mine, >=0 mines number in area
    vector<vector<int>> mines;
    vector<vector<bool>> flags;
    vector<vector<bool>> discovered;
    int sizeOfMap;
    //-1 lose, 0 not end, 1 win
    int isEnd;
    int numberOfMines;
    int direction[8][2]={{1,0},
                         {1,1},
                         {0,1},
                         {-1,1},
                         {-1,0},
                         {-1,-1},
                         {0,-1},
                         {1,-1},
                        };
    inline bool isInBoard(int x, int y)const{
            return x>=0 && x<sizeOfMap && y>=0 && y<sizeOfMap;
    }
public:
    mineSweeper(unsigned lines):sizeOfMap(int(lines)),isEnd(false){
        //initialize map
        for(unsigned i=0; i<lines; ++i)
        {
            mines.push_back(vector<int>(lines,0));
            discovered.push_back(vector<bool>(lines,false));
            flags.push_back(vector<bool>(lines,false));
        }
        //initialize number of landmines
        switch (lines) {
        case 8:numberOfMines = 10;break;
        case 16:numberOfMines = 40;break;
        case 24:numberOfMines = 99;break;
        default:
            numberOfMines = sizeOfMap;
            break;
        }
        //initialize landemines
        clock_t c;
        int tx,ty;
        for(int i = 0; i < numberOfMines; ++i){
            c = clock();
            srand(unsigned(c));
            long long int r = rand();
            tx = r%sizeOfMap;
            r/=100;
            ty = r%sizeOfMap;
            if(mines[unsigned(tx)][unsigned(ty)] == -1)
                --i;
            else
                mines[unsigned(tx)][unsigned(ty)] = -1;
        }

        //continue to intialize map
        for(int i=0; i<sizeOfMap; ++i){
            for(int j=0; j<sizeOfMap; ++j){
                if(mines[unsigned(i)][unsigned(j)] == 0){
                    int counter = 0;
                    int x,y;
                    for(int k=0; k<8; ++k){
                        x=i+direction[k][0];
                        y=j+direction[k][1];
                        if(isInBoard(x,y)){
                            if(mines[unsigned(x)][unsigned(y)] == -1)
                                ++counter;
                        }
                    }
                    mines[unsigned(i)][unsigned(j)] = counter;
                }
            }
        }
    }

    vector<vector<int>> getMap()const{
        vector<vector<int>> temp = mines;
        for(int i=0; i < sizeOfMap; ++i){
            for(int j=0; j < sizeOfMap; ++j){
                if(!discovered[unsigned(i)][unsigned(j)])
                    temp[unsigned(i)][unsigned(j)] = -10;
            }
        }
        return temp;
    }

    bool clickAble(){
        return isEnd == 0;
    }

    int getResult(){
        return isEnd;
    }

    bool click(int x,int y){
        if((!isInBoard(x,y)) || discovered[unsigned(x)][unsigned(y)] || flags[unsigned(x)][unsigned(y)])
            return false;
        discovered[unsigned(x)][unsigned(y)] = true;
        if(mines[unsigned(x)][unsigned(y)] == -1){
            isEnd = -1;
            for(int i=0; i < sizeOfMap; ++i){
                for(int j=0; j < sizeOfMap; ++j){
                    if(mines[unsigned(i)][unsigned(j)] == -1)
                        discovered[unsigned(i)][unsigned(j)] = true;
                }
            }
        }

        if(mines[unsigned(x)][unsigned(y)] == 0){
            queue<int> sx;
            queue<int> sy;
            sx.push(x);
            sy.push(y);
            int &tempX= sx.front(),&tempY= sy.front();
            while (!sx.empty()) {
                tempX = sx.front();
                tempY = sy.front();
                for(int k(0); k<8; ++k){
                    int dx = tempX + direction[k][0];
                    int dy = tempY + direction[k][1];
                    if(isInBoard(dx,dy) && mines[unsigned(dx)][unsigned(dy)] >= 0){
                        if((mines[unsigned(dx)][unsigned(dy)] == 0) && !discovered[unsigned(dx)][unsigned(dy)]){
                            sx.push(dx);
                            sy.push(dy);
                        }
                        discovered[unsigned(dx)][unsigned(dy)] = true;
                    }
                }
                sx.pop();
                sy.pop();
            }
        }

        int i,j;
        int count = 0, count_1 = 0;
        for(i = 0; i<sizeOfMap; ++i){
            for(j=0; j<sizeOfMap; ++j){
                if((discovered[unsigned(i)][unsigned(j)] == false) ){
                    ++count_1;
                    if(mines[unsigned(i)][unsigned(j)] == -1)
                        ++count;
                }
            }
        }
        if(count_1==count && count == numberOfMines){
            isEnd = 1;
        }
        return true;
    }

    bool isFlag(int x,int y){
        return flags[unsigned(x)][unsigned(y)];
    }

    bool putFlags(int x, int y){
        if(isInBoard(x,y) && !discovered[unsigned(x)][unsigned(y)]){
            flags[unsigned(x)][unsigned(y)] = true;

            int nf=0;
            int i=0,j=0;
            for(i = 0; i < sizeOfMap; ++i){
                for(j = 0; j< sizeOfMap; ++j){
                    if((flags[unsigned(i)][unsigned(j)]) == (mines[unsigned(i)][unsigned(j)] == -1)){
                        ++nf;
                        continue;
                    }
                    else {
                        break;
                    }
                }
            }
            if(i == sizeOfMap && j == sizeOfMap && nf == numberOfMines){
                isEnd = 1;
            }

            return true;
        }
        return false;
    }

    bool unputFlags(int x,int y){
        if(flags[unsigned(x)][unsigned(y)]){
            flags[unsigned(x)][unsigned(y)] = false;
            return true;
        }
        return false;
    }

    bool getArecommand(int &x, int &y){
        for(int i(0); i < sizeOfMap; ++i){
            for(int j(0); j<sizeOfMap; ++j){
                if(mines[unsigned(i)][unsigned(j)] >=0 && !discovered[unsigned(i)][unsigned(j)]){
                    if(flags[unsigned(i)][unsigned(j)])
                        flags[unsigned(i)][unsigned(j)] = false;
                    x = i;
                    y = j;
                    click(x,y);
                    return true;
                }
            }
        }
        return false;
    }
};

#endif // MINESWEEPER_H
