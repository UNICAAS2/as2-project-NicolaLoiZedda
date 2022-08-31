#include "node.h"

#include <data_structures/trapezoid.h>

Node::Node(nodeType type, const size_t index) :
    type(type),
    index(index)
{

}

Node::Node(nodeType type, const size_t index, const size_t leftChild, const size_t rightChild) :
    type(type),
    index(index),
    leftChild(leftChild),
    rightChild(rightChild)
{

}

const Node::nodeType& Node::getType() const
{
    return type;
}

size_t Node::getIndex() const
{
    return index;
}

size_t Node::getLeftChild() const
{
    return leftChild;
}

size_t Node::getRightChild() const
{
    return rightChild;
}

void Node::setType(nodeType type)
{
    this->type = type;
}

void Node::setIndex(const size_t index)
{
    this->index = index;
}

void Node::setLeftChild(const size_t leftChild)
{
    this->leftChild = leftChild;
}

void Node::setRightChild(const size_t rightChild)
{
    this->rightChild = rightChild;
}
