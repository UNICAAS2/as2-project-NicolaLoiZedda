#ifndef NODE_H
#define NODE_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

/*
 * a node is the single element of the dag vector, it represents each geometry element in the canvas
 * it holds the index of the element in the trapezoidal map
 * and a "type” attribute used in order to distinguish the type of element the node refers to
 */
class Node
{
public:

    // enum used for the node type
    enum nodeType {point_node, segment_node, trapezoid_node};

    // constructors
    Node(nodeType type, const size_t index);
    Node(nodeType type, const size_t index, const size_t leftChild, const size_t rightChild);

    // getter methods
    const nodeType& getType() const;
    size_t getIndex() const;
    size_t getLeftChild() const;
    size_t getRightChild() const;

    // setter methods
    void setType(nodeType type);
    void setIndex(const size_t index);
    void setLeftChild(const size_t leftChild);
    void setRightChild(const size_t rightChild);

private:

    nodeType type;
    size_t index;
    size_t leftChild;
    size_t rightChild;
};

#endif // NODE_H
