#ifndef DIRECTEDACYCLICGRAPH_H
#define DIRECTEDACYCLICGRAPH_H

#include <data_structures/node.h>

class DirectedAcyclicGraph
{
public:

    DirectedAcyclicGraph();

    const Node& getNode(const size_t index) const;
    Node& getNodeRef(const size_t index);
    const Node& getRoot() const;
    size_t numberOfNodes() const;

    void addNode(const Node& node);
    void addNodeAtIndex(const Node& node, const size_t index);
    void clear();


private:

    std::vector<Node> dag;
};

#endif // DIRECTEDACYCLICGRAPH_H
