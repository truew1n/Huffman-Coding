#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <vector>
#include <stdexcept>
#include <cstdint>
#include "BinaryTree.hpp"

template <typename T>
class PriorityQueue {
private:
    std::vector<std::pair<int32_t, T>> Heap;

    void HeapifyUp(int32_t Index);
    void HeapifyDown(int32_t Index);

    int32_t Parent(int32_t I) const;
    int32_t LeftChild(int32_t I) const;
    int32_t RightChild(int32_t I) const;

public:
    bool Empty() const;
    int32_t Size() const;

    void Push(int32_t Priority, const T& Value);
    void Pop();

    T Top() const;
    int32_t TopPriority() const;
};

template class PriorityQueue<Node<char>*>;

#endif
