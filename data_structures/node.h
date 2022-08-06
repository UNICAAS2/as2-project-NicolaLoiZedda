#ifndef NODE_H
#define NODE_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

#include <data_structures/trapezoid.h>

class Node
{
public:
    Node();

    enum nodeType {xnode, ynode, leafnode};

    // getter methods
    const nodeType& getType() const;
    const cg3::Point2d& getXNode() const;
    const cg3::Segment2d& getYNode() const;
    const Trapezoid& getLeaf() const;
    const Node& getLeftChild() const;
    const Node& getRightChild() const;

    // setter methods
    void setType(Node::nodeType type);
    void setXNode(const cg3::Point2d* xNode);
    void setYNode(const cg3::Segment2d* yNode);
    void setLeaf(Trapezoid* leaf);
    void setLeftChild(const Node* leftChild);
    void setRightChild(const Node* rightChild);

private:
    nodeType type;
    cg3::Point2d* xNode;
    cg3::Segment2d* yNode;
    Trapezoid* leaf;
    Node* leftChild;
    Node* rightChild;
};

#endif // NODE_H
