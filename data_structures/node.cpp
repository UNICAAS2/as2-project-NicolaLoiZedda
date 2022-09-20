#include "node.h"

#include <data_structures/trapezoid.h>

/**
 * @brief Node::Node node constructor
 * @param type type of element the node refers to
 * @param index index in the map of the element the node refers to
 */
Node::Node(nodeType type, const size_t index) :
    type(type),
    index(index),
    leftChild(std::numeric_limits<size_t>::max()),
    rightChild(std::numeric_limits<size_t>::max())
{

}

/**
 * @brief Node::Node node constructor
 * @param type type of element the node refers to
 * @param index index in the map of the element the node refers to
 * @param leftChild index in the dag of the left child of the node
 * @param rightChild index in the dag of the right child of the node
 */
Node::Node(nodeType type, const size_t index, const size_t leftChild, const size_t rightChild) :
    type(type),
    index(index),
    leftChild(leftChild),
    rightChild(rightChild)
{

}

/**
 * @brief Node::getType gets type of the node
 * @return the type of the node
 */
const Node::nodeType& Node::getType() const
{
    return type;
}

/**
 * @brief Node::getIndex gets index stored in the node
 * @return the index stored in the node
 */
size_t Node::getIndex() const
{
    return index;
}

/**
 * @brief Node::getLeftChild gets index of the left child of the node
 * @return the index in the dag of the left child of the node
 */
size_t Node::getLeftChild() const
{
    return leftChild;
}

/**
 * @brief Node::getRightChild gets index of the right child of the node
 * @return the index in the dag of the right child of the node
 */
size_t Node::getRightChild() const
{
    return rightChild;
}

/**
 * @brief Node::setType sets type
 * @param type type
 */
void Node::setType(nodeType type)
{
    this->type = type;
}

/**
 * @brief Node::setIndex sets index
 * @param index index
 */
void Node::setIndex(const size_t index)
{
    this->index = index;
}

/**
 * @brief Node::setLeftChild sets left child
 * @param leftChild index in the dag of the left child of the node
 */
void Node::setLeftChild(const size_t leftChild)
{
    this->leftChild = leftChild;
}

/**
 * @brief Node::setRightChild sets right child
 * @param rightChild index in the dag of the right child of the node
 */
void Node::setRightChild(const size_t rightChild)
{
    this->rightChild = rightChild;
}
