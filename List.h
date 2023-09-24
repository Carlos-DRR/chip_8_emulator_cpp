#ifndef LIST_H
#define LIST_H
#include "Node.h"

template <class T>

class List{
    private:
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