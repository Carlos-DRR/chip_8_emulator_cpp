#include "Stack.h"


Stack::Stack(){
    List<uint16_t>();
} 

void Stack::push(uint16_t info){
    List<uint16_t>::add(info);
}


uint16_t Stack::pop(){
    List<uint16_t>::removeLast();
}

void Stack::print(){
    List<uint16_t>::print();
}
