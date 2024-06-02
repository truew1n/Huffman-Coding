#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <iostream>
#include <cstdint>

template<typename T>
class Node {
public:
    T Data;
    Node* Left;
    Node* Right;
    Node(const T& Value) : Data(Value), Left(nullptr), Right(nullptr) {}
};

template class Node<char>;

template <typename T>
class BinaryTree {
private:
    Node<T>* Root;

public:
    BinaryTree() : Root(nullptr) {}

    Node<T>* GetRoot();
    void SetRoot(Node<T>* NewRoot);
};

template class BinaryTree<char>;

// Implementations
template <typename T>
Node<T>* BinaryTree<T>::GetRoot() {
    return Root;
}

template <typename T>
void BinaryTree<T>::SetRoot(Node<T>* NewRoot) {
    Root = NewRoot;
}

#endif
