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
            if (rightTrapezoid.getUpperRightNeighbor() != std::numeric_limits<size_t>::max())
                tm.getTrapezoidRefAtIndex(rightTrapezoid.getUpperRightNeighbor()).setUpperLeftNeighbor(leftTrapezoidIndex);
            if (rightTrapezoid.getLowerRightNeighbor() != std::numeric_limits<size_t>::max())
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
        std::vector<size_t> lastTwoTrapezoidsInserted(2, std::numeric_limits<size_t>::max());
        size_t mergeCandidate = std::numeric_limits<size_t>::max();
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
            if (tm.getMergedTrapezoid() != std::numeric_limits<size_t>::max())
                availableIndexes.push_back(tm.getMergedTrapezoid());
            availableIndexes.push_back(tm.numberOfTrapezoids());

            size_t topTrapezoidIndex;
            size_t bottomTrapezoidIndex;
            size_t leftTrapezoidIndex = std::numeric_limits<size_t>::max();
            size_t rightTrapezoidIndex = std::numeric_limits<size_t>::max();

            // useful in case of precedent split2 or split3
            size_t topTrapezoidUpperLeftNeighborIndex = std::numeric_limits<size_t>::max();
            size_t topTrapezoidUpperRightNeighborIndex = std::numeric_limits<size_t>::max();
            size_t topTrapezoidLowerLeftNeighborIndex = std::numeric_limits<size_t>::max();
            //size_t topTrapezoidLowerRightNeighborIndex = std::numeric_limits<size_t>::max();//DA DEFINIRE

            size_t bottomTrapezoidUpperLeftNeighborIndex = std::numeric_limits<size_t>::max();
            //size_t bottomTrapezoidUpperRightNeighborIndex = std::numeric_limits<size_t>::max();//DA DEFINIRE
            size_t bottomTrapezoidLowerLeftNeighborIndex = std::numeric_limits<size_t>::max();
            size_t bottomTrapezoidLowerRightNeighborIndex = std::numeric_limits<size_t>::max();

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

                topTrapezoidUpperLeftNeighborIndex = leftTrapezoidIndex;
                bottomTrapezoidLowerLeftNeighborIndex = leftTrapezoidIndex;
            }
            else
            {
                topTrapezoidUpperLeftNeighborIndex = splitTrapezoid.getUpperLeftNeighbor();
                topTrapezoidLowerLeftNeighborIndex = lastTwoTrapezoidsInserted[0];
                bottomTrapezoidUpperLeftNeighborIndex = lastTwoTrapezoidsInserted[1];
                bottomTrapezoidLowerLeftNeighborIndex = splitTrapezoid.getLowerLeftNeighbor();
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

                topTrapezoidUpperRightNeighborIndex = rightTrapezoidIndex;
                bottomTrapezoidLowerRightNeighborIndex = rightTrapezoidIndex;
            }
            else
            {
                topTrapezoidUpperRightNeighborIndex = splitTrapezoid.getUpperRightNeighbor();
                bottomTrapezoidLowerRightNeighborIndex = splitTrapezoid.getLowerRightNeighbor();
            }

            cg3::Point2d topTrapezoidLeftPoint = (leftTrapezoidExists) ? segment.p1() : splitTrapezoid.getLeftPoint();
            cg3::Point2d topTrapezoidRightPoint = (rightTrapezoidExists) ? segment.p2() : splitTrapezoid.getRightPoint();
            cg3::Point2d bottomTrapezoidLeftPoint = (leftTrapezoidExists) ? segment.p1() : splitTrapezoid.getLeftPoint();
            cg3::Point2d bottomTrapezoidRightPoint = (rightTrapezoidExists) ? segment.p2() : splitTrapezoid.getRightPoint();

            Trapezoid topTrapezoid = Trapezoid(splitTrapezoid.getTop(), segment, topTrapezoidLeftPoint, topTrapezoidRightPoint,
                                               topTrapezoidUpperLeftNeighborIndex, topTrapezoidUpperRightNeighborIndex, topTrapezoidLowerLeftNeighborIndex, std::numeric_limits<size_t>::max(),
                                               splitTrapezoid.getNodeIndex());
            Trapezoid bottomTrapezoid = Trapezoid(segment, splitTrapezoid.getBottom(), bottomTrapezoidLeftPoint, bottomTrapezoidRightPoint,
                                                  bottomTrapezoidUpperLeftNeighborIndex, std::numeric_limits<size_t>::max(), bottomTrapezoidLowerLeftNeighborIndex, bottomTrapezoidLowerRightNeighborIndex);
            Trapezoid leftTrapezoid = Trapezoid(splitTrapezoid.getTop(), splitTrapezoid.getBottom(), splitTrapezoid.getLeftPoint(), segment.p1(),
                                                splitTrapezoid.getUpperLeftNeighbor(), topTrapezoidIndex, splitTrapezoid.getLowerLeftNeighbor(), bottomTrapezoidIndex);
            Trapezoid rightTrapezoid = Trapezoid(splitTrapezoid.getTop(), splitTrapezoid.getBottom(), segment.p2(), splitTrapezoid.getRightPoint(),
                                                 topTrapezoidIndex, splitTrapezoid.getUpperRightNeighbor(), bottomTrapezoidIndex, splitTrapezoid.getLowerRightNeighbor());

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

            if (!leftTrapezoidExists)
            {
                if (lastTwoTrapezoidsInserted[0] != std::numeric_limits<size_t>::max())
                    tm.getTrapezoidRefAtIndex(lastTwoTrapezoidsInserted[0]).setLowerRightNeighbor(topTrapezoidIndex);
                if (lastTwoTrapezoidsInserted[1] != std::numeric_limits<size_t>::max())
                    tm.getTrapezoidRefAtIndex(lastTwoTrapezoidsInserted[1]).setUpperRightNeighbor(bottomTrapezoidIndex);
            }

            // merge
            if (mergeCandidate == 0)
                tm.setMergedTrapezoid(merge(tm, lastTwoTrapezoidsInserted[mergeCandidate], topTrapezoidIndex));
            else if (mergeCandidate == 1)
                tm.setMergedTrapezoid(merge(tm, lastTwoTrapezoidsInserted[mergeCandidate], bottomTrapezoidIndex));
            else
                tm.setMergedTrapezoid(std::numeric_limits<size_t>::max());

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
                dag.addNodeAtIndex(rightEndpointNode, rightEndpointNodeIndex);
                dag.addNodeAtIndex(segmentNode, segmentNodeIndex);
                dag.addNodeAtIndex(topTrapezoidNode, topTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(topTrapezoidNodeIndex);
                dag.addNodeAtIndex(bottomTrapezoidNode, bottomTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(bottomTrapezoidNodeIndex);
                if (leftTrapezoidExists)
                {
                    dag.addNodeAtIndex(leftTrapezoidNode, leftTrapezoidNodeIndex);
                    tm.getTrapezoidRefAtIndex(leftTrapezoidIndex).setNodeIndex(leftTrapezoidNodeIndex);
                }
                if (rightTrapezoidExists)
                {
                    dag.addNodeAtIndex(rightTrapezoidNode, rightTrapezoidNodeIndex);
                    tm.getTrapezoidRefAtIndex(rightTrapezoidIndex).setNodeIndex(rightTrapezoidNodeIndex);
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
                dag.addNodeAtIndex(segmentNode, segmentNodeIndex);
                dag.addNodeAtIndex(topTrapezoidNode, topTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(topTrapezoidNodeIndex);
                dag.addNodeAtIndex(bottomTrapezoidNode, bottomTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(bottomTrapezoidNodeIndex);
                dag.addNodeAtIndex(leftTrapezoidNode, leftTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(leftTrapezoidIndex).setNodeIndex(leftTrapezoidNodeIndex);
            }
            // trapezoid completely crossed by segment
            else if (segment.p1().x() < splitTrapezoid.getLeftPoint().x() && segment.p2().x() > splitTrapezoid.getRightPoint().x())
            {
                size_t segmentNodeIndex = splitTrapezoid.getNodeIndex();
                size_t topTrapezoidNodeIndex;
                size_t bottomTrapezoidNodeIndex;

                if (lastTwoTrapezoidsInserted[mergeCandidate] != std::numeric_limits<size_t>::max())
                {
                    if (mergeCandidate == 0)
                    {
                        if (topTrapezoidIndex != tm.getMergedTrapezoid())
                        {
                            topTrapezoidNodeIndex = dag.numberOfNodes();
                            bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                        }
                        else
                        {
                            topTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                            topTrapezoidNodeIndex = trapezoidIndexToNodeIndex(tm, topTrapezoidIndex);
                            bottomTrapezoidNodeIndex = dag.numberOfNodes();
                        }
                    }
                    else
                    {
                        topTrapezoidNodeIndex = dag.numberOfNodes();

                        if (bottomTrapezoidIndex != tm.getMergedTrapezoid())
                            bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                        else
                        {
                            bottomTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                            bottomTrapezoidNodeIndex = trapezoidIndexToNodeIndex(tm, bottomTrapezoidIndex);
                        }
                    }
                }

                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);
                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);

                dag.addNodeAtIndex(segmentNode, segmentNodeIndex);
                dag.addNodeAtIndex(topTrapezoidNode, topTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(topTrapezoidNodeIndex);
                dag.addNodeAtIndex(bottomTrapezoidNode, bottomTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(bottomTrapezoidNodeIndex);
            }
            // last of multiple trapezoids intersected by segment
            else if (segment.p1().x() < splitTrapezoid.getLeftPoint().x() && segment.p2().x() <= splitTrapezoid.getRightPoint().x())
            {
                size_t rightEndpointNodeIndex = splitTrapezoid.getNodeIndex();
                size_t segmentNodeIndex = dag.numberOfNodes();
                size_t topTrapezoidNodeIndex;
                size_t bottomTrapezoidNodeIndex;
                size_t rightTrapezoidNodeIndex;

                if (lastTwoTrapezoidsInserted[mergeCandidate] != std::numeric_limits<size_t>::max())
                {
                    if (mergeCandidate == 0)
                    {
                        if (topTrapezoidIndex != tm.getMergedTrapezoid())
                        {
                            topTrapezoidNodeIndex = segmentNodeIndex + 1;
                            bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                        }
                        else
                        {
                            topTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                            topTrapezoidNodeIndex = trapezoidIndexToNodeIndex(tm, topTrapezoidIndex);
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
                            bottomTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                            bottomTrapezoidNodeIndex = trapezoidIndexToNodeIndex(tm, bottomTrapezoidIndex);
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
                dag.addNodeAtIndex(segmentNode, segmentNodeIndex);
                if (topTrapezoidNodeIndex == dag.numberOfNodes())
                {
                    dag.addNodeAtIndex(topTrapezoidNode, topTrapezoidNodeIndex);
                    tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(topTrapezoidNodeIndex);
                }
                if (bottomTrapezoidNodeIndex == dag.numberOfNodes())
                {
                    dag.addNodeAtIndex(bottomTrapezoidNode, bottomTrapezoidNodeIndex);
                    tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(bottomTrapezoidNodeIndex);
                }
                dag.addNodeAtIndex(rightTrapezoidNode, rightTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(rightTrapezoidIndex).setNodeIndex(rightTrapezoidNodeIndex);
            }

            if (cg3::isPointAtLeft(segment, splitTrapezoid.getRightPoint()))
            {
                mergeCandidate = 1;
                lastTwoTrapezoidsInserted[0] = (rightTrapezoidIndex == std::numeric_limits<size_t>::max()) ? topTrapezoidIndex : std::numeric_limits<size_t>::max();
                lastTwoTrapezoidsInserted[1] = bottomTrapezoidIndex;
            }
            else
            {
                mergeCandidate = 0;
                lastTwoTrapezoidsInserted[0] = topTrapezoidIndex;
                lastTwoTrapezoidsInserted[1] = (rightTrapezoidIndex == std::numeric_limits<size_t>::max()) ? bottomTrapezoidIndex : std::numeric_limits<size_t>::max();
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
