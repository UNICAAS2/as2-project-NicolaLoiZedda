#ifndef DIRECTEDACYCLICGRAPH_H
#define DIRECTEDACYCLICGRAPH_H

#include <data_structures/node.h>

class DirectedAcyclicGraph
{
public:

    DirectedAcyclicGraph();

    const Node& getNode(size_t index) const;
    const Node& getRoot() const;
    size_t numberOfNodes() const;
    void addNode(Node& node);
    void addNodeAtIndex(Node& node, size_t index);
    void clear();


private:

    std::vector<Node> dag;
};

#endif // DIRECTEDACYCLICGRAPH_H
