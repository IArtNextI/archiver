//
// Created by Artem on 05.10.2021.
//

#pragma once

#include <vector>
#include "../trie/trie.h"

template <class T>
class PriorityQueue {
private:
    std::vector<T> heap_;
    size_t size_;

public:
    PriorityQueue();
    bool empty();
    size_t size();
    void push(T a);
    T top();
    void pop();
};
