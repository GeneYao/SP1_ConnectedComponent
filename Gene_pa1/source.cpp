#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <queue>

using namespace std;

struct coordinate
{
    int x;
    int y;
};

void CollorFill(coordinate pos, char color, int w, int h, vector<vector<char>> &bitMap, vector<vector<bool>> &visit);
int ConnectCount(int w, int h, vector<vector<char>> &bitMap, vector<vector<bool>> &visit);
void BFS(coordinate pos, int w, int h, vector<vector<char>> &bitMap, vector<vector<bool>> &visit);

int main(int argc, char *argv[])
{
    ifstream fin(argv[1]);       

    if (!fin) {
        cout << "# file reading error" << endl;
		return 0;
    }
    else if (argc < 6) {
        cout << "Please input : .//paintBucket [image*.in] [x] [y] [color] [image*.out]" << endl;
    }
    else {
        // --- 1. Read Input File ---        
        vector<vector<char>> bitMap;
        vector<vector<bool>> colorVisit, countVisit;
        int connectCount;
        coordinate pos = {atoi(argv[2]), atoi(argv[3])};
        char color = *argv[4];

        int w, h;
        fin >> w >> h;
        // declare bitMap & initailize               
        bitMap.resize(h);
        colorVisit.resize(h);
        countVisit.resize(h);
        for(int i = 0; i < h; ++i) {
            bitMap[i].resize(w);
            colorVisit[i].resize(w);
            countVisit[i].resize(w);
        }             
        
        // push value into 2D vector
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                fin >> bitMap[i][j];
                colorVisit[i][j] = false;
                countVisit[i][j] = false;
            }      
        }           
        
        fin.close();
        
        // --- 2. Color Fill ---        
        CollorFill(pos, color, w, h, bitMap, colorVisit);

        // --- 3. Count Conneted Component ---    
        connectCount = ConnectCount(w, h, bitMap, countVisit);            

        // --- 3. Write Output File ---
        ofstream fout(argv[5]);
        fout << h << " " << w << endl;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++)
                fout << bitMap[i][j] << " ";
            fout << endl;
        }
        fout << connectCount << endl;
        fout.close();        
    }   

    return 0;
}

void CollorFill(coordinate pos, char color, int w, int h, vector<vector<char>> &bitMap, vector<vector<bool>> &visit)
{
    BFS(pos, w, h, bitMap, visit);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (visit[i][j] == true) {
                bitMap[i][j] = color;
            }
        }      
    }
}

int ConnectCount(int w, int h, vector<vector<char>> &bitMap, vector<vector<bool>> &visit)
{
    int connectCount = 0;
    coordinate curPos;

    // traverse bitMap
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (visit[i][j] == false) {
                curPos = {j, i};                
                BFS(curPos, w, h, bitMap, visit);
                connectCount++;
            }
        }      
    }
    return connectCount;
}

void BFS(coordinate pos, int w, int h, vector<vector<char>> &bitMap, vector<vector<bool>> &visit)
{
    queue<coordinate> q;

    q.push(pos);
    visit[pos.y][pos.x] = true;

    while (!q.empty()) {
        coordinate curPos = q.front();
        q.pop();
        
        int rightStep = curPos.x + 1;
        int leftStep = curPos.x - 1;
        int upStep = curPos.y - 1;
        int downStep = curPos.y + 1; 

        if (rightStep < w && bitMap[curPos.y][curPos.x] == bitMap[curPos.y][rightStep] && visit[curPos.y][rightStep] == false) {
            visit[curPos.y][rightStep] = true;            
            q.push({rightStep, curPos.y});
        }
        if (leftStep >= 0 && bitMap[curPos.y][curPos.x] == bitMap[curPos.y][leftStep] && visit[curPos.y][leftStep] == false) {
            visit[curPos.y][leftStep] = true;            
            q.push({leftStep, curPos.y});
        } 
        if (upStep >= 0 && bitMap[curPos.y][curPos.x] == bitMap[upStep][curPos.x] && visit[upStep][curPos.x] == false) {
            visit[upStep][curPos.x] = true;
            q.push({curPos.x, upStep});
        } 
        if (downStep < h && bitMap[curPos.y][curPos.x] == bitMap[downStep][curPos.x] && visit[downStep][curPos.x] == false) {
            visit[downStep][curPos.x] = true;            
            q.push({curPos.x, downStep});
        }
    }
}
