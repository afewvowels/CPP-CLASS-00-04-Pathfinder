#ifndef _STACK_H
#define _STACK_H

#include <iostream>
#include "exceptions.h"
using namespace std;

const int INITIAL_STACK_SIZE = 16;

template <class StackType>
class Stack {
public:
  Stack(void) {

    top = 0;

    cap = INITIAL_STACK_SIZE;

    data = new StackType[cap];
  }

  ~Stack(void) {

    delete[] data;
  }

  bool isEmpty(void) { return top == 0; }
  int size(void) { return top; }

  void clear(void) { top = 0; }

  void push(StackType val) {

    // if no space left, do the following:
    if (top == cap) {
      // make a new array twice as large
      StackType *tmpData = new StackType[2*cap];

      // copy data from old array to new array
      for (int i=0;i<top;i++)
        tmpData[i] = data[i];

      // delete the old array and point to the new one
      delete[] data;

      data = tmpData;

      // update the capacity
      cap *= 2;
    }

    // normal stack push
    data[top] = val;

    top++;
  }

  StackType pop(void) throw (ContainerEmptyException) {

    if (top == 0)
      throw ContainerEmptyException();

    top--;

    return data[top];
  }

  StackType peek(void) throw (ContainerEmptyException) {

    if (top == 0)
      throw ContainerEmptyException();

    return data[top-1];
  }


private:
  StackType
    *data;
  int
    top,
    cap;
};

#endif
