#include "directedacyclicgraph.h"

DirectedAcyclicGraph::DirectedAcyclicGraph()
{

}

const Node& DirectedAcyclicGraph::getNode(uint i) const
{
    return DAG[i];
}

void DirectedAcyclicGraph::insertNode(Node& node)
{
    DAG.push_back(node);
}
