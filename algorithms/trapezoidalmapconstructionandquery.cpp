#include "trapezoidalmapconstructionandquery.h"

#include "data_structures/trapezoidalmap_dataset.h"

#include <algorithms/geometryfunctions.h>

namespace TrapezoidalMapConstructionAndQuery
{
    size_t getTrapezoidFromPoint(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, cg3::Point2d& queryPoint)
    {
        Node node = dag.getRoot();

        while (node.getType() != Node::trapezoid_node)
        {
            if (node.getType() == Node::point_node)
            {
                cg3::Point2d point = tm.getPointAtIndex(node.getIndex());

                if (point.x() > queryPoint.x())
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

        intersectedTrapezoids.push_back(getTrapezoidFromPoint(tm, dag, p));
        size_t index = intersectedTrapezoids[0];
        cg3::Point2d rightP = tm.getTrapezoidAtIndex(index).getRightPoint();

        size_t i = 0;
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

    void splitTrapezoid(TrapezoidalMap &tm, DirectedAcyclicGraph &dag, const size_t trapezoidIndex, const cg3::Segment2d &segment)
    {
        // trapezoidal map split
        size_t leftEndpointIndex = tm.numberOfPoints();
        size_t rightEndpointIndex = leftEndpointIndex + 1;
        tm.addPoint(segment.p1());
        tm.addPoint(segment.p2());

        size_t segmentIndex = tm.numberOfSegments();
        tm.addSegment(segment);

        bool leftExists = false;
        bool rightExists = false;

        Trapezoid trapezoid = tm.getTrapezoidAtIndex(trapezoidIndex);
        if (segment.p1().x() > trapezoid.getLeftPoint().x())
            leftExists = true;
        if (segment.p2().x() < trapezoid.getRightPoint().x())
            rightExists = true;

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
        bottomTrapezoid.setLeftPoint(segment.p1());
        bottomTrapezoid.setRightPoint(segment.p2());

        if (leftExists)
        {
            leftTrapezoid.setTop(trapezoid.getTop());
            leftTrapezoid.setBottom(trapezoid.getBottom());
            leftTrapezoid.setLeftPoint(trapezoid.getLeftPoint());
            leftTrapezoid.setRightPoint(segment.p1());

            leftTrapezoid.setUpperRightNeighbor(trapezoidIndex);
            leftTrapezoid.setLowerRightNeighbor(trapezoidIndex + 1);

            topTrapezoid.setUpperLeftNeighbor(trapezoidIndex + 2);
            bottomTrapezoid.setLowerLeftNeighbor(trapezoidIndex + 2);
        }

        if (rightExists)
        {
            rightTrapezoid.setTop(trapezoid.getTop());
            rightTrapezoid.setBottom(trapezoid.getBottom());
            rightTrapezoid.setLeftPoint(segment.p2());
            rightTrapezoid.setRightPoint(trapezoid.getRightPoint());

            rightTrapezoid.setUpperLeftNeighbor(trapezoidIndex);
            rightTrapezoid.setLowerLeftNeighbor(trapezoidIndex + 1);

            topTrapezoid.setUpperRightNeighbor(trapezoidIndex + 3);
            bottomTrapezoid.setLowerRightNeighbor(trapezoidIndex + 3);
        }

        tm.addTrapezoidAtIndex(topTrapezoid, trapezoidIndex);
        tm.addTrapezoid(bottomTrapezoid);
        if (leftExists)
            tm.addTrapezoid(leftTrapezoid);
        if (rightExists)
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

        if (leftExists)
            leftEndpointNode.setLeftChild(leftEndpointNodeIndex + 5);
        leftEndpointNode.setRightChild(leftEndpointNodeIndex + 1);
        rightEndpointNode.setLeftChild(leftEndpointNodeIndex + 2);
        if (rightExists)
            rightEndpointNode.setRightChild(leftEndpointNodeIndex + 6);

        segmentNode.setLeftChild(leftEndpointNodeIndex + 3);
        segmentNode.setRightChild(leftEndpointNodeIndex + 4);

        dag.addNodeAtIndex(leftEndpointNode, leftEndpointNodeIndex);
        dag.addNode(rightEndpointNode);
        dag.addNode(segmentNode);
        dag.addNode(topTrapezoidNode);
        dag.addNode(bottomTrapezoidNode);
        if (leftExists)
            dag.addNode(leftTrapezoidNode);
        if (rightExists)
            dag.addNode(rightTrapezoidNode);
    }

    void incrementalStep(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        const cg3::Segment2d orderedSegment = GeometryFunctions::getOrderedSegment(segment);

        std::vector<size_t> intersectedTrapezoidsIndexes = followSegment(tm, dag, orderedSegment);

        if (intersectedTrapezoidsIndexes.size() == 1)
        {
            std::cout << " segment completely inside one trapezoid " << std::endl;
            splitTrapezoid(tm, dag, intersectedTrapezoidsIndexes[0], orderedSegment);
        }
        else
        {
            std::cout << " split in case of " << intersectedTrapezoidsIndexes.size() << " not implemented" << std::endl;
        }
        // to be continued

    }

    void clearStructures(TrapezoidalMap& tm, DirectedAcyclicGraph& dag)
    {
        tm.clear();
        dag.clear();
    }
}
