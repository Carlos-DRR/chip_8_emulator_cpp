#include "List.h"
#include <iostream>

template <class T>
List<T>::List(){
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template <class T>
bool List<T>::empty(){
    return size == 0;
}

template <class T>
int List<T>::getSize(){
    return this->size;
}

template <class T>
void List<T>::add(T info){
    Node<T> * node = new Node<T>(info); //constructor set next to null
    if(empty()){
        head = node;
        tail = node;
    }else{
        tail->setNextNode(node);
        tail = node;
    }
    size += 1;
}

template <class T>
T List<T>::remove(T info){
    if(!empty()){
        Node<T> *current = head;
        Node<T> *previous = nullptr;
        while(current != nullptr && current->getInfo() != info){
            previous = current;
            current = current->getNextNode();
        }
        if(current != nullptr){
            if(current == head){
                head = current->getNextNode();
            }else{
                previous->setNextNode(current->getNextNode());
            }
            if(current == tail){
                tail = previous;
            }
            delete current;
            size -= 1;
        }else throw 1; //"Element not found exception"

    }else throw 2; // "Empty list exception"
}

template <class T>
T List<T>::removeLast(){
    if(!empty()){
        Node<T> *previous = nullptr;
        Node<T> *current = head;
        while(current->getNextNode() != nullptr){
            previous = current;
            current = current->getNextNode();
        }
        T info = current->getInfo();
        if(current == head){
            head = nullptr;
        }else{
            previous->setNextNode(current->getNextNode());
        }
        tail = previous;
        delete current;
        size -= 1;
        return info;
        
    }else throw 2;
}

template <class T>
void List<T>::print(){
    Node<T> * current = head;
    while(current != nullptr){
        std::cout << current->getInfo() << std::endl;
        current = current->getNextNode();
    }
}

template class List<double>;
template class List<float>;
template class List<int>;
template class List<uint8_t>;
template class List<uint16_t>;