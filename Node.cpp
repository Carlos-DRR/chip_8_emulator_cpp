#include "Node.hpp"

template <class T>
Node<T>::Node(T info){
    this->info = info;
    this->next = nullptr;
}

template <class T>
T Node<T>::getInfo(){
    return this->info;
}

template <class T>
Node<T>* Node<T>::getNextNode(){
    return this->next;
}

template <class T>
void Node<T>::setNextNode(Node *next){
    this->next = next;
}

template class Node<double>;
template class Node<float>;
template class Node<int>;
template class Node<uint8_t>;
template class Node<uint16_t>;
