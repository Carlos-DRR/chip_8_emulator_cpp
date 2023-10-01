#ifndef STACK_HPP
#define STACK_HPP
#include "List.hpp"
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