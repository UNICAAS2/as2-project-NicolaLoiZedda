#ifndef DIRECTEDACYCLICGRAPH_H
#define DIRECTEDACYCLICGRAPH_H

#include <data_structures/node.h>

class DirectedAcyclicGraph
{
public:
    DirectedAcyclicGraph();

    const Node& getNode(uint i) const;
    void insertNode(Node& node);


private:
    std::vector<Node> DAG;
};

#endif // DIRECTEDACYCLICGRAPH_H
