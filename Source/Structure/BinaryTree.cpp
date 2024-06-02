#include "BinaryTree.hpp"

template<typename T>
Node<T> *BinaryTree<T>::GetRoot() {
    return Root;
}

template<typename T>
void BinaryTree<T>::SetRoot(Node<T> *NewRoot) {
    *Root = NewRoot;
}