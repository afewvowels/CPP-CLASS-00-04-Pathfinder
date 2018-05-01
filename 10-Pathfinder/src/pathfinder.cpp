//
//  pathfinder.cpp
//  10-Pathfinder
//
//  Created by Keith Smith on 4/26/18.
//  Copyright © 2018 Keith Smith. All rights reserved.
//

#include <iostream>
#include "pathfinder.h"
#include <stdlib.h>
#include <bitset>
#include "DisjointSet.h"
#include <random>

int sampleNoReplacement(int items[],int &n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(1,130000);
    int i;
    if(n != 0) {
        i = distribution(generator) % n;
    }
    else {
        cout << "n == 0" << endl;
        return 0;
    }
    int temp;
    temp = items[i];
    n-=1;
    items[i]=items[n];
    
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

u_int8_t encode(int r,int nR,int c,int nC,u_int8_t d) {
    return (r*nC+c) * 4 + d;
}

u_int8_t decodeDirection(int coded) {
    return coded % 4;
}

u_int8_t decodeColumn(int coded) {
    return (coded/4) & 50;
}

u_int8_t decodeRow(int coded) {
    return (coded/(4*50));
}

void generateMaze(uint8_t maze[], uint8_t printMaze[][MAX_COLS], int nR, int nC) {
    for(int r = 0; r < MAX_ROWS; r++) {
        for(int c = 0; c < MAX_COLS; c++) {
        }
    }
}

int main(int argc, const char * argv[]) {
    
    int nR = atoi(argv[1]);
    int nC = atoi(argv[2]);
    
    int count = (nR*nC*4);
    
    int items[count];
    
    DisjointSet disjointSet = *new DisjointSet(nR*nC, nR, nC);
    
    uint8_t mazePrime[count];
    uint8_t maze[MAX_ROWS][MAX_COLS];
    
    int i = 0;
    
    for(int r = 0; i < nR; r++) {
        for(int c = 0; c < nC; c++) {
            mazePrime[encode(r,nR,c,nC,0)] = 15;
            for(int d = 0; d < 4; d++) {
                items[i] = encode(r,nR,c,nC,d);
                i++;
            }
        }
    }
    
    i = 0;
    
    int numItems = nR*nC;
    
    while(i < (nR*nC-1)) {
        int e;
        
        u_int8_t room1;
        u_int8_t room2;
        
        int r;
        int c;
        int d;
        
        do {
            do {
                e = sampleNoReplacement(items, numItems);
                cout << "Sampling" << endl;
            } while((decodeRow(e) == 0 && isBitSet(decodeDirection(e),0)) ||
                     (decodeRow(e) == nR-1 && isBitSet(decodeDirection(e),2)) ||
                     (decodeColumn(e) == 0 && isBitSet(decodeDirection(e),3)) ||
                     (decodeColumn(e) == nC-1 && isBitSet(decodeDirection(e), 1)));
            
            cout << "Room found" << endl;
            
            r = decodeRow(e);
            c = decodeColumn(e);
            d = decodeDirection(e);
            
            room1 = encode(r,nR,c,nC,d);
            
            if(d==0) {
                room2 = encode(r+1,nR,c,nC,2);
            }
            else if(d==1) {
                room2 = encode(r,nR,c+1,nC,3);
            }
            else if(d==2) {
                room2 = encode(r-1,nR,c,nC,0);
            }
            else if(d==3) {
                room2 = encode(r,nR,c-1,nC,1);
            }
            else {
                room2 = 0;
                cout << "Error determining wall direction" << endl;
            }
            
        } while(disjointSet.disjointSetFind(room1) == disjointSet.disjointSetFind(room2));

        cout << "Room union" << endl;
        disjointSet.disjointSetUnion(room1, room2);
        i++;
        
        room1 = mazePrime[encode(decodeRow(room1), nR, decodeColumn(room1), nC, 0)];
        room2 = mazePrime[encode(decodeRow(room2), nR, decodeColumn(room2), nC, 0)];
            
        if(decodeRow(room1) > decodeRow(room2) && decodeColumn(room1) == decodeColumn(room2)) {
            room1 = setBit(room1, 2, 0);
            room2 = setBit(room2, 0, 0);
        }
        else if(decodeRow(room1) < decodeRow(room2) && decodeColumn(room1) == decodeColumn(room2)) {
            room1 = setBit(room1, 0, 0);
            room2 = setBit(room2, 2, 0);
        }
        else if(decodeRow(room1) == decodeRow(room2) && decodeColumn(room1) > decodeColumn(room2)) {
            room1 = setBit(room1, 3, 0);
            room2 = setBit(room2, 1, 0);
        }
        else if(decodeRow(room1) == decodeRow(room2) && decodeColumn(room1) < decodeColumn(room2)) {
            room1 = setBit(room1, 1, 0);
            room2 = setBit(room2, 3, 0);
        }
        else {
            cout << "ERROR REMOVING WALL" << endl;
        }
        
        mazePrime[encode(decodeRow(room1), nR, decodeColumn(room1), nC, 0)] = room1;
        mazePrime[encode(decodeRow(room2), nR, decodeColumn(room2), nC, 0)] = room2;
        
        cout << "wall removed" << endl;
    }
    
    for(int i = 0; i < count; i++) {
        items[i] = i;
    }
    
    for(int i = 0; i < (nR*nC-1); i++) {
        int temp;
        temp = sampleNoReplacement(items, count);
        
        int d = temp % 4;
        int c = (temp/4) % nC;
        int r = (temp/(4*nC));
        
        if(d==0 && r != 0 && r != (nR-1)) {
            if(maze[r+1][c]==0) {
                maze[r][c] = setBit(maze[r][c],0,0);
                maze[r+1][c] = setBit(maze[r][c],0,0);
            }
            else if(maze[r-1][c]==0) {
                maze[r][c] -= 2;
                maze[r-1][c] -= 2;
            }
        }
    }\

    printMaze(maze,nR,nC);
    
    cout << "Program ran." << endl;
    
    return 0;
}
