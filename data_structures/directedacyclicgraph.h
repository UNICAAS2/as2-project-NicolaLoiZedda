#ifndef DIRECTEDACYCLICGRAPH_H
#define DIRECTEDACYCLICGRAPH_H

#include <data_structures/node.h>

class DirectedAcyclicGraph
{
public:
    DirectedAcyclicGraph();

    const Node& getNode(size_t index) const;
    const Node& getRoot() const;
    void addNode(Node& node);


private:
    std::vector<Node> dag;
};

#endif // DIRECTEDACYCLICGRAPH_H
