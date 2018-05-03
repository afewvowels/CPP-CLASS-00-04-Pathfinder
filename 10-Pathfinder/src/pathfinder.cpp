//
//  pathfinder.cpp
//  10-Pathfinder
//
//  Created by Keith Smith on 4/26/18.
//  Copyright © 2018 Keith Smith. All rights reserved.
//

#include "pathfinder.h"

int sampleNoReplacement(int items[],int &n,int randomValue) {
    int i = randomValue % n;
    int temp = items[i];
    n-=1;
    items[i]=items[n-1];
    
    return temp;
}

void showBits(unsigned int n) {
    for(int i = 31; i >= 0; i--) {
        if(n & (1 << i)) {
            cout << "1";
        }
        else {
            cout << "0";
        }
    }
    
    cout << endl;
}

u_int8_t setBit(u_int8_t bitToShift, unsigned int bitShift, bool value) {
    u_int8_t mask = 1 << bitShift;
    u_int8_t state = u_int8_t(value);
    return (bitToShift & ~mask) | (-state & mask);
}

bool isBitSet(u_int8_t bitToCheck, unsigned int position) {
    bitToCheck >>= position;
    
    return (bitToCheck & 1) != 0;
}

uint8_t encode(uint8_t r, uint8_t nR, uint8_t c, uint8_t nC, uint8_t d) {
    return c + nC * (r + nR * d);
}

uint8_t decodeDirection(uint8_t coded, uint8_t nR, uint8_t nC) {
    return coded / (nR * nC);
}

uint8_t decodeColumn(uint8_t coded, uint8_t nR, uint8_t nC) {
    return coded % nC;
}

uint8_t decodeRow(uint8_t coded, uint8_t nR, uint8_t nC) {
    return (coded / nC) % nR;
}

void generateMaze(uint8_t maze[][MAX_COLS], uint8_t nR, uint8_t nC) {
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(0,130000);
    
    int count = (nR*nC*4);
    
    int *items;
    items = new int[count];
    
    int i = 0;
    
    for(int r = 0; r < nR; r++) {
        for(int c = 0; c < nC; c++) {
            maze[r][c] = 15;
            for(int d = 0; d < 4; d++) {
                items[i] = encode(r,nR,c,nC,d);
                i++;
            }
        }
    }

    DisjointSet disjointSet = *new DisjointSet(nR*nC, nR, nC);

    i = 0;

    while(i < (nR*nC-1)) {
        int e;

        int room1;
        int room2;

        uint8_t r;
        uint8_t c;
        uint8_t d;
        
        int randomValue = distribution(generator);
        
        e = sampleNoReplacement(items, count, randomValue);
        
        r = decodeRow(e, nR, nC);
        c = decodeColumn(e, nR, nC);
        d = decodeDirection(e, nR, nC);
        
        if((r == 0 && d == 0) ||
           (r == (nR-1) && d == 2) ||
           (c == 0 && d == 3) ||
           (c == (nC-1) && d == 1)) {
            continue;
        }
        
        // 0. Remove two nested do / do loops
        // 1. Get sample
        // 2. Decode
        // 3. If exterior checks, continue
        // 4. If r & c & d == 0, continue directions
        // 5. if d==0, does wall exist, are room connected, else continue
        // 6. check adjacent wall in room2, continue
        // 7. Then get room2
        // 8. Then remove wall and adjacent room's wall
        // 9.
        // 10. if disjointSet.find == disjointSet.find, continue

        if(!isBitSet(maze[r][c], d)) {
            continue;
        }
        
        if(d == 0 && r != 0) {
            if(!isBitSet(maze[r-1][c], 2)) {
                setBit(maze[r][c], d, 0);
                continue;
            }
        }
        else if(d == 1 && c != (nC-1)) {
            if(!isBitSet(maze[r][c+1], 3)) {
                setBit(maze[r][c], d, 0);
                continue;
            }
        }
        else if(d == 2 && r != (nR-1)) {
            if(!isBitSet(maze[r+1][c], 0)) {
                setBit(maze[r][c], d, 0);
                continue;
            }
        }
        else if(d == 3 && c != 0) {
            if(!isBitSet(maze[r][c-1], 1)) {
                setBit(maze[r][c], d, 0);
                continue;
            }
        }
        
        room1 = disjointSet.encode(r, c);
        
        if(d==0) {
            room2 = disjointSet.encode(r-1, c);
        }
        else if(d==1) {
            room2 = disjointSet.encode(r, c+1);
        }
        else if(d==2) {
            room2 = disjointSet.encode(r+1, c);
        }
        else if(d==3) {
            room2 = disjointSet.encode(r, c-1);
        }
        else {
            continue;
        }
        
        if(disjointSet.disjointSetFind(room1) == disjointSet.disjointSetFind(room2)) {
            continue;
        }

        disjointSet.disjointSetUnion(room1, room2);
        i++;
        
        int r1 = disjointSet.decodeRow(room1);
        int c1 = disjointSet.decodeColumn(room1);
        
        int r2 = disjointSet.decodeRow(room2);
        int c2 = disjointSet.decodeColumn(room2);

        // r1 below r2
        if(r1 > r2 && c1 == c2) {
            maze[r1][c1] = setBit(maze[r1][c1], 0, 0);
            maze[r2][c2] = setBit(maze[r2][c2], 2, 0);
        }
        // r1 above r2
        else if(r1 < r2 && c1 == c2) {
            maze[r1][c1] = setBit(maze[r1][c1], 2, 0);
            maze[r2][c2] = setBit(maze[r2][c2], 0, 0);
        }
        // c2 | c1
        else if(r1 == r2 && c1 > c2) {
            maze[r1][c1] = setBit(maze[r1][c1], 3, 0);
            maze[r2][c2] = setBit(maze[r2][c2], 1, 0);

        }
        // c1 | c2
        else if(r1 == r2 && c1 < c2) {
            maze[r1][c1] = setBit(maze[r1][c1], 1, 0);
            maze[r2][c2] = setBit(maze[r2][c2], 3, 0);
        }
        else {
            continue;
        }
    }
    
    delete [] items;
}

void findPath(uint8_t maze[][MAX_COLS], uint8_t nR, uint8_t nC) {
    Stack<uint8_t> s;
    
    uint8_t r;
    uint8_t c;
    uint8_t d;
    
    int rNext;
    int cNext;
    
    s.push(encode(0,nR,0,nC,0));
    
    maze[0][0] |= VISITED;
    
    while(!s.isEmpty()) {
        r = decodeRow(s.peek(), nR, nC);
        c = decodeColumn(s.peek(), nR, nC);
        d = decodeDirection(s.peek(), nR, nC);
        cout << "Decode value " << unsigned(s.peek()) << endl;
        cout << "Decoded R " << unsigned(r) << " C " << unsigned(c) << " D " << unsigned(d) << endl;
        
        if(r == (nR-1) && c == (nC-1)) {
            break;
        }
        
        if(d == 4) {
            maze[r][c] |= DEAD_END;
            s.pop();
            cout << "Dead end" << endl;
        }
        else {
            if(d == 0) {
                rNext = r - 1;
                cNext = c;
            }
            else if(d == 1) {
                rNext = r;
                cNext = c + 1;
            }
            else if(d == 2) {
                rNext = r + 1;
                cNext = c;
            }
            else if(d == 3) {
                rNext = r;
                cNext = c - 1;
            }
            
            cout << "R " << unsigned(r) << " C " << unsigned(c) << endl;
            s.pop();
            s.push(encode(r,nR,c,nC,d+1));
            cout << "Encode value " << unsigned(encode(r,nR,c,nC,d+1)) << endl;
            cout << "Encoded R " << unsigned(r) << " C " << unsigned(c) << " D " << unsigned(d+1) << endl;
            
            if(!isBitSet(maze[r][c], d) &&
               (maze[rNext][cNext] != unsigned(16) && maze[rNext][cNext] != unsigned(17) &&
                maze[rNext][cNext] != unsigned(18) && maze[rNext][cNext] != unsigned(19) &&
                maze[rNext][cNext] != unsigned(20) && maze[rNext][cNext] != unsigned(21) &&
                maze[rNext][cNext] != unsigned(22) && maze[rNext][cNext] != unsigned(23) &&
                maze[rNext][cNext] != unsigned(24) && maze[rNext][cNext] != unsigned(25) &&
                maze[rNext][cNext] != unsigned(26) && maze[rNext][cNext] != unsigned(27) &&
                maze[rNext][cNext] != unsigned(28) && maze[rNext][cNext] != unsigned(29) &&
                maze[rNext][cNext] != unsigned(30) && maze[rNext][cNext] != unsigned(31) &&
                maze[rNext][cNext] != unsigned(32) && maze[rNext][cNext] != unsigned(33) &&
                maze[rNext][cNext] != unsigned(34) && maze[rNext][cNext] != unsigned(35) &&
                maze[rNext][cNext] != unsigned(36) && maze[rNext][cNext] != unsigned(37) &&
                maze[rNext][cNext] != unsigned(38) && maze[rNext][cNext] != unsigned(39) &&
                maze[rNext][cNext] != unsigned(40) && maze[rNext][cNext] != unsigned(41) &&
                maze[rNext][cNext] != unsigned(42) && maze[rNext][cNext] != unsigned(43) &&
                maze[rNext][cNext] != unsigned(44) && maze[rNext][cNext] != unsigned(45) &&
                maze[rNext][cNext] != unsigned(46) && maze[rNext][cNext] != unsigned(47))) {
                   s.push(encode(rNext,nR,cNext,nC,0));
                   maze[rNext][cNext] |= VISITED;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    uint8_t nR = atoi(argv[1]);
    uint8_t nC = atoi(argv[2]);
    
    uint8_t maze[MAX_ROWS][MAX_COLS];
    
    cout << "Generate maze" << endl;
    
    generateMaze(maze,nR,nC);
    
    cout << "Find path" << endl;

    findPath(maze,nR,nC);
    
    cout << "Print maze" << endl;
    
    printMaze(maze,nR,nC);
    
    cout << "Program ran." << endl;
    
    return 0;
}
