#include "trapezoid.h"

#include "utils/geometryutils.h"

/**
 * @brief Trapezoid::Trapezoid trapezoid constructor
 */
Trapezoid::Trapezoid() :
    upperLeftNeighbor(std::numeric_limits<size_t>::max()),
    upperRightNeighbor(std::numeric_limits<size_t>::max()),
    lowerLeftNeighbor(std::numeric_limits<size_t>::max()),
    lowerRightNeighbor(std::numeric_limits<size_t>::max()),
    nodeIndex(std::numeric_limits<size_t>::max())
{

}

/**
 * @brief Trapezoid::Trapezoid trapezoid constructor
 * @param top segment bounding the trapezoid from above
 * @param bottom segment bounding the trapezoid from below
 * @param leftPoint endpoint of the segment bounding the trapezoid from left
 * @param rightPoint endpoint of the segment bounding the trapezoid from right
 */
Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint) :
    top(top),
    bottom(bottom),
    leftPoint(leftPoint),
    rightPoint(rightPoint),
    upperLeftNeighbor(std::numeric_limits<size_t>::max()),
    upperRightNeighbor(std::numeric_limits<size_t>::max()),
    lowerLeftNeighbor(std::numeric_limits<size_t>::max()),
    lowerRightNeighbor(std::numeric_limits<size_t>::max()),
    nodeIndex(std::numeric_limits<size_t>::max())
{

}

/**
 * @brief Trapezoid::Trapezoid trapezoid constructor
 * @param top segment bounding the trapezoid from above
 * @param bottom segment bounding the trapezoid from below
 * @param leftPoint endpoint of the segment bounding the trapezoid from left
 * @param rightPoint endpoint of the segment bounding the trapezoid from right
 * @param nodeIndex index of the trapezoid in the dag
 */
Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint, const size_t nodeIndex) :
    top(top),
    bottom(bottom),
    leftPoint(leftPoint),
    rightPoint(rightPoint),
    upperLeftNeighbor(std::numeric_limits<size_t>::max()),
    upperRightNeighbor(std::numeric_limits<size_t>::max()),
    lowerLeftNeighbor(std::numeric_limits<size_t>::max()),
    lowerRightNeighbor(std::numeric_limits<size_t>::max()),
    nodeIndex(nodeIndex)
{

}

/**
 * @brief Trapezoid::Trapezoid trapezoid constructor
 * @param top segment bounding the trapezoid from above
 * @param bottom segment bounding the trapezoid from below
 * @param leftPoint endpoint of the segment bounding the trapezoid from left
 * @param rightPoint endpoint of the segment bounding the trapezoid from right
 * @param upperLeftNeighbor index in the map of the upper left neighbor
 * @param upperRightNeighbor index in the map of the upper right neighbor
 * @param lowerLeftNeighbor index in the map of the lower left neighbor
 * @param lowerRightNeighbor index in the map of the lower right neighbor
 */
Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint,
          const size_t upperLeftNeighbor, const size_t upperRightNeighbor, const size_t lowerLeftNeighbor, const size_t lowerRightNeighbor) :
    top(top),
    bottom(bottom),
    leftPoint(leftPoint),
    rightPoint(rightPoint),
    upperLeftNeighbor(upperLeftNeighbor),
    upperRightNeighbor(upperRightNeighbor),
    lowerLeftNeighbor(lowerLeftNeighbor),
    lowerRightNeighbor(lowerRightNeighbor),
    nodeIndex(std::numeric_limits<size_t>::max())
{

}

/**
 * @brief Trapezoid::Trapezoid trapezoid constructor
 * @param top segment bounding the trapezoid from above
 * @param bottom segment bounding the trapezoid from below
 * @param leftPoint endpoint of the segment bounding the trapezoid from left
 * @param rightPoint endpoint of the segment bounding the trapezoid from right
 * @param upperLeftNeighbor index in the map of the upper left neighbor
 * @param upperRightNeighbor index in the map of the upper right neighbor
 * @param lowerLeftNeighbor index in the map of the lower left neighbor
 * @param lowerRightNeighbor index in the map of the lower right neighbor
 * @param nodeIndex index of the trapezoid in the dag
 */
Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint,
          const size_t upperLeftNeighbor, const size_t upperRightNeighbor, const size_t lowerLeftNeighbor, const size_t lowerRightNeighbor, const size_t nodeIndex) :
    top(top),
    bottom(bottom),
    leftPoint(leftPoint),
    rightPoint(rightPoint),
    upperLeftNeighbor(upperLeftNeighbor),
    upperRightNeighbor(upperRightNeighbor),
    lowerLeftNeighbor(lowerLeftNeighbor),
    lowerRightNeighbor(lowerRightNeighbor),
    nodeIndex(nodeIndex)
{

}

/**
 * @brief Trapezoid::getTop gets top segment
 * @return top segment
 */
const cg3::Segment2d& Trapezoid::getTop() const
{
    return top;
}

/**
 * @brief Trapezoid::getBottom gets bottom segment
 * @return bottom segment
 */
const cg3::Segment2d& Trapezoid::getBottom() const
{
    return bottom;
}

/**
 * @brief Trapezoid::getLeftPoint gets left point
 * @return left point
 */
const cg3::Point2d& Trapezoid::getLeftPoint() const
{
    return leftPoint;
}

/**
 * @brief Trapezoid::getRightPoint gets right point
 * @return right point
 */
const cg3::Point2d& Trapezoid::getRightPoint() const
{
    return rightPoint;
}

/**
 * @brief Trapezoid::getUpperLeftNeighbor gets index of the trapezoid's upper left neighbor
 * @return index of the trapezoid's upper left neighbor in the map
 */
size_t Trapezoid::getUpperLeftNeighbor() const
{
    return upperLeftNeighbor;
}

/**
 * @brief Trapezoid::getUpperRightNeighbor gets index of the trapezoid's upper right neighbor
 * @return index of the trapezoid's upper right neighbor in the map
 */
size_t Trapezoid::getUpperRightNeighbor() const
{
    return upperRightNeighbor;
}

/**
 * @brief Trapezoid::getLowerLeftNeighbor gets index of the trapezoid's lower left neighbor
 * @return index of the trapezoid's lower left neighbor in the map
 */
size_t Trapezoid::getLowerLeftNeighbor() const
{
    return lowerLeftNeighbor;
}

/**
 * @brief Trapezoid::getLowerRightNeighbor gets index of the trapezoid's lower right neighbor
 * @return index of the trapezoid's lower right neighbor in the map
 */
size_t Trapezoid::getLowerRightNeighbor() const
{
    return lowerRightNeighbor;
}

/**
 * @brief Trapezoid::getNodeIndex gets index of the trapezoid in the dag
 * @return index of the trapezoid in the dag
 */
size_t Trapezoid::getNodeIndex() const
{
    return nodeIndex;
}

/**
 * @brief Trapezoid::getVertices gets the trapezoids' vertices
 * @return an array containing each point representing the trapezoids' vertices
 */
const std::array<cg3::Point2d, Trapezoid::NUM_OF_VERTICES> Trapezoid::getVertices() const
{
    std::array<cg3::Point2d, Trapezoid::NUM_OF_VERTICES> vertices;

    // top left vertex
    if (getTop().p1() != getLeftPoint())
        vertices[0] = cg3::Point2d(leftPoint.x(), GeometryUtils::getVerticalLineAndSegmentIntersection(leftPoint.x(), top));
    else
        vertices[0] = getLeftPoint();

    // top right vertex
    if (getTop().p2() != getRightPoint())
        vertices[1] = cg3::Point2d(rightPoint.x(), GeometryUtils::getVerticalLineAndSegmentIntersection(rightPoint.x(), top));
    else
        vertices[1] = getRightPoint();

    // bottom right vertex
    if (getBottom().p2() != getRightPoint())
        vertices[2] = cg3::Point2d(rightPoint.x(), GeometryUtils::getVerticalLineAndSegmentIntersection(rightPoint.x(), bottom));
    else
        vertices[2] = getRightPoint();

    // bottom left vertex
    if (getBottom().p1() != getLeftPoint())
        vertices[3] = cg3::Point2d(leftPoint.x(), GeometryUtils::getVerticalLineAndSegmentIntersection(leftPoint.x(), bottom));
    else
        vertices[3] = getLeftPoint();

    return vertices;
}

/**
 * @brief Trapezoid::setTop sets top segment
 * @param top segment to be set as top segment
 */
void Trapezoid::setTop(const cg3::Segment2d& top)
{
    this->top = top;
}

/**
 * @brief Trapezoid::setBottom sets bottom segment
 * @param bottom segment to be set as bottom segment
 */
void Trapezoid::setBottom(const cg3::Segment2d& bottom)
{
    this->bottom = bottom;
}

/**
 * @brief Trapezoid::setLeftPoint sets left point
 * @param leftPoint sets point to be set as left point
 */
void Trapezoid::setLeftPoint(const cg3::Point2d& leftPoint)
{
    this->leftPoint = leftPoint;
}

/**
 * @brief Trapezoid::setRightPoint sets right point
 * @param rightPoint sets point to be set as right point
 */
void Trapezoid::setRightPoint(const cg3::Point2d& rightPoint)
{
    this->rightPoint = rightPoint;
}

/**
 * @brief Trapezoid::setUpperLeftNeighbor sets upper left neighbor
 * @param upperLeftNeighbor index of the upper left neighbor in the map
 */
void Trapezoid::setUpperLeftNeighbor(const size_t upperLeftNeighbor)
{
    this->upperLeftNeighbor = upperLeftNeighbor;
}

/**
 * @brief Trapezoid::setUpperRightNeighbor sets upper right neighbor
 * @param upperRightNeighbor index of the upper right neighbor in the map
 */
void Trapezoid::setUpperRightNeighbor(const size_t upperRightNeighbor)
{
    this->upperRightNeighbor = upperRightNeighbor;
}

/**
 * @brief Trapezoid::setLowerLeftNeighbor sets lower left neighbor
 * @param lowerLeftNeighbor index of the lower left neighbor in the map
 */
void Trapezoid::setLowerLeftNeighbor(const size_t lowerLeftNeighbor)
{
    this->lowerLeftNeighbor = lowerLeftNeighbor;
}

/**
 * @brief Trapezoid::setLowerRightNeighbor sets lower right neighbor
 * @param lowerRightNeighbor index of the lower right neighbor in the map
 */
void Trapezoid::setLowerRightNeighbor(const size_t lowerRightNeighbor)
{
    this->lowerRightNeighbor = lowerRightNeighbor;
}

/**
 * @brief Trapezoid::setNodeIndex sets node index
 * @param nodeIndex index of the trapezoid in the dag
 */
void Trapezoid::setNodeIndex(const size_t nodeIndex)
{
    this->nodeIndex = nodeIndex;
}
