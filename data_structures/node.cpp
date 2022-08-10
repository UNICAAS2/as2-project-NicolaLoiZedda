#include "node.h"

#include <data_structures/trapezoid.h>

Node::Node()
{
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

Node::Node(nodeType type, const cg3::Point2d& xNode)
{
    this->type = type;
    this->xNode = xNode;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

Node::Node(nodeType type, const cg3::Segment2d& yNode)
{
    this->type = type;
    this->yNode = yNode;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

Node::Node(nodeType type, const Trapezoid& leaf)
{
    this->type = type;
    this->leaf = leaf;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

Node::Node(nodeType type, const cg3::Point2d& xNode, Node* const leftChild, Node* const rightChild)
{
    this->type = type;
    this->xNode = xNode;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}

Node::Node(nodeType type, const cg3::Segment2d& yNode, Node* const leftChild, Node* const rightChild)
{
    this->type = type;
    this->yNode = yNode;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}

Node::Node(nodeType type, const Trapezoid& leaf, Node* const leftChild, Node* const rightChild)
{
    this->type = type;
    this->leaf = leaf;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}

const Node::nodeType& Node::getType() const
{
    return type;
}

const cg3::Point2d& Node::getXNode() const
{
    return xNode;
}

const cg3::Segment2d& Node::getYNode() const
{
    return yNode;
}

const Trapezoid& Node::getLeaf() const
{
    return leaf;
}

const Node& Node::getLeftChild() const
{
    return *leftChild;
}

const Node& Node::getRightChild() const
{
    return *rightChild;
}

// setter methods
void Node::setType(nodeType type)
{
    this->type = type;
}

void Node::setXNode(const cg3::Point2d& xNode)
{
    this->xNode = xNode;
}

void Node::setYNode(const cg3::Segment2d& yNode)
{
    this->yNode = yNode;
}

void Node::setLeaf(const Trapezoid& leaf)
{
    this->leaf = leaf;
}

void Node::setLeftChild(Node* const leftChild)
{
    this->leftChild = leftChild;
}

void Node::setRightChild(Node* const rightChild)
{
    this->rightChild = rightChild;
}
