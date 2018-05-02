//
//  DisjointSet.h
//  10-Pathfinder
//
//  Created by Keith Smith on 5/1/18.
//  Copyright © 2018 Keith Smith. All rights reserved.
//

#ifndef DisjointSet_h
#define DisjointSet_h

class DisjointSet {
public:
    DisjointSet(int objectsTotal, int rows, int cols);
    
    void disjointSetUnion(int elementA, int elementB);
    int disjointSetFind(int element);
    
    void setElementsArr(int index, int value);
    
    int encode(int row, int col);
    int decodeRow(int n);
    int decodeColumn(int n);
private:
    int count;
    
    int nR;
    int nC;
    
    int elements[2500];
    int rank[2500];
};

#endif /* disjointSet_h */
