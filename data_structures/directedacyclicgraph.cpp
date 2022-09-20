#include "directedacyclicgraph.h"

/**
 * @brief DirectedAcyclicGraph::DirectedAcyclicGraph dag constructor
 */
DirectedAcyclicGraph::DirectedAcyclicGraph()
{
    Node node = Node(Node::trapezoid_node, 0);
    dag.push_back(node);
}

/**
 * @brief DirectedAcyclicGraph::getNode gets node at index
 * @param index index of the node
 * @return node at the index
 */
const Node& DirectedAcyclicGraph::getNode(const size_t index) const
{
    return dag[index];
}

/**
 * @brief DirectedAcyclicGraph::getNodeRef gets reference to the node at index
 * @param index index of the node
 * @return node at the index
 */
Node& DirectedAcyclicGraph::getNodeRef(const size_t index)
{
    return dag[index];
}

/**
 * @brief DirectedAcyclicGraph::getRoot gets node at the root of the dag
 * @return node at the root
 */
const Node& DirectedAcyclicGraph::getRoot() const
{
    return getNode(0);
}

/**
 * @brief DirectedAcyclicGraph::numberOfNodes gets the number of nodes present in the dag
 * @return number of nodes present in the dag
 */
size_t DirectedAcyclicGraph::numberOfNodes() const
{
    return dag.size();
}

/**
 * @brief DirectedAcyclicGraph::addNode adds node at the back of the dag
 * @param node node to be added
 */
void DirectedAcyclicGraph::addNode(const Node& node)
{
    dag.push_back(node);
}

/**
 * @brief DirectedAcyclicGraph::addNodeAtIndex adds node to the dag at a given index
 * @param node node to be added
 * @param index index where the node will be inserted
 */
void DirectedAcyclicGraph::addNodeAtIndex(const Node& node, const size_t index)
{
    if (index == dag.size())
        dag.push_back(node);
    else
        dag[index] = node;
}

/**
 * @brief DirectedAcyclicGraph::clear clears all data in the dag
 */
void DirectedAcyclicGraph::clear()
{
    dag.clear();
}
