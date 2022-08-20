#include "directedacyclicgraph.h"

DirectedAcyclicGraph::DirectedAcyclicGraph()
{

}

const Node& DirectedAcyclicGraph::getNode(uint i) const
{
    return DAG[i];
}

const Node& DirectedAcyclicGraph::getRoot() const
{
    return DirectedAcyclicGraph::getNode(0);
}

void DirectedAcyclicGraph::addNode(Node& node)
{
    DAG.push_back(node);
}
