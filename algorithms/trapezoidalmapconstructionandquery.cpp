#include "trapezoidalmapconstructionandquery.h"

#include "data_structures/trapezoidalmap_dataset.h"

namespace TrapezoidalMapConstructionAndQuery
{
    void initializeStructures(TrapezoidalMap& tm, DirectedAcyclicGraph& dag)
    {
        Trapezoid trapezoid = Trapezoid();
        Node node = Node(Node::trapezoid_node, 0);
        tm.addTrapezoid(trapezoid);
        dag.addNode(node);
    }

    size_t getTrapezoidFromPoint(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, cg3::Point2d& queryPoint)
    {
        Node node = dag.getRoot();

        while (node.getType() != Node::trapezoid_node)
        {
            if (node.getType() == Node::point_node)
            {
                cg3::Point2d point = tm.getPointAtIndex(node.getIndex());

                if (point.x() < queryPoint.x())
                    node = dag.getNode(node.getLeftChild());
                else
                    node = dag.getNode(node.getRightChild());
            }
            else
            {
                cg3::Segment2d segment = tm.getSegmentAtIndex(node.getIndex());

                if (cg3::isPointAtLeft(segment, queryPoint))
                    node = dag.getNode(node.getLeftChild());
                else
                    node = dag.getNode(node.getRightChild());
            }
        }

        return node.getIndex();
    }

    const std::vector<size_t> followSegment(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        std::vector<size_t> intersectedTrapezoids;
        cg3::Point2d p = segment.p1();
        cg3::Point2d q = segment.p2();
        size_t i = 0;

        intersectedTrapezoids.push_back(getTrapezoidFromPoint(tm, dag, p));
        size_t index = intersectedTrapezoids[0];
        cg3::Point2d rightP = tm.getTrapezoidAtIndex(index).getRightPoint();

        while (rightP.x() <= q.x())
        {
            if (cg3::isPointAtLeft(segment, rightP))
                intersectedTrapezoids.push_back(tm.getTrapezoidAtIndex(intersectedTrapezoids[i]).getLowerRightNeighbor());
            else
                intersectedTrapezoids.push_back(tm.getTrapezoidAtIndex(intersectedTrapezoids[i]).getUpperRightNeighbor());

            rightP = tm.getTrapezoidAtIndex(intersectedTrapezoids[i+1]).getRightPoint();
            i++;
        }

        return intersectedTrapezoids;
    }

    void splitInFour(TrapezoidalMap &tm, DirectedAcyclicGraph &dag, const size_t trapezoidIndex, const cg3::Segment2d &segment)
    {
        // trapezoidal map split
        size_t leftEndpointIndex = tm.numberOfPoints();
        size_t rightEndpointIndex = leftEndpointIndex + 1;
        tm.addPoint(segment.p1());
        tm.addPoint(segment.p2());

        size_t segmentIndex = tm.numberOfSegments();
        tm.addSegment(segment);

        Trapezoid trapezoid = tm.getTrapezoidAtIndex(trapezoidIndex);
        Trapezoid topTrapezoid = Trapezoid();
        Trapezoid bottomTrapezoid = Trapezoid();
        Trapezoid leftTrapezoid = Trapezoid();
        Trapezoid rightTrapezoid = Trapezoid();

        topTrapezoid.setTop(trapezoid.getTop());
        topTrapezoid.setBottom(segment);
        topTrapezoid.setLeftPoint(segment.p1());
        topTrapezoid.setRightPoint(segment.p2());

        bottomTrapezoid.setTop(segment);
        bottomTrapezoid.setBottom(trapezoid.getBottom());
        bottomTrapezoid.setLeftPoint(trapezoid.getLeftPoint());
        bottomTrapezoid.setRightPoint(trapezoid.getRightPoint());

        leftTrapezoid.setTop(trapezoid.getTop());
        leftTrapezoid.setBottom(trapezoid.getBottom());
        leftTrapezoid.setLeftPoint(trapezoid.getLeftPoint());
        leftTrapezoid.setRightPoint(segment.p1());

        rightTrapezoid.setTop(trapezoid.getTop());
        rightTrapezoid.setBottom(trapezoid.getBottom());
        rightTrapezoid.setLeftPoint(segment.p2());
        rightTrapezoid.setRightPoint(trapezoid.getRightPoint());

        tm.addTrapezoidAtIndex(topTrapezoid, trapezoidIndex);
        tm.addTrapezoid(bottomTrapezoid);
        tm.addTrapezoid(leftTrapezoid);
        tm.addTrapezoid(rightTrapezoid);

        // dag split
        Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex);
        Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex);
        Node segmentNode = Node(Node::segment_node, segmentIndex);

        Node topTrapezoidNode = Node(Node::trapezoid_node, trapezoidIndex);
        Node bottomTrapezoidNode = Node(Node::trapezoid_node, trapezoidIndex + 1);
        Node leftTrapezoidNode = Node(Node::trapezoid_node, trapezoidIndex + 2);
        Node rightTrapezoidNode = Node(Node::trapezoid_node, trapezoidIndex + 3);

        size_t leftEndpointNodeIndex = dag.numberOfNodes() - 1;

        leftEndpointNode.setLeftChild(leftEndpointNodeIndex + 5);
        leftEndpointNode.setRightChild(leftEndpointNodeIndex + 1);
        rightEndpointNode.setLeftChild(leftEndpointNodeIndex + 2);
        rightEndpointNode.setRightChild(leftEndpointNodeIndex + 6);

        segmentNode.setLeftChild(leftEndpointNodeIndex + 3);
        segmentNode.setRightChild(leftEndpointNodeIndex + 4);

        dag.addNodeAtIndex(leftEndpointNode, leftEndpointNodeIndex);
        dag.addNode(rightEndpointNode);
        dag.addNode(segmentNode);
        dag.addNode(topTrapezoidNode);
        dag.addNode(bottomTrapezoidNode);
        dag.addNode(leftTrapezoidNode);
        dag.addNode(rightTrapezoidNode);
    }

    void incrementalStep(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        std::vector<size_t> intersectedTrapezoidsIndexes = followSegment(tm, dag, segment);

        if (intersectedTrapezoidsIndexes.size() == 1)
            splitInFour(tm, dag, intersectedTrapezoidsIndexes[0], segment);
        /*
        else
        {

        }
*/
        // to be continued

    }
}
