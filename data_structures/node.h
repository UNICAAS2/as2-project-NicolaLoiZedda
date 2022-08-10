#ifndef NODE_H
#define NODE_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

#include <data_structures/trapezoid.h>

class Node
{
public:
    enum nodeType {xnode, ynode, leafnode};

    Node();
    Node(nodeType type, const cg3::Point2d& xNode);
    Node(nodeType type, const cg3::Segment2d& yNode);
    Node(nodeType type, const Trapezoid& leaf);
    Node(nodeType type, const cg3::Point2d& xNode, Node* const leftChild, Node* const rightChild);
    Node(nodeType type, const cg3::Segment2d& yNode, Node* const leftChild, Node* const rightChild);
    Node(nodeType type, const Trapezoid& leaf, Node* const leftChild, Node* const rightChild);

    // getter methods
    const nodeType& getType() const;
    const cg3::Point2d& getXNode() const;
    const cg3::Segment2d& getYNode() const;
    const Trapezoid& getLeaf() const;
    const Node& getLeftChild() const;
    const Node& getRightChild() const;

    // setter methods
    void setType(nodeType type);
    void setXNode(const cg3::Point2d& xNode);
    void setYNode(const cg3::Segment2d& yNode);
    void setLeaf(const Trapezoid& leaf);
    void setLeftChild(Node* const leftChild);
    void setRightChild(Node* const rightChild);

private:
    nodeType type;
    cg3::Point2d xNode;
    cg3::Segment2d yNode;
    Trapezoid leaf;
    Node* leftChild;
    Node* rightChild;
};

#endif // NODE_H
