#ifndef LIST_HPP
#define LIST_HPP
#include "Node.hpp"

template <class T>

class List{
    protected:
        Node<T> *head;
        Node<T> *tail;
        int size;
    public:
        List();
        bool empty();
        int getSize();
        void add(T info);
        T remove(T info);
        T removeLast();
        void print();

};

#endif