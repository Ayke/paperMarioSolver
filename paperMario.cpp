#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <set>
#include <map>
#include <queue>
using namespace std;

bool DEBUG = false;

const int STEPLIMIT = 6;

//NUM is the number of enemies
const int NUM = 8;

const string fileName = "paperMario.in";
ifstream inputFile(fileName);
//N is the number of columns, counting clockwise starting from 0 clock
const int N = 12;
//M is the number of row, counting outwards (the innermost cicle is row 0)
const int M = 4;


int e[N][M];

// set <long long> visited;
map <long long, long long> visited;
map <long long, int> steps;

queue <long long> q;

int inputMap[NUM][2] = {
    {1,0},{1,1},{4,1},{4,0},
    {7,1},{7,0},{7,2},{7,3}
};


void printMap(int e[N][M])
{
    for(int j = M-1; j >= 0; j--) {
        for(int i = 0; i < N; i++) {
            cout << e[i][j];
        }
        cout << endl;
    }
    cout << "-------------------------------" << endl;
}


void mapDecode(int e[N][M], long long code)
{
    for(int i = N-1; i >= 0; i--) {
        for(int j = M-1; j >= 0; j--) {
            e[i][j] = (int) (code & 1);
            code = code >> 1;
        }
    }
}

long long mapEncode(int e[N][M])
{
    long long ans = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            ans = (ans << 1) + e[i][j];
        }
    }
    return ans;
}

bool victory(int e[N][M])
{
    int mapCopy[N][M];
    for(int i = 0; i < N; i++) for(int j = 0; j < M; j++) mapCopy[i][j] = e[i][j];

    //Test if enemies are in row
    for(int i = 0; i < N; i++) {
        if ((mapCopy[i][0] == 0) || (mapCopy[i][1] == 0) || (mapCopy[i][2] == 0) || (mapCopy[i][3] == 0)) continue;
        mapCopy[i][0] = 0;
        mapCopy[i][1] = 0;
        mapCopy[i][2] = 0;
        mapCopy[i][3] = 0;
    }

    if (DEBUG) {
        cout << "Victory test after row sweeping:" << endl;
        printMap(mapCopy);
    }

    //Test if enemies are in blocks
    bool inBlock[N];
    for(int i = 0; i < N; i++) {
        inBlock[i] = false;
        if ((mapCopy[i][0] == 0) && (mapCopy[i][1] == 0) && (mapCopy[i][2] == 0) && (mapCopy[i][3] == 0)) continue;
        if ((mapCopy[i][0] == 1) && (mapCopy[i][1] == 1) && (mapCopy[i][2] == 0) && (mapCopy[i][3] == 0)) inBlock[i] = true;
        else return false;
    }
    for(int i = 0; i < N; i++) {
        if (!inBlock[i]) continue;
        int j = (i-1 + N) % N;
        if (inBlock[j]) {
            inBlock[j] = false;
            inBlock[i] = false;
            mapCopy[j][0] = 0;  mapCopy[j][1] = 0;  mapCopy[j][2] = 0;  mapCopy[j][3] = 0;
            mapCopy[i][0] = 0;  mapCopy[i][1] = 0;  mapCopy[i][2] = 0;  mapCopy[i][3] = 0;
        }
    }

    if (DEBUG) {
        cout << "Victory test after block sweeping:" << endl;
        printMap(mapCopy);
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if (mapCopy[i][j] == 1) return false;
        }
    }
    return true;
}

void loadInput()
{
    int dx, dy;
    int enemyNum = 0;

    if (inputFile.is_open()) {
        while (inputFile >> dx) {
            inputFile >> dy;
            dx %= N;    dy %= M;
            e[dx][dy] = 1;
            enemyNum++;
        }
        cout << "Read " << enemyNum << " enemies from file." << endl;
    } else cout << "Cannot open the input file." << endl;


    // // DEBUG INPUT
    // for(int i = 0; i < NUM; i++) {
    //     dx = inputMap[i][0] % N;
    //     dy = inputMap[i][1] % M;
    //     e[dx][dy] = 1;
    // }
    
    printMap(e);
    long long initK = mapEncode(e);
    visited[initK] = -1;
    steps[initK] = 0;
    q.push(initK);


    // int e2[N][M];
    // mapDecode(e2, 153931704172548);
    // printMap(e2);
    // cout << q.front() << endl;
    // cout << visited.count(initK) << visited.count(1) << endl;
    // cout << victory(e) << endl;
}

long long search()
{
    // bool first = true;
    while (q.size() > 0) {
        long long code = q.front();
        q.pop();

        mapDecode(e, code);
        if (victory(e)) return code;
        if (steps[code] >= STEPLIMIT) continue;

        // cout << "Rotation" << endl;
        // Opeartion: rotating a row
        for(int row = 0; row < M; row++) {
            mapDecode(e, code);
            for(int angle = 1; angle <= N-1; angle++) {
                int tmp = e[N-1][row];
                for(int i = N-1; i >= 1; i--) e[i][row] = e[i-1][row];
                e[0][row] = tmp;
                // cout << "Row,Angle:" << row << ',' << angle << endl;
                // if (first) printMap(e);
                // Check if this node has been visited
                long long newCode = mapEncode(e);
                if (visited.count(newCode) != 0) continue;
                // Visit this node.
                visited[newCode] = code;
                steps[newCode] = steps[code] + 1;
                q.push(newCode);
            }
        }

        // cout << "Sliding" << endl;
        // Operation: sliding a column
        for(int col = 0; col < N / 2; col++) {
            int col2 = col + (N / 2);
            mapDecode(e, code);
            for(int tick = 1; tick <= 2*M-1; tick++) {
                int tmp = e[col][0];
                for(int i = 0; i <= M-2; i++) e[col][i] = e[col][i+1];
                e[col][M-1] = e[col2][M-1];
                for(int i = M-1; i >= 1; i--) e[col2][i] = e[col2][i-1];
                e[col2][0] = tmp;
                // cout << "Column,tick:" << col << ':' << col2 << ',' << tick << endl;
                // if (first) printMap(e);
                // Check if this node has been visited
                long long newCode = mapEncode(e);
                if (visited.count(newCode) != 0) continue;
                // Visit this node.
                visited[newCode] = code;
                steps[newCode] = steps[code] + 1;
                q.push(newCode);
            }
        }
        // first = false;
    }
    return -1;
}

void printSolution(long long code)
{
    long long ansArr[10];
    int n = 0;
    do {
        ansArr[n++] = code;
        code = visited[code];
    } while (code >= 0);
    for(int i = n-1; i >= 0; i--) {
        mapDecode(e, ansArr[i]);
        printMap(e);
    }
}

void paintMap(int e[N][M], int inputMap[][2], int num)
{
    for(int i = 0; i < num; i++) {
        int dx = inputMap[i][0] % N;
        int dy = inputMap[i][1] % M;
        e[dx][dy] = 1;
    }
}

void debugCheck()
{
    int m1[N][M];
    long long m1Code;

    int inputM1[4][2] = {{1,0},{1,1},{7,0},{7,3}};
    mapDecode(m1, 0);
    paintMap(m1, inputM1, 4);
    m1Code = mapEncode(m1);
    if (visited.count(m1Code) != 0) cout << "The following map has been visited:" << endl;
    else cout << "The following map has NOT been visited:" << endl;    
    printMap(m1);



    int inputM2[4][2] = {{1,0},{1,1},{7,0},{6,3}};
    mapDecode(m1, 0);
    paintMap(m1, inputM2, 4);
    m1Code = mapEncode(m1);
    if (visited.count(m1Code) != 0) cout << "The following map has been visited:" << endl;
    else cout << "The following map has NOT been visited:" << endl;    
    printMap(m1);


    int inputM3[4][2] = {{1,1},{1,2},{1,0},{6,3}};
    mapDecode(m1, 0);
    paintMap(m1, inputM3, 4);
    m1Code = mapEncode(m1);
    if (visited.count(m1Code) != 0) cout << "The following map has been visited:" << endl;
    else cout << "The following map has NOT been visited:" << endl;    
    printMap(m1);


    int inputM4[4][2] = {{1,1},{1,2},{1,0},{1,3}};
    mapDecode(m1, 0);
    paintMap(m1, inputM4, 4);
    m1Code = mapEncode(m1);
    if (visited.count(m1Code) != 0) cout << "The following map has been visited:" << endl;
    else cout << "The following map has NOT been visited:" << endl;    
    printMap(m1);
    DEBUG = true;
    cout << "VICTORY: ";
    cout << victory(m1);
    cout << endl << endl;
}

int main()
{
    loadInput();
    long long answer = search();
    // debugCheck();
    cout << "THE ANSWER IS...." << endl;
    if (answer < 0) cout << "Sorry, no solution" << endl;
    else printSolution(answer);
    return 0;
}
