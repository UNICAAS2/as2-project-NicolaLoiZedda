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

        std::cout << "index = " << node.getIndex() << std::endl;
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
        while (rightP.x() < q.x())
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

    size_t merge(TrapezoidalMap &tm, DirectedAcyclicGraph &dag, const size_t leftTrapezoidIndex, const size_t rightTrapezoidIndex)
    {
        Trapezoid& leftTrapezoid = tm.getTrapezoidRefAtIndex(leftTrapezoidIndex);
        Trapezoid rightTrapezoid = tm.getTrapezoidAtIndex(rightTrapezoidIndex);

        if (leftTrapezoid.getTop() == rightTrapezoid.getTop() && leftTrapezoid.getBottom() == rightTrapezoid.getBottom())
        {
            leftTrapezoid.setRightPoint(rightTrapezoid.getRightPoint());
            leftTrapezoid.setUpperRightNeighbor(rightTrapezoid.getUpperRightNeighbor());
            leftTrapezoid.setLowerRightNeighbor(rightTrapezoid.getLowerRightNeighbor());

            // update neighbors
            tm.getTrapezoidRefAtIndex(rightTrapezoid.getUpperRightNeighbor()).setUpperLeftNeighbor(leftTrapezoidIndex);
            tm.getTrapezoidRefAtIndex(rightTrapezoid.getLowerRightNeighbor()).setLowerLeftNeighbor(leftTrapezoidIndex);

            return rightTrapezoidIndex;
        }

        return std::numeric_limits<size_t>::max();
    }

    void splitTrapezoids(TrapezoidalMap &tm, DirectedAcyclicGraph &dag, std::vector<size_t> trapezoidIndexes, const cg3::Segment2d &segment)
    {
        // trapezoidal map split
        size_t leftEndpointIndex = tm.numberOfPoints();
        size_t rightEndpointIndex = leftEndpointIndex + 1;
        tm.addPoint(segment.p1());
        tm.addPoint(segment.p2());

        size_t segmentIndex = tm.numberOfSegments();
        tm.addSegment(segment);

        bool isMergeTop;
        size_t mergeCandidate = std::numeric_limits<size_t>::max();
        // get merged trap(?)
        for (size_t i = 0; i < trapezoidIndexes.size(); i++)
        {
            size_t trapezoidIndex = trapezoidIndexes[i];
            size_t bottomTrapezoidIndex = tm.numberOfTrapezoids();
            size_t leftTrapezoidIndex;
            size_t rightTrapezoidIndex;

            Trapezoid oldTrapezoid = tm.getTrapezoidAtIndex(trapezoidIndex);            
            Trapezoid topTrapezoid = Trapezoid();
            Trapezoid bottomTrapezoid = Trapezoid();
            Trapezoid leftTrapezoid = Trapezoid();
            Trapezoid rightTrapezoid = Trapezoid();

            bool leftExists = false;
            bool rightExists = false;

            if (segment.p1().x() > oldTrapezoid.getLeftPoint().x())
                leftExists = true;
            if (segment.p2().x() < oldTrapezoid.getRightPoint().x())
                rightExists = true;

            topTrapezoid.setTop(oldTrapezoid.getTop());
            topTrapezoid.setBottom(segment);
            if (leftExists)
                topTrapezoid.setLeftPoint(segment.p1());
            else
                topTrapezoid.setLeftPoint(oldTrapezoid.getLeftPoint());
            if (rightExists)
                topTrapezoid.setRightPoint(segment.p2());
            else
                topTrapezoid.setRightPoint(oldTrapezoid.getRightPoint());

            bottomTrapezoid.setTop(segment);
            bottomTrapezoid.setBottom(oldTrapezoid.getBottom());
            if (leftExists)
                bottomTrapezoid.setLeftPoint(segment.p1());
            else
                bottomTrapezoid.setLeftPoint(oldTrapezoid.getLeftPoint());
            if (rightExists)
                bottomTrapezoid.setRightPoint(segment.p2());
            else
                bottomTrapezoid.setRightPoint(oldTrapezoid.getRightPoint());

            if (leftExists)
                leftTrapezoidIndex = bottomTrapezoidIndex + 1;

            if (leftExists)
                rightTrapezoidIndex = leftTrapezoidIndex + 1;
            else if (rightExists)
                rightTrapezoidIndex = bottomTrapezoidIndex + 1;

            if (leftExists)
            {
                leftTrapezoid.setTop(oldTrapezoid.getTop());
                leftTrapezoid.setBottom(oldTrapezoid.getBottom());
                leftTrapezoid.setLeftPoint(oldTrapezoid.getLeftPoint());
                leftTrapezoid.setRightPoint(segment.p1());

                leftTrapezoid.setUpperRightNeighbor(trapezoidIndex);
                leftTrapezoid.setLowerRightNeighbor(bottomTrapezoidIndex);

                topTrapezoid.setUpperLeftNeighbor(leftTrapezoidIndex);
                bottomTrapezoid.setLowerLeftNeighbor(leftTrapezoidIndex);
            }

            if (rightExists)
            {
                rightTrapezoid.setTop(oldTrapezoid.getTop());
                rightTrapezoid.setBottom(oldTrapezoid.getBottom());
                rightTrapezoid.setLeftPoint(segment.p2());
                rightTrapezoid.setRightPoint(oldTrapezoid.getRightPoint());

                rightTrapezoid.setUpperLeftNeighbor(trapezoidIndex);
                rightTrapezoid.setLowerLeftNeighbor(bottomTrapezoidIndex);

                topTrapezoid.setUpperRightNeighbor(rightTrapezoidIndex);
                bottomTrapezoid.setLowerRightNeighbor(rightTrapezoidIndex);
            }

            tm.addTrapezoidAtIndex(topTrapezoid, trapezoidIndex);
            tm.addTrapezoid(bottomTrapezoid);
            if (leftExists)
                tm.addTrapezoid(leftTrapezoid);
            if (rightExists)
                tm.addTrapezoid(rightTrapezoid);

            // update old trapezoid neighbors
            size_t oldTrapezoidUpperLeftNeighborIndex = oldTrapezoid.getUpperLeftNeighbor();
            if (oldTrapezoidUpperLeftNeighborIndex != std::numeric_limits<size_t>::max() && leftExists)
                tm.getTrapezoidRefAtIndex(oldTrapezoidUpperLeftNeighborIndex).setUpperRightNeighbor(leftTrapezoidIndex);

            size_t oldTrapezoidLowerLeftNeighborIndex = oldTrapezoid.getLowerLeftNeighbor();
            if (oldTrapezoidLowerLeftNeighborIndex != std::numeric_limits<size_t>::max() && leftExists)
                tm.getTrapezoidRefAtIndex(oldTrapezoidLowerLeftNeighborIndex).setLowerRightNeighbor(leftTrapezoidIndex);

            size_t oldTrapezoidUpperRightNeighborIndex = oldTrapezoid.getUpperRightNeighbor();
            if (oldTrapezoidUpperRightNeighborIndex != std::numeric_limits<size_t>::max() && rightExists)
                tm.getTrapezoidRefAtIndex(oldTrapezoidUpperRightNeighborIndex).setUpperLeftNeighbor(rightTrapezoidIndex);

            size_t oldTrapezoidLowerRightNeighborIndex = oldTrapezoid.getLowerRightNeighbor();
            if (oldTrapezoidLowerRightNeighborIndex != std::numeric_limits<size_t>::max() && rightExists)
                tm.getTrapezoidRefAtIndex(oldTrapezoidLowerRightNeighborIndex).setLowerLeftNeighbor(rightTrapezoidIndex);

            // dag split
            Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex);
            Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex);
            Node segmentNode = Node(Node::segment_node, segmentIndex);

            Node topTrapezoidNode = Node(Node::trapezoid_node, trapezoidIndex);
            Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
            Node leftTrapezoidNode = Node(Node::trapezoid_node, leftTrapezoidIndex);
            Node rightTrapezoidNode = Node(Node::trapezoid_node, rightTrapezoidIndex);

            size_t leftEndpointNodeIndex = oldTrapezoid.getNodeIndex();
            size_t rightEndpointNodeIndex = dag.numberOfNodes();
            size_t segmentNodeIndex = rightEndpointNodeIndex + 1;
            size_t topTrapezoidNodeIndex = segmentNodeIndex + 1;
            size_t bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
            size_t leftTrapezoidNodeIndex;
            size_t rightTrapezoidNodeIndex;

            if (leftExists)
                leftTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;

            if (leftExists)
                rightTrapezoidNodeIndex = leftTrapezoidNodeIndex + 1;
            else if (rightExists)
                rightTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;

            if (leftExists)
                leftEndpointNode.setLeftChild(leftTrapezoidNodeIndex);
            leftEndpointNode.setRightChild(rightEndpointNodeIndex);
            rightEndpointNode.setLeftChild(segmentNodeIndex);
            if (rightExists)
                rightEndpointNode.setRightChild(rightTrapezoidNodeIndex);

            segmentNode.setLeftChild(topTrapezoidNodeIndex);
            segmentNode.setRightChild(bottomTrapezoidNodeIndex);

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
                tm.getTrapezoidRefAtIndex(leftTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            }
            if (rightExists)
            {
                dag.addNode(rightTrapezoidNode);
                tm.getTrapezoidRefAtIndex(rightTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            }

            // MERGE
            if (mergeCandidate != std::numeric_limits<size_t>::max())
            {
                if (isMergeTop)
                    tm.setMergedTrapezoid(merge(tm, dag, mergeCandidate, trapezoidIndex));
                else
                    tm.setMergedTrapezoid(merge(tm, dag, mergeCandidate, bottomTrapezoidIndex));
            }

            if (cg3::isPointAtLeft(segment, oldTrapezoid.getRightPoint()))
            {
                mergeCandidate = bottomTrapezoidIndex;
                isMergeTop = false;
            }
            else
            {
                mergeCandidate = trapezoidIndex;
                isMergeTop = true;
            }
       }
    }

    void incrementalStep(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        const cg3::Segment2d orderedSegment = GeometryFunctions::getOrderedSegment(segment);

        std::vector<size_t> intersectedTrapezoidsIndexes = followSegment(tm, dag, orderedSegment);

        splitTrapezoids(tm, dag, intersectedTrapezoidsIndexes, orderedSegment);
    }

    void clearStructures(TrapezoidalMap& tm, DirectedAcyclicGraph& dag)
    {
        tm.clear();
        dag.clear();
    }
}
