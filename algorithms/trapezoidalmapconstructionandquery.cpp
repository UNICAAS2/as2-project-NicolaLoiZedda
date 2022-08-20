#include "trapezoidalmapconstructionandquery.h"

namespace TrapezoidalMapConstructionAndQuery
{

    void initializeStructures(TrapezoidalMap& tm, DirectedAcyclicGraph& dag)
    {
        Trapezoid trapezoid = Trapezoid();
        Node node = Node(Node::leafnode, trapezoid);
        tm.addTrapezoid(trapezoid);
        dag.addNode(node);
    }

    const Trapezoid& searchPointInDAG(DirectedAcyclicGraph& dag, cg3::Point2d& queryPoint)
    {
        Node node = dag.getRoot();

        while (node.getType() != Node::leafnode)
        {
            if (node.getType() == Node::xnode)
            {
                if (node.getXNode().x() < queryPoint.x())
                    node = node.getLeftChild();
                else
                    node = node.getRightChild();
            }
            else
            {
                if (node.getType() == Node::ynode)
                {
                    if (cg3::isPointAtLeft(node.getYNode(), queryPoint))
                        node = node.getLeftChild();
                    else
                        node = node.getRightChild();
                }
            }
        }

        return node.getLeaf();
    }

    const std::vector<Trapezoid> followSegment(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        std::vector<Trapezoid> intersectedTrapezoids;
        cg3::Point2d p = segment.p1();
        cg3::Point2d q = segment.p2();
        uint i = 0;

        intersectedTrapezoids[0] = searchPointInDAG(dag, p);
        cg3::Point2d rightP = intersectedTrapezoids[0].getRightPoint();

        while(rightP.x() <= q.x())
        {
            if (cg3::isPointAtLeft(segment, rightP))
                intersectedTrapezoids[i+1] = intersectedTrapezoids[i].getLowerRightNeighbor();
            else
                intersectedTrapezoids[i+1] = intersectedTrapezoids[i].getUpperRightNeighbor();

            rightP = intersectedTrapezoids[i+1].getRightPoint();
            i++;
        }

        return intersectedTrapezoids;
    }

    void buildTrapezoidalMap(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, cg3::Segment2d& segment)
    {
        std::vector<Trapezoid> intersectedTrapezoids = followSegment(tm, dag, segment);

        if (intersectedTrapezoids.size() == 1)
        {
            tm.splitInFour(intersectedTrapezoids[0], segment);
            //dag.splitLeaf()
        }
/*
        else
        {

        }
*/
        // to be continued

    }
}
