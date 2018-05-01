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

u_int8_t decodeColumn(int coded, int nC) {
    return (coded/4) % nC;
}

u_int8_t decodeRow(int coded, int nC) {
    return (coded/(4*nC));
}

void generateMaze(uint8_t mazePrime[], int nR, int nC) {
    int count = (nR*nC*4);
    
    int items[count];
    
    for(int i = 0; i < count; i++) {
        items[i] = i;
    }
    
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
    /*
    DisjointSet disjointSet = *new DisjointSet(nR*nC, nR, nC);

    i = 0;

    int numItems = nR*nC*4;

    while(i < (nR*nC-1)) {
        int e;

        u_int8_t room1;
        u_int8_t room2;

        int r;
        int c;
        int d;

        do {
            do {
                if(numItems == 0) {
                    e = 0;
                    break;
                }

                e = sampleNoReplacement(items, numItems);
                cout << "Sampling" << endl;
            } while((decodeRow(e, nC) == 0 && isBitSet(decodeDirection(e),0)) ||
                    (decodeRow(e, nC) == nR-1 && isBitSet(decodeDirection(e),2)) ||
                    (decodeColumn(e, nC) == 0 && isBitSet(decodeDirection(e),3)) ||
                    (decodeColumn(e, nC) == nC-1 && isBitSet(decodeDirection(e),1)));

            if(numItems == 0) {
                break;
            }

            cout << "Room found" << endl;

            r = decodeRow(e, nC);
            c = decodeColumn(e, nC);
            d = decodeDirection(e);
            
            cout << "Room decoded" << endl;

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
            
            cout << "Rooms encoded" << endl;

        } while(disjointSet.disjointSetFind(room1) == disjointSet.disjointSetFind(room2));

        cout << "Room union" << endl;
        disjointSet.disjointSetUnion(room1, room2);
        i++;

        room1 = mazePrime[encode(decodeRow(room1, nC), nR, decodeColumn(room1, nC), nC, 0)];
        room2 = mazePrime[encode(decodeRow(room2, nC), nR, decodeColumn(room2, nC), nC, 0)];

        if(decodeRow(room1, nC) > decodeRow(room2, nC) && decodeColumn(room1, nC) == decodeColumn(room2, nC)) {
            room1 = setBit(room1, 2, 0);
            room2 = setBit(room2, 0, 0);
        }
        else if(decodeRow(room1, nC) < decodeRow(room2, nC) && decodeColumn(room1, nC) == decodeColumn(room2, nC)) {
            room1 = setBit(room1, 0, 0);
            room2 = setBit(room2, 2, 0);
        }
        else if(decodeRow(room1, nC) == decodeRow(room2, nC) && decodeColumn(room1, nC) > decodeColumn(room2, nC)) {
            room1 = setBit(room1, 3, 0);
            room2 = setBit(room2, 1, 0);
        }
        else if(decodeRow(room1, nC) == decodeRow(room2, nC) && decodeColumn(room1, nC) < decodeColumn(room2, nC)) {
            room1 = setBit(room1, 1, 0);
            room2 = setBit(room2, 3, 0);
        }
        else {
            cout << "ERROR REMOVING WALL" << endl;
        }

        mazePrime[encode(decodeRow(room1, nC), nR, decodeColumn(room1, nC), nC, 0)] = room1;
        mazePrime[encode(decodeRow(room2, nC), nR, decodeColumn(room2, nC), nC, 0)] = room2;

        cout << "wall removed" << endl;
    }*/
}

void copyMaze(uint8_t mazePrime[], uint8_t maze[][MAX_COLS], int nR, int nC) {
    for(int r = 0; r < nR; r++) {
        for(int c = 0; c < nC; c++) {
            maze[r][c] = mazePrime[encode(r,nR,c,nC,0)];
        }
    }
}

int main(int argc, const char * argv[]) {
    int nR = atoi(argv[1]);
    int nC = atoi(argv[2]);
    
    int count = nR*nC*4;
    
    uint8_t mazePrime[count];
    uint8_t maze[MAX_ROWS][MAX_COLS];
    
    cout << "generate maze" << endl;
    
    generateMaze(mazePrime,nR,nC);
    
    copyMaze(mazePrime, maze, nR, nC);

    printMaze(maze,nR,nC);
    
    cout << "Program ran." << endl;
    
    return 0;
}
