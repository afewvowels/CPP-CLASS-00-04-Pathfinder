//
//  DisjointSet.cpp
//  10-Pathfinder
//
//  Created by Keith Smith on 5/1/18.
//  Copyright © 2018 Keith Smith. All rights reserved.
//

#include <stdio.h>
#include "DisjointSet.h"

DisjointSet::DisjointSet(int objectsTotal, int rows, int cols) {
    count = objectsTotal;
    
    nR = rows;
    nC = cols;
    
    for(int i = 0; i < count; i++) {
        elements[i] = i;
    }
    
    for(int i = 0; i < count; i++) {
        rank[i] = 0;
    }
}

void DisjointSet::disjointSetUnion(int elementA, int elementB) {
    elementA = disjointSetFind(elementA);
    elementB = disjointSetFind(elementB);
    
    if(elementA != elementB) {
        if(rank[elementA] < rank[elementB]) {
            elements[elementA] = elementB;
        }
        else if(rank[elementA] > rank[elementB]) {
            elements[elementB] = elementA;
        }
        else {
            rank[elementA] = rank[elementA] + 1;
            elements[elementB] = elementA;
        }
    }
}

int DisjointSet::disjointSetFind(int element) {
    if(elements[element] == element) {
        return element;
    }
    else {
        elements[element] = disjointSetFind(elements[element]);
        return elements[element];
    }
}

void DisjointSet::setElementsArr(int index, int value) {
    elements[index] = value;
}

int DisjointSet::encode(int row, int col) {
    return (row * nC + col);
}

int DisjointSet::decodeRow(int n) {
    return (n / nC);
}

int DisjointSet::decodeColumn(int n) {
    return (n % nC);
}
