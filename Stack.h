#ifndef STACK_H
#define STACK_H
#include "List.h"
#include <cstdint>


class Stack : public List<uint16_t>{
    public:
        Stack();
        void push(uint16_t info);
        uint16_t pop();
        void print();
        uint16_t top();
};

#endif