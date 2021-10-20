//
// Created by Artem on 05.10.2021.
//
#include "priority_queue.h"

#include <memory>

template<class T>
PriorityQueue<T>::PriorityQueue() : size_(0) {};

template<class T>
bool PriorityQueue<T>::empty() {
    return size_ == 0;
}

template<class T>
size_t PriorityQueue<T>::size() {
    return size_;
}

template<class T>
void PriorityQueue<T>::push(T a) {
    if (size_ < heap_.size()) {
        heap_[size_++] = a;
    } else {
        heap_.push_back(a);
        ++size_;
    }
    size_t i = size_ - 1;
    while (i > 0) {
        if (heap_[(i - 1) / 2] < heap_[i]) {
            std::swap(heap_[(i - 1) / 2], heap_[i]);
            i = (i - 1) / 2;
        } else {
            break;
        }
    }
}

template<class T>
T PriorityQueue<T>::top() {
    return heap_[0];
}

template<class T>
void PriorityQueue<T>::pop() {
    std::swap(heap_[0], heap_[size_ - 1]);
    --size_;
    size_t i = 0;
    while (true) {
        if (2 * i + 2 < size_) {
            if (heap_[i] < heap_[2 * i + 1] || heap_[i] < heap_[2 * i + 2]) {
                if (heap_[2 * i + 1] < heap_[2 * i + 2]) {
                    std::swap(heap_[i], heap_[2 * i + 2]);
                    i = 2 * i + 2;
                } else {
                    std::swap(heap_[i], heap_[2 * i + 1]);
                    i = 2 * i + 1;
                }
            } else {
                break;
            }
        } else if (2 * i + 1 < size_) {
            if (heap_[i] < heap_[2 * i + 1]) {
                std::swap(heap_[i], heap_[2 * i + 1]);
                i = 2 * i + 1;
            } else {
                break;
            }
        } else {
            break;
        }
    }
}

template PriorityQueue<std::shared_ptr<TrieNode>>::PriorityQueue();
template void PriorityQueue<std::shared_ptr<TrieNode>>::push(std::shared_ptr<TrieNode> a);
template size_t PriorityQueue<std::shared_ptr<TrieNode>>::size();
template bool PriorityQueue<std::shared_ptr<TrieNode>>::empty();
template std::shared_ptr<TrieNode> PriorityQueue<std::shared_ptr<TrieNode>>::top();
template void PriorityQueue<std::shared_ptr<TrieNode>>::pop();

template PriorityQueue<int>::PriorityQueue();
template void PriorityQueue<int>::push(int a);
template size_t PriorityQueue<int>::size();
template bool PriorityQueue<int>::empty();
template int PriorityQueue<int>::top();
template void PriorityQueue<int>::pop();
