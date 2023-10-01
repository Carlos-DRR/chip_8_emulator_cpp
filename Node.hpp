#ifndef NODE_HPP
#define NODE_HPP
#include <iostream>

template <class T>
class Node{
    private:
        T info;
        Node *next;
    public:
        Node(T info);
        T getInfo();
        Node *getNextNode();
        void setNextNode(Node *next);
};

#endif