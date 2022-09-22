#include "trapezoidalmapconstructionandquery.h"

#include "data_structures/trapezoidalmap_dataset.h"

#include <algorithms/geometryfunctions.h>

namespace TrapezoidalMapConstructionAndQuery
{
    /**
     * @brief getTrapezoidFromPoint gets the trapezoid on which a point lies
     * @param tm the trapezoidal map
     * @param dag the directed acyclic graph
     * @param queryPoint the point
     * @return the index in the map of the trapezoid on which the point lies
     */
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

    /**
     * @brief getLeftmostTrapezoidIntersectedBySegment gets the leftmost trapezoid intersected by a segment
     * @param tm the trapezoidal map
     * @param dag the directed acyclic graph
     * @param segment the segment
     * @return the index in the map of the leftmost trapezoid intersected by the segment
     */
    size_t getLeftmostTrapezoidIntersectedBySegment(const TrapezoidalMap& tm, const DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        Node node = dag.getRoot();

        while (node.getType() != Node::trapezoid_node)
        {
            if (node.getType() == Node::point_node)
            {
                cg3::Point2d point = tm.getPointAtIndex(node.getIndex());

                if (point.x() > segment.p1().x())
                    node = dag.getNode(node.getLeftChild());
                else
                    node = dag.getNode(node.getRightChild());
            }
            else
            {
                cg3::Segment2d nodeSegment = tm.getSegmentAtIndex(node.getIndex());

                if (cg3::isPointAtLeft(nodeSegment, segment.p1()))
                    node = dag.getNode(node.getLeftChild());
                else if (cg3::isPointAtRight(nodeSegment, segment.p1()))
                    node = dag.getNode(node.getRightChild());
                else if (GeometryFunctions::slope(segment) > GeometryFunctions::slope(nodeSegment))
                    node = dag.getNode(node.getLeftChild());
                else
                    node = dag.getNode(node.getRightChild());
            }
        }

        return node.getIndex();
    }

    /**
     * @brief followSegment get all the trapezoids intersected by a segment
     * @param tm the trapezoidal map
     * @param dag the directed acyclic graph
     * @param segment the segment
     * @return a vector containing all the trapezoids intersected by the segment
     */
    const std::vector<size_t> followSegment(const TrapezoidalMap& tm, const DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        std::vector<size_t> intersectedTrapezoids;

        intersectedTrapezoids.push_back(getLeftmostTrapezoidIntersectedBySegment(tm, dag, segment));
        cg3::Point2d rightP = tm.getTrapezoidAtIndex(intersectedTrapezoids[0]).getRightPoint();

        size_t i = 0;
        while (rightP.x() < segment.p2().x())
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

    /**
     * @brief merge performs the merge operation of two trapezoids
     * @param tm the trapezoidal map
     * @param leftTrapezoidIndex index in the map of the left trapezoid to merge
     * @param rightTrapezoidIndex index in the map of the right trapezoid to merge
     * @return the index of the merged trapezoid
     */
    size_t merge(TrapezoidalMap& tm, const size_t leftTrapezoidIndex, const size_t rightTrapezoidIndex)
    {
        Trapezoid& leftTrapezoid = tm.getTrapezoidRefAtIndex(leftTrapezoidIndex);
        Trapezoid rightTrapezoid = tm.getTrapezoidAtIndex(rightTrapezoidIndex);

        // if the two trapezoids have the properties to merge the merge is performed
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

            // we return the index of the merged trapezoid
            return rightTrapezoidIndex;
        }

        return std::numeric_limits<size_t>::max();
    }

    /**
     * @brief splitTrapezoids execututes all the split operations for each trapezoid intersected by a segment
     * @param tm the trapezoidal map
     * @param dag the directed acyclic graph
     * @param trapezoidIndexes a vector containing the indexes in the map of the intersected trapezoids
     * @param segment the segment being added to the map
     */
    void splitTrapezoids(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, std::vector<size_t>& trapezoidIndexes, const cg3::Segment2d& segment)
    {
        /*
         * we first add the segment and its endpoints to the map
         * we save their indexes in the map for later use
         */
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
            // get the trapezoid on which the split will be applied
            Trapezoid splitTrapezoid = tm.getTrapezoidAtIndex(trapezoidIndexes[i]);

            bool leftTrapezoidExists = false;
            bool rightTrapezoidExists = false;
            /*
             * if the x-coordinate of the leftmost endpoint of the segment is bigger than that of the trapezoid's left point
             * then the left trapezoid exists
             */
            if (segment.p1().x() > splitTrapezoid.getLeftPoint().x())
                leftTrapezoidExists = true;
            /*
             * if the x-coordinate of the rightmost endpoint of the segment is smaller than that of the trapezoid's right point
             * then the right trapezoid exists
             */
            if (segment.p2().x() < splitTrapezoid.getRightPoint().x())
                rightTrapezoidExists = true;

            /*
             * the first available index is that of the intersected trapezoid
             * the second one is that of the merged trapezoid (if it exists)
             * the third one is the smallest unused index, namely the one corresponding to the number of trapezoids already in the map
             * all the following indexes will simply be equal to the precedent index + 1
             */
            std::list<size_t> availableIndexes;
            availableIndexes.push_back(trapezoidIndexes[i]);
            if (tm.getMergedTrapezoid() != std::numeric_limits<size_t>::max())
                availableIndexes.push_back(tm.getMergedTrapezoid());
            availableIndexes.push_back(tm.numberOfTrapezoids());

            size_t topTrapezoidIndex;
            size_t bottomTrapezoidIndex;
            size_t leftTrapezoidIndex = std::numeric_limits<size_t>::max();
            size_t rightTrapezoidIndex = std::numeric_limits<size_t>::max();

            size_t topTrapezoidUpperLeftNeighborIndex = std::numeric_limits<size_t>::max();
            size_t topTrapezoidUpperRightNeighborIndex = std::numeric_limits<size_t>::max();
            size_t topTrapezoidLowerLeftNeighborIndex = std::numeric_limits<size_t>::max();

            size_t bottomTrapezoidUpperLeftNeighborIndex = std::numeric_limits<size_t>::max();
            size_t bottomTrapezoidLowerLeftNeighborIndex = std::numeric_limits<size_t>::max();
            size_t bottomTrapezoidLowerRightNeighborIndex = std::numeric_limits<size_t>::max();

            // top trapezoid gets the first available index
            topTrapezoidIndex = availableIndexes.front();
            availableIndexes.pop_front();

            // bottom trapezoid gets the first available index
            bottomTrapezoidIndex = availableIndexes.front();
            availableIndexes.pop_front();

            if (leftTrapezoidExists)
            {
                // left trapezoid gets the first available index
                if (!availableIndexes.empty())
                {
                    leftTrapezoidIndex = availableIndexes.front();
                    availableIndexes.pop_front();
                }
                else
                    leftTrapezoidIndex = bottomTrapezoidIndex + 1;

                /*
                 * if the left trapezoid exists it will always be
                 * the upper left neighbor of the top trapezoid and
                 * the lower left neighbor of the bottom trapezoid
                 */
                topTrapezoidUpperLeftNeighborIndex = leftTrapezoidIndex;
                bottomTrapezoidLowerLeftNeighborIndex = leftTrapezoidIndex;
            }
            else
            {
                /* if the left trapezoid does not exist
                 * the top trapezoid will inherit the upper left neighbor from the split trapezoid
                 * -----------------------------------------------------------------------------------------------------
                 * the top trapezoid's lower left neighbor will be the last inserted top trapezoid (if it exists)
                 * otherwise it won't have a lower left neighbor
                 * -----------------------------------------------------------------------------------------------------
                 * the bottom trapezoid's upper left neighbor will be the last inserted bottom trapezoid (if it exists)
                 * otherwise it won't have an upper left neighbor
                 * -----------------------------------------------------------------------------------------------------
                 * the bottom trapezoid will inherit the lower left neighbor from the split trapezoid
                 */
                topTrapezoidUpperLeftNeighborIndex = splitTrapezoid.getUpperLeftNeighbor();
                topTrapezoidLowerLeftNeighborIndex = lastTwoTrapezoidsInserted[0];
                bottomTrapezoidUpperLeftNeighborIndex = lastTwoTrapezoidsInserted[1];
                bottomTrapezoidLowerLeftNeighborIndex = splitTrapezoid.getLowerLeftNeighbor();
            }

            if (rightTrapezoidExists)
            {
                // right trapezoid gets the first available index
                if (leftTrapezoidExists)
                    rightTrapezoidIndex = leftTrapezoidIndex + 1;
                else if (!availableIndexes.empty())
                {
                    rightTrapezoidIndex = availableIndexes.front();
                    availableIndexes.pop_front();
                }
                else
                    rightTrapezoidIndex = bottomTrapezoidIndex + 1;

                /*
                 * if the right trapezoid exists it will always be
                 * the upper right neighbor of the top trapezoid and
                 * the lower right neighbor of the bottom trapezoid
                 */
                topTrapezoidUpperRightNeighborIndex = rightTrapezoidIndex;
                bottomTrapezoidLowerRightNeighborIndex = rightTrapezoidIndex;
            }
            else
            {
                /* if the right trapezoid does not exist
                 * the top trapezoid will inherit the upper right neighbor from the split trapezoid and
                 * the bottom trapezoid will inherit the lower right neighbor from the split trapezoid
                 */
                topTrapezoidUpperRightNeighborIndex = splitTrapezoid.getUpperRightNeighbor();
                bottomTrapezoidLowerRightNeighborIndex = splitTrapezoid.getLowerRightNeighbor();
            }

            /*
             * sets the points defining the top and bottom trapezoids
             * if the left trapezoid exists it means that the segment starts inside this trapezoid
             * and the left endpoint of the segment will be used as left point of both the top and bottom trapezoids
             * otherwise they will inherit the left point from the split trapezoid
             * the right point is defined symmetrically
             */
            cg3::Point2d topTrapezoidLeftPoint = (leftTrapezoidExists) ? segment.p1() : splitTrapezoid.getLeftPoint();
            cg3::Point2d topTrapezoidRightPoint = (rightTrapezoidExists) ? segment.p2() : splitTrapezoid.getRightPoint();
            cg3::Point2d bottomTrapezoidLeftPoint = (leftTrapezoidExists) ? segment.p1() : splitTrapezoid.getLeftPoint();
            cg3::Point2d bottomTrapezoidRightPoint = (rightTrapezoidExists) ? segment.p2() : splitTrapezoid.getRightPoint();

            /*
             * the order of insertion in the map is:
             * 1st, top trapezoid
             * 2nd, bottom trapezoid
             * 3rd left trapezoid (if it exists) otherwise right trapezoid (if it exists)
             * 4th right trapezoid (if it exists and also does the left trapezoid)
             */
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

            // update split trapezoid neighbors
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

            // if there was a previous split we update the neighbors of the last two trapezoids to the left
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
            /*
             * segment intersects one trapezoid
             * handles splitting in 4 or 2 trapezoids
             */
            if (segment.p1().x() >= splitTrapezoid.getLeftPoint().x() && segment.p2().x() <= splitTrapezoid.getRightPoint().x())
            {
                // assign indexes
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

                // create nodes
                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
                Node leftTrapezoidNode = Node(Node::trapezoid_node, leftTrapezoidIndex);
                Node rightTrapezoidNode = Node(Node::trapezoid_node, rightTrapezoidIndex);

                Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex, leftTrapezoidNodeIndex, rightEndpointNodeIndex);
                Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex, segmentNodeIndex, rightTrapezoidNodeIndex);
                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);


                // add nodes to dag and set node index to the trapezoids
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
            /*
             * first of multiple trapezoids intersected by segment
             * handles splitting in 3 trapezoids with a left trapezoid (unless it coincides with the vertical line)
             */
            else if (segment.p1().x() >= splitTrapezoid.getLeftPoint().x() && segment.p2().x() > splitTrapezoid.getRightPoint().x())
            {
                // assign indexes
                size_t leftEndpointNodeIndex = splitTrapezoid.getNodeIndex();
                size_t segmentNodeIndex = dag.numberOfNodes();
                size_t topTrapezoidNodeIndex = segmentNodeIndex + 1;
                size_t bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                size_t leftTrapezoidNodeIndex = std::numeric_limits<size_t>::max();

                if (leftTrapezoidExists)
                    leftTrapezoidNodeIndex = bottomTrapezoidNodeIndex + 1;

                // create nodes
                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
                Node leftTrapezoidNode = Node(Node::trapezoid_node, leftTrapezoidIndex);

                Node leftEndpointNode = Node(Node::point_node, leftEndpointIndex, leftTrapezoidNodeIndex, segmentNodeIndex);
                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);

                // add nodes to dag and set node index to the trapezoids
                dag.addNodeAtIndex(leftEndpointNode, leftEndpointNodeIndex);
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
            }
            /*
             * trapezoid completely crossed by segment
             * handles splitting in 2 trapezoids
             */
            else if (segment.p1().x() < splitTrapezoid.getLeftPoint().x() && segment.p2().x() > splitTrapezoid.getRightPoint().x())
            {
                // assign indexes
                size_t segmentNodeIndex = splitTrapezoid.getNodeIndex();
                size_t topTrapezoidNodeIndex;
                size_t bottomTrapezoidNodeIndex;

                if (mergeCandidate == 0)
                {
                    if (topTrapezoidIndex != tm.getMergedTrapezoid())
                    {
                        topTrapezoidNodeIndex = dag.numberOfNodes();
                        bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                    }
                    // if the top trapezoid got merged we use the index of the top trapezoid of the previous split
                    else
                    {
                        topTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                        topTrapezoidNodeIndex = tm.getTrapezoidAtIndex(topTrapezoidIndex).getNodeIndex();
                        bottomTrapezoidNodeIndex = dag.numberOfNodes();
                    }
                }
                else
                {
                    topTrapezoidNodeIndex = dag.numberOfNodes();

                    if (bottomTrapezoidIndex != tm.getMergedTrapezoid())
                        bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                    // if the bottom trapezoid got merged we use the index of the bottom trapezoid of the previous split
                    else
                    {
                        bottomTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                        bottomTrapezoidNodeIndex = tm.getTrapezoidAtIndex(bottomTrapezoidIndex).getNodeIndex();
                    }
                }

                // create nodes
                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);
                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);

                // add nodes to dag and set node index to the trapezoids
                dag.addNodeAtIndex(segmentNode, segmentNodeIndex);
                dag.addNodeAtIndex(topTrapezoidNode, topTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(topTrapezoidIndex).setNodeIndex(topTrapezoidNodeIndex);
                dag.addNodeAtIndex(bottomTrapezoidNode, bottomTrapezoidNodeIndex);
                tm.getTrapezoidRefAtIndex(bottomTrapezoidIndex).setNodeIndex(bottomTrapezoidNodeIndex);
            }
            /*
             * last of multiple trapezoids intersected by segment
             * handles splitting in 3 trapezoids with a right trapezoid (unless it coincides with the vertical line)
             */
            else if (segment.p1().x() < splitTrapezoid.getLeftPoint().x() && segment.p2().x() <= splitTrapezoid.getRightPoint().x())
            {
                // assign indexes
                size_t rightEndpointNodeIndex = splitTrapezoid.getNodeIndex();
                size_t segmentNodeIndex = dag.numberOfNodes();
                size_t topTrapezoidNodeIndex;
                size_t bottomTrapezoidNodeIndex;
                size_t rightTrapezoidNodeIndex = std::numeric_limits<size_t>::max();

                if (mergeCandidate == 0)
                {
                    if (topTrapezoidIndex != tm.getMergedTrapezoid())
                    {
                        topTrapezoidNodeIndex = segmentNodeIndex + 1;
                        bottomTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                    }
                    // if the top trapezoid got merged we use the index of the top trapezoid of the previous split
                    else
                    {
                        topTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                        topTrapezoidNodeIndex = tm.getTrapezoidAtIndex(topTrapezoidIndex).getNodeIndex();
                        bottomTrapezoidNodeIndex = segmentNodeIndex + 1;
                    }

                    if (rightTrapezoidExists)
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
                    // if the bottom trapezoid got merged we use the index of the bottom trapezoid of the previous split
                    else
                    {
                        bottomTrapezoidIndex = lastTwoTrapezoidsInserted[mergeCandidate];
                        bottomTrapezoidNodeIndex = tm.getTrapezoidAtIndex(bottomTrapezoidIndex).getNodeIndex();

                        if (rightTrapezoidExists)
                            rightTrapezoidNodeIndex = topTrapezoidNodeIndex + 1;
                    }
                }

                // create nodes
                Node topTrapezoidNode = Node(Node::trapezoid_node, topTrapezoidIndex);
                Node bottomTrapezoidNode = Node(Node::trapezoid_node, bottomTrapezoidIndex);
                Node rightTrapezoidNode = Node(Node::trapezoid_node, rightTrapezoidIndex);

                Node rightEndpointNode = Node(Node::point_node, rightEndpointIndex, segmentNodeIndex, rightTrapezoidNodeIndex);

                Node segmentNode = Node(Node::segment_node, segmentIndex, topTrapezoidNodeIndex, bottomTrapezoidNodeIndex);

                // add nodes to dag and set node index to the trapezoids
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
                if (rightTrapezoidExists)
                {
                    dag.addNodeAtIndex(rightTrapezoidNode, rightTrapezoidNodeIndex);
                    tm.getTrapezoidRefAtIndex(rightTrapezoidIndex).setNodeIndex(rightTrapezoidNodeIndex);
                }
            }

            /*
             * we save the top and bottom trapezoid indexes to update their neighbors after the next split and to perform the merge
             * we always save the top trapezoid at index 0 and the bottom one at index 1
             * so we use mergecandidate to know which of the two trapezoids is elegible for merging
             */
            if (cg3::isPointAtLeft(segment, splitTrapezoid.getRightPoint()))
            {
                mergeCandidate = 1;
                lastTwoTrapezoidsInserted[0] = topTrapezoidIndex;
                lastTwoTrapezoidsInserted[1] = bottomTrapezoidIndex;
            }
            else
            {
                mergeCandidate = 0;
                lastTwoTrapezoidsInserted[0] = topTrapezoidIndex;
                lastTwoTrapezoidsInserted[1] = bottomTrapezoidIndex;
            }
        }
    }

    /**
     * @brief incrementalStep executes the incremental step of adding a segment to the map
     * @param tm the trapezoidal map
     * @param dag the directed acyclic graph
     * @param segment the segment being added to the map
     */
    void incrementalStep(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        const cg3::Segment2d orderedSegment = GeometryFunctions::getOrderedSegment(segment);

        std::vector<size_t> intersectedTrapezoidsIndexes = followSegment(tm, dag, orderedSegment);

        splitTrapezoids(tm, dag, intersectedTrapezoidsIndexes, orderedSegment);
    }

    /**
     * @brief colorTrapezoids assign a random color to the trapezoids who don't already have one
     * @param tm the drawable trapezoidal map
     */
    void colorTrapezoids(DrawableTrapezoidalMap& tm)
    {
        for (size_t i = tm.getTrapezoidColors().size(); i < tm.numberOfTrapezoids(); i++)
            tm.setTrapezoidColor(tm.randomColor(), i);
    }

    /**
     * @brief clearStructures clears all map and dag data
     * @param tm the trapezoidal map
     * @param dag the directed acyclic graph
     */
    void clearStructures(TrapezoidalMap& tm, DirectedAcyclicGraph& dag)
    {
        tm.clear();
        dag.clear();
    }
}
