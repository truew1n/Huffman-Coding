#include "PriorityQueue.hpp"

template <typename T>
void PriorityQueue<T>::HeapifyUp(int32_t Index) {
    while (Index > 0 && Heap[Parent(Index)].first > Heap[Index].first) {
        std::swap(Heap[Parent(Index)], Heap[Index]);
        Index = Parent(Index);
    }
}

template <typename T>
void PriorityQueue<T>::HeapifyDown(int32_t Index) {
    int32_t MinIndex = Index;
    int32_t Left = LeftChild(Index);

    if (Left < Size() && Heap[Left].first < Heap[MinIndex].first) {
        MinIndex = Left;
    }

    int32_t Right = RightChild(Index);

    if (Right < Size() && Heap[Right].first < Heap[MinIndex].first) {
        MinIndex = Right;
    }

    if (Index != MinIndex) {
        std::swap(Heap[Index], Heap[MinIndex]);
        HeapifyDown(MinIndex);
    }
}

template <typename T>
int PriorityQueue<T>::Parent(int I) const {
    return (I - 1) / 2;
}

template <typename T>
int PriorityQueue<T>::LeftChild(int I) const {
    return (2 * I + 1);
}

template <typename T>
int PriorityQueue<T>::RightChild(int I) const {
    return (2 * I + 2);
}

template <typename T>
bool PriorityQueue<T>::Empty() const {
    return Heap.empty();
}

template <typename T>
int PriorityQueue<T>::Size() const {
    return Heap.size();
}

template <typename T>
void PriorityQueue<T>::Push(int32_t Priority, const T& Value) {
    Heap.push_back(std::make_pair(Priority, Value));
    HeapifyUp(Size() - 1);
}

template <typename T>
void PriorityQueue<T>::Pop() {
    if (Empty()) {
        throw std::out_of_range("Priority queue is empty");
    }

    Heap[0] = Heap.back();
    Heap.pop_back();
    HeapifyDown(0);
}

template <typename T>
T PriorityQueue<T>::Top() const {
    if (Empty()) {
        throw std::out_of_range("Priority queue is empty");
    }

    return Heap.front().second;
}

template <typename T>
int32_t PriorityQueue<T>::TopPriority() const {
    if (Empty()) {
        throw std::out_of_range("Priority queue is empty");
    }

    return Heap.front().first;
}
