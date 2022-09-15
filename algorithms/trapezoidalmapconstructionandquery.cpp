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

    size_t merge(TrapezoidalMap &tm, const size_t leftTrapezoidIndex, const size_t rightTrapezoidIndex)
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
        // add segment and its endpoints to the map
        size_t leftEndpointIndex = tm.numberOfPoints();
        size_t rightEndpointIndex = leftEndpointIndex + 1;
        tm.addPoint(segment.p1());
        tm.addPoint(segment.p2());

        size_t segmentIndex = tm.numberOfSegments();
        tm.addSegment(segment);

        // trapezoidal map split
        bool isMergeTop;
        size_t mergeCandidate = std::numeric_limits<size_t>::max();
        size_t mergedTrapezoid = tm.getMergedTrapezoid();
        for (size_t i = 0; i < trapezoidIndexes.size(); i++)
        {
            // determine which type of split will be applied
            Trapezoid splitTrapezoid = tm.getTrapezoidAtIndex(trapezoidIndexes[i]);
            bool leftTrapezoidExists = false;
            bool rightTrapezoidExists = false;

            if (segment.p1().x() > splitTrapezoid.getLeftPoint().x())
                leftTrapezoidExists = true;
            if (segment.p2().x() < splitTrapezoid.getRightPoint().x())
                rightTrapezoidExists = true;

            // assing new trapezoids' indexes
            std::list<size_t> availableIndexes;
            availableIndexes.push_back(trapezoidIndexes[i]);
            if (mergedTrapezoid != std::numeric_limits<size_t>::max())
                availableIndexes.push_back(mergedTrapezoid);
            availableIndexes.push_back(tm.numberOfTrapezoids());

            size_t topTrapezoidIndex;
            size_t bottomTrapezoidIndex;
            size_t leftTrapezoidIndex = std::numeric_limits<size_t>::max();
            size_t rightTrapezoidIndex = std::numeric_limits<size_t>::max();

            topTrapezoidIndex = availableIndexes.front();
            availableIndexes.pop_front();

            bottomTrapezoidIndex = availableIndexes.front();
            availableIndexes.pop_front();

            if (leftTrapezoidExists)
            {
                if (!availableIndexes.empty())
                {
                    leftTrapezoidIndex = availableIndexes.front();
                    availableIndexes.pop_front();
                }
                else
                    leftTrapezoidIndex = bottomTrapezoidIndex + 1;
            }

            if (rightTrapezoidExists)
            {
                if (leftTrapezoidExists)
                    rightTrapezoidIndex = leftTrapezoidIndex + 1;
                else if (!availableIndexes.empty())
                {
                    rightTrapezoidIndex = availableIndexes.front();
                    availableIndexes.pop_front();
                }
                else
                    rightTrapezoidIndex = bottomTrapezoidIndex + 1;
            }

            cg3::Point2d topTrapezoidLeftPoint = leftTrapezoidExists ? segment.p1() : splitTrapezoid.getLeftPoint();
            cg3::Point2d topTrapezoidRightPoint = rightTrapezoidExists ? segment.p2() : splitTrapezoid.getRightPoint();
            cg3::Point2d bottomTrapezoidLeftPoint = leftTrapezoidExists ? segment.p1() : splitTrapezoid.getLeftPoint();
            cg3::Point2d bottomTrapezoidRightPoint = rightTrapezoidExists ? segment.p2() : splitTrapezoid.getRightPoint();

            Trapezoid topTrapezoid = Trapezoid(splitTrapezoid.getTop(), segment, topTrapezoidLeftPoint, topTrapezoidRightPoint,
                                               leftTrapezoidIndex, rightTrapezoidIndex, std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max());
            Trapezoid bottomTrapezoid = Trapezoid(segment, splitTrapezoid.getBottom(), bottomTrapezoidLeftPoint, bottomTrapezoidRightPoint,
                                                  std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), leftTrapezoidIndex, rightTrapezoidIndex);
            Trapezoid leftTrapezoid = Trapezoid(splitTrapezoid.getTop(), splitTrapezoid.getBottom(), splitTrapezoid.getLeftPoint(), segment.p1(),
                                                std::numeric_limits<size_t>::max(), topTrapezoidIndex, std::numeric_limits<size_t>::max(), bottomTrapezoidIndex);
            Trapezoid rightTrapezoid = Trapezoid(splitTrapezoid.getTop(), splitTrapezoid.getBottom(), segment.p2(), splitTrapezoid.getRightPoint(),
                                                 topTrapezoidIndex, std::numeric_limits<size_t>::max(), bottomTrapezoidIndex, std::numeric_limits<size_t>::max());

            tm.addTrapezoidAtIndex(topTrapezoid, topTrapezoidIndex);
            tm.addTrapezoidAtIndex(bottomTrapezoid, bottomTrapezoidIndex);
            if (leftTrapezoidExists)
                tm.addTrapezoidAtIndex(leftTrapezoid, leftTrapezoidIndex);
            if (rightTrapezoidExists)
                tm.addTrapezoidAtIndex(rightTrapezoid, rightTrapezoidIndex);

            // update old trapezoid neighbors (could an old neighbor be mergedTrapezoid??)
            size_t splitTrapezoidUpperLeftNeighborIndex = splitTrapezoid.getUpperLeftNeighbor();
            if (splitTrapezoidUpperLeftNeighborIndex != std::numeric_limits<size_t>::max())
            {
                if (leftTrapezoidExists)
                    tm.getTrapezoidRefAtIndex(splitTrapezoidUpperLeftNeighborIndex).setUpperRightNeighbor(leftTrapezoidIndex);
                else
                    tm.getTrapezoidRefAtIndex(splitTrapezoidUpperLeftNeighborIndex).setUpperRightNeighbor(topTrapezoidIndex);
            }
            size_t splitTrapezoidLowerLeftNeighborIndex = splitTrapezoid.getLowerLeftNeighbor();
            if (splitTrapezoidLowerLeftNeighborIndex != std::numeric_limits<size_t>::max())
            {
                if (leftTrapezoidExists)
                    tm.getTrapezoidRefAtIndex(splitTrapezoidLowerLeftNeighborIndex).setLowerRightNeighbor(leftTrapezoidIndex);
                else
                    tm.getTrapezoidRefAtIndex(splitTrapezoidLowerLeftNeighborIndex).setLowerRightNeighbor(bottomTrapezoidIndex);
            }
            size_t splitTrapezoidUpperRightNeighborIndex = splitTrapezoid.getUpperRightNeighbor();
            if (splitTrapezoidUpperRightNeighborIndex != std::numeric_limits<size_t>::max())
            {
                if (rightTrapezoidExists)
                    tm.getTrapezoidRefAtIndex(splitTrapezoidUpperRightNeighborIndex).setUpperLeftNeighbor(rightTrapezoidIndex);
                else
                    tm.getTrapezoidRefAtIndex(splitTrapezoidUpperRightNeighborIndex).setUpperLeftNeighbor(topTrapezoidIndex);
            }
            size_t splitTrapezoidLowerRightNeighborIndex = splitTrapezoid.getLowerRightNeighbor();
            if (splitTrapezoidLowerRightNeighborIndex != std::numeric_limits<size_t>::max())
            {
                if (rightTrapezoidExists)
                    tm.getTrapezoidRefAtIndex(splitTrapezoidLowerRightNeighborIndex).setLowerLeftNeighbor(rightTrapezoidIndex);
                else
                    tm.getTrapezoidRefAtIndex(splitTrapezoidLowerRightNeighborIndex).setLowerLeftNeighbor(bottomTrapezoidIndex);
            }

            // merge
            if (mergeCandidate != std::numeric_limits<size_t>::max())
            {
                if (isMergeTop)
                    tm.setMergedTrapezoid(merge(tm, mergeCandidate, topTrapezoidIndex));
                else
                    tm.setMergedTrapezoid(merge(tm, mergeCandidate, bottomTrapezoidIndex));
            }

            // dag update
            // segment intersects one trapezoid
            if (segment.p1().x() >= splitTrapezoid.getLeftPoint().x() && segment.p2().x() <= splitTrapezoid.getRightPoint().x())
            {
                size_t leftEndpointNodeIndex = splitTrapezoid.getNodeIndex();
                size_t rightEndpointNodeIndex = dag.numberOfNodes();
                size_t segmentNodeIndex = rightEndpointNodeIndex + 1;
                size_t topTrapezoidNodeIndex = segmentNodeIndex + 1;
                size_t bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                size_t leftTrapezoidNodeIndex = std::numeric_limits<size_t>::max();
                size_t rightTrapezoidNodeIndex = std::numeric_limits<size_t>::max();

                if (leftTrapezoidExists)
                    leftTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;

                if (rightTrapezoidExists)
                {
                    if (leftTrapezoidExists)
                        rightTrapezoidNodeIndex = leftTrapezoidNodeIndex + 1;
                    else
                        rightTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;
                }

                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
                Node leftTrapezoidNode = Node(Node::trapezoid_node, leftTrapezoidIndex);
                Node rightTrapezoidNode = Node(Node::trapezoid_node, rightTrapezoidIndex);

                Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex, leftTrapezoidNodeIndex, rightEndpointNodeIndex);
                Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex, segmentNodeIndex, rightTrapezoidNodeIndex);
                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);

                dag.addNodeAtIndex(leftEndpointNode, leftEndpointNodeIndex);
                dag.addNode(rightEndpointNode);
                dag.addNode(segmentNode);
                dag.addNode(topTrapezoidNode);
                tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                dag.addNode(bottomTrapezoidNode);
                tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                if (leftTrapezoidExists)
                {
                    dag.addNode(leftTrapezoidNode);
                    tm.getTrapezoidRefAtIndex(leftTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                }
                if (rightTrapezoidExists)
                {
                    dag.addNode(rightTrapezoidNode);
                    tm.getTrapezoidRefAtIndex(rightTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                }
            }
            // first of multiple trapezoids intersected by segment
            else if (segment.p1().x() >= splitTrapezoid.getLeftPoint().x() && segment.p2().x() > splitTrapezoid.getRightPoint().x())
            {
                size_t leftEndpointNodeIndex = splitTrapezoid.getNodeIndex();
                size_t segmentNodeIndex = dag.numberOfNodes();
                size_t topTrapezoidNodeIndex = segmentNodeIndex + 1;
                size_t bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                size_t leftTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;

                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
                Node leftTrapezoidNode = Node(Node::trapezoid_node, leftTrapezoidIndex);

                Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex, leftTrapezoidNodeIndex, segmentNodeIndex);
                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);

                dag.addNodeAtIndex(leftEndpointNode, leftEndpointNodeIndex);
                dag.addNode(segmentNode);
                dag.addNode(topTrapezoidNode);
                tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                dag.addNode(bottomTrapezoidNode);
                tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                dag.addNode(leftTrapezoidNode);
                tm.getTrapezoidRefAtIndex(leftTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            }
            // trapezoid completely crossed by segment
            else if (segment.p1().x() < splitTrapezoid.getLeftPoint().x() && segment.p2().x() > splitTrapezoid.getRightPoint().x())
            {
                size_t segmentIndex = splitTrapezoid.getNodeIndex();
                size_t topTrapezoidNodeIndex = dag.numberOfNodes();
                size_t bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;

                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);
                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);

                dag.addNodeAtIndex(segmentNode, segmentIndex);
                dag.addNode(topTrapezoidNode);
                tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                dag.addNode(bottomTrapezoidNode);
                tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            }
            // last of multiple trapezoids intersected by segment
            else if (segment.p1().x() < splitTrapezoid.getLeftPoint().x() && segment.p2().x() <= splitTrapezoid.getRightPoint().x())
            {
                size_t rightEndpointNodeIndex = splitTrapezoid.getNodeIndex();
                size_t segmentNodeIndex = dag.numberOfNodes();
                size_t topTrapezoidNodeIndex;
                size_t bottomTrapezoidNodeIndex;
                size_t rightTrapezoidNodeIndex;

                if (mergeCandidate != std::numeric_limits<size_t>::max())
                {
                    if (isMergeTop)
                    {
                        if (topTrapezoidIndex != tm.getMergedTrapezoid())
                        {
                            topTrapezoidNodeIndex = segmentNodeIndex + 1;
                            bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                        }
                        else
                        {
                            topTrapezoidNodeIndex = trapezoidIndexToNodeIndex(tm, mergeCandidate);
                            bottomTrapezoidNodeIndex = segmentNodeIndex + 1;
                        }

                        rightTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;
                    }
                    else
                    {
                        topTrapezoidNodeIndex = segmentNodeIndex + 1;

                        if (bottomTrapezoidIndex != tm.getMergedTrapezoid())
                        {
                            bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                            rightTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;
                        }
                        else
                        {
                            bottomTrapezoidNodeIndex = trapezoidIndexToNodeIndex(tm, mergeCandidate);
                            rightTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                        }
                    }
                }

                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
                Node rightTrapezoidNode = Node(Node::trapezoid_node, rightTrapezoidIndex);

                Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex, segmentNodeIndex, rightTrapezoidNodeIndex);

                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);

                dag.addNodeAtIndex(rightEndpointNode, rightEndpointNodeIndex);
                dag.addNode(segmentNode);
                if (topTrapezoidNodeIndex == dag.numberOfNodes())
                {
                    dag.addNode(topTrapezoidNode);
                    tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                }
                if (bottomTrapezoidNodeIndex == dag.numberOfNodes())
                {
                    dag.addNode(bottomTrapezoidNode);
                    tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
                }
                dag.addNode(rightTrapezoidNode);
                tm.getTrapezoidRefAtIndex(rightTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            }

            if (cg3::isPointAtLeft(segment, splitTrapezoid.getRightPoint()))
            {
                mergeCandidate = bottomTrapezoidIndex;
                isMergeTop = false;
            }
            else
            {
                mergeCandidate = topTrapezoidIndex;
                isMergeTop = true;
            }
/*
            // dag split
            Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex);
            Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex);
            Node segmentNode = Node(Node::segment_node, segmentIndex);

            Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
            Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
            Node leftTrapezoidNode = Node(Node::trapezoid_node, leftTrapezoidIndex);
            Node rightTrapezoidNode = Node(Node::trapezoid_node, rightTrapezoidIndex);

            size_t leftEndpointNodeIndex = splitTrapezoid.getNodeIndex();
            size_t rightEndpointNodeIndex = dag.numberOfNodes();
            size_t segmentNodeIndex = rightEndpointNodeIndex + 1;
            size_t topTrapezoidNodeIndex = segmentNodeIndex + 1;
            size_t bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
            size_t leftTrapezoidNodeIndex;
            size_t rightTrapezoidNodeIndex;

            if (leftTrapezoidExists)
                leftTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;

            if (leftTrapezoidExists)
                rightTrapezoidNodeIndex = leftTrapezoidNodeIndex + 1;
            else if (rightTrapezoidExists)
                rightTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;

            if (leftTrapezoidExists)
                leftEndpointNode.setLeftChild(leftTrapezoidNodeIndex);
            leftEndpointNode.setRightChild(rightEndpointNodeIndex);
            rightEndpointNode.setLeftChild(segmentNodeIndex);
            if (rightTrapezoidExists)
                rightEndpointNode.setRightChild(rightTrapezoidNodeIndex);

            segmentNode.setLeftChild(topTrapezoidNodeIndex);
            segmentNode.setRightChild(bottomTrapezoidNodeIndex);

            dag.addNodeAtIndex(leftEndpointNode, leftEndpointNodeIndex);
            dag.addNode(rightEndpointNode);
            dag.addNode(segmentNode);
            dag.addNode(topTrapezoidNode);
            tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            dag.addNode(bottomTrapezoidNode);
            tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            if (leftTrapezoidExists)
            {
                dag.addNode(leftTrapezoidNode);
                tm.getTrapezoidRefAtIndex(leftTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            }
            if (rightTrapezoidExists)
            {
                dag.addNode(rightTrapezoidNode);
                tm.getTrapezoidRefAtIndex(rightTrapezoidIndex).setNodeIndex(dag.numberOfNodes() - 1);
            }
*/
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
