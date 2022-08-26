#include "directedacyclicgraph.h"

DirectedAcyclicGraph::DirectedAcyclicGraph()
{
    Node node = Node(Node::trapezoid_node, 0);
    dag.push_back(node);
}

const Node& DirectedAcyclicGraph::getNode(size_t index) const
{
    return dag[index];
}

const Node& DirectedAcyclicGraph::getRoot() const
{
    return getNode(0);
}

void DirectedAcyclicGraph::addNode(Node& node)
{
    dag.push_back(node);
}
