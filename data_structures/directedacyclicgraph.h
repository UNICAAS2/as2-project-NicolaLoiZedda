#ifndef DIRECTEDACYCLICGRAPH_H
#define DIRECTEDACYCLICGRAPH_H

#include <data_structures/node.h>

class DirectedAcyclicGraph
{
public:
    DirectedAcyclicGraph();

private:
    std::vector<Node> DAG;
};

#endif // DIRECTEDACYCLICGRAPH_H
