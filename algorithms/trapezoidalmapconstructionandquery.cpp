#include "trapezoidalmapconstructionandquery.h"

#include "data_structures/trapezoidalmap_dataset.h"

#include <algorithms/geometryfunctions.h>

namespace TrapezoidalMapConstructionAndQuery
{
    size_t getTrapezoidFromPoint(const TrapezoidalMap& tm, const DirectedAcyclicGraph& dag, const cg3::Point2d& queryPoint)
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

    size_t trapezoidIndexToNodeIndex(const TrapezoidalMap& tm, const size_t index)
    {
        return tm.getTrapezoidAtIndex(index).getNodeIndex();
    }

    size_t nodeIndexToTrapezoidIndex(const DirectedAcyclicGraph& dag, const size_t index)
    {
        return dag.getNode(index).getIndex();
    }

    const std::vector<size_t> followSegment(const TrapezoidalMap& tm, const DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
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

        Trapezoid oldTrapezoid = tm.getTrapezoidAtIndex(trapezoidIndex);

        if (segment.p1().x() > oldTrapezoid.getLeftPoint().x())
            leftExists = true;
        if (segment.p2().x() < oldTrapezoid.getRightPoint().x())
            rightExists = true;

        size_t bottomTrapezoidIndex = tm.numberOfTrapezoids();

        Trapezoid topTrapezoid = Trapezoid();
        Trapezoid bottomTrapezoid = Trapezoid();
        Trapezoid leftTrapezoid = Trapezoid();
        Trapezoid rightTrapezoid = Trapezoid();

        topTrapezoid.setTop(oldTrapezoid.getTop());
        topTrapezoid.setBottom(segment);
        topTrapezoid.setLeftPoint(segment.p1());
        topTrapezoid.setRightPoint(segment.p2());

        bottomTrapezoid.setTop(segment);
        bottomTrapezoid.setBottom(oldTrapezoid.getBottom());
        bottomTrapezoid.setLeftPoint(segment.p1());
        bottomTrapezoid.setRightPoint(segment.p2());

        if (leftExists)
        {
            leftTrapezoid.setTop(oldTrapezoid.getTop());
            leftTrapezoid.setBottom(oldTrapezoid.getBottom());
            leftTrapezoid.setLeftPoint(oldTrapezoid.getLeftPoint());
            leftTrapezoid.setRightPoint(segment.p1());

            leftTrapezoid.setUpperRightNeighbor(trapezoidIndex);
            leftTrapezoid.setLowerRightNeighbor(bottomTrapezoidIndex);

            topTrapezoid.setUpperLeftNeighbor(bottomTrapezoidIndex + 1);
            bottomTrapezoid.setLowerLeftNeighbor(bottomTrapezoidIndex + 1);
        }

        if (rightExists)
        {
            rightTrapezoid.setTop(oldTrapezoid.getTop());
            rightTrapezoid.setBottom(oldTrapezoid.getBottom());
            rightTrapezoid.setLeftPoint(segment.p2());
            rightTrapezoid.setRightPoint(oldTrapezoid.getRightPoint());

            rightTrapezoid.setUpperLeftNeighbor(trapezoidIndex);
            rightTrapezoid.setLowerLeftNeighbor(bottomTrapezoidIndex);

            topTrapezoid.setUpperRightNeighbor(bottomTrapezoidIndex + 2);
            bottomTrapezoid.setLowerRightNeighbor(bottomTrapezoidIndex + 2);
        }

        tm.addTrapezoidAtIndex(topTrapezoid, trapezoidIndex);
        tm.addTrapezoid(bottomTrapezoid);
        if (leftExists)
            tm.addTrapezoid(leftTrapezoid);
        if (rightExists)
            tm.addTrapezoid(rightTrapezoid);

        // update old trapezoid neighbors DA COMPLETARE
        size_t oldTrapezoidUpperLeftNeighborIndex = oldTrapezoid.getUpperLeftNeighbor();
        if (oldTrapezoidUpperLeftNeighborIndex != std::numeric_limits<size_t>::max() && leftExists)
            tm.getTrapezoidRefAtIndex(oldTrapezoidUpperLeftNeighborIndex).setUpperRightNeighbor(bottomTrapezoidIndex + 1);

        size_t oldTrapezoidLowerLeftNeighborIndex = oldTrapezoid.getLowerLeftNeighbor();
        if (oldTrapezoidLowerLeftNeighborIndex != std::numeric_limits<size_t>::max() && leftExists)
            tm.getTrapezoidRefAtIndex(oldTrapezoidLowerLeftNeighborIndex).setLowerRightNeighbor(bottomTrapezoidIndex + 1);

        size_t oldTrapezoidUpperRightNeighborIndex = oldTrapezoid.getUpperRightNeighbor();
        if (oldTrapezoidUpperRightNeighborIndex != std::numeric_limits<size_t>::max() && rightExists)
            tm.getTrapezoidRefAtIndex(oldTrapezoidUpperRightNeighborIndex).setUpperRightNeighbor(bottomTrapezoidIndex + 2);

        size_t oldTrapezoidLowerRightNeighborIndex = oldTrapezoid.getLowerRightNeighbor();
        if (oldTrapezoidLowerRightNeighborIndex != std::numeric_limits<size_t>::max() && rightExists)
            tm.getTrapezoidRefAtIndex(oldTrapezoidLowerRightNeighborIndex).setLowerRightNeighbor(bottomTrapezoidIndex + 2);

        // dag split
        Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex);
        Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex);
        Node segmentNode = Node(Node::segment_node, segmentIndex);

        Node topTrapezoidNode = Node(Node::trapezoid_node, trapezoidIndex);
        Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
        Node leftTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex + 1);
        Node rightTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex + 2);

        size_t leftEndpointNodeIndex = oldTrapezoid.getNodeIndex();
        size_t rightEndpointNodeIndex = dag.numberOfNodes();

        if (leftExists)
            leftEndpointNode.setLeftChild(rightEndpointNodeIndex + 4);
        leftEndpointNode.setRightChild(rightEndpointNodeIndex);
        rightEndpointNode.setLeftChild(rightEndpointNodeIndex + 1);
        if (rightExists)
            rightEndpointNode.setRightChild(rightEndpointNodeIndex + 5);

        segmentNode.setLeftChild(rightEndpointNodeIndex + 2);
        segmentNode.setRightChild(rightEndpointNodeIndex + 3);

        dag.addNodeAtIndex(leftEndpointNode, leftEndpointNodeIndex);
        dag.addNode(rightEndpointNode);
        dag.addNode(segmentNode);
        dag.addNode(topTrapezoidNode);
        tm.getTrapezoidRefAtIndex(trapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
        dag.addNode(bottomTrapezoidNode);
        tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
        if (leftExists)
        {
            dag.addNode(leftTrapezoidNode);
            tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex + 1).setNodeIndex(dag.numberOfNodes() - 1);
        }
        if (rightExists)
        {
            dag.addNode(rightTrapezoidNode);
            tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex + 2).setNodeIndex(dag.numberOfNodes() - 1);
        }
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
