#include "node.h"

#include <data_structures/trapezoid.h>

Node::Node()
{

}

const Node::nodeType& Node::getType() const
{
    return type;
}

const cg3::Point2d& Node::getXNode() const
{
    return *xNode;
}

const cg3::Segment2d& Node::getYNode() const
{
    return *yNode;
}

const Trapezoid& Node::getLeaf() const
{
    return *leaf;
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
void setType(Node::nodeType type)
{
    type = type;
}

void setXNode(const cg3::Point2d* xNode)
{
    xNode = xNode;
}

void setYNode(const cg3::Segment2d* yNode)
{
    yNode = yNode;
}

void setLeaf(Trapezoid* leaf)
{
    leaf = leaf;
}

void setLeftChild(const Node* leftChild)
{
    leftChild = leftChild;
}

void setRightChild(const Node* rightChild)
{
    rightChild = rightChild;
}
