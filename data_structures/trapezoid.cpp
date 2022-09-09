#include "trapezoid.h"

#include <algorithms/geometryfunctions.h>

Trapezoid::Trapezoid() :
    upperLeftNeighbor(std::numeric_limits<size_t>::max()),
    upperRightNeighbor(std::numeric_limits<size_t>::max()),
    lowerLeftNeighbor(std::numeric_limits<size_t>::max()),
    lowerRightNeighbor(std::numeric_limits<size_t>::max()),
    nodeIndex(std::numeric_limits<size_t>::max())
{

}

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

const cg3::Segment2d& Trapezoid::getTop() const
{
    return top;
}

const cg3::Segment2d& Trapezoid::getBottom() const
{
    return bottom;
}

const cg3::Point2d& Trapezoid::getLeftPoint() const
{
    return leftPoint;
}

const cg3::Point2d& Trapezoid::getRightPoint() const
{
    return rightPoint;
}

size_t Trapezoid::getUpperLeftNeighbor() const
{
    return upperLeftNeighbor;
}

size_t Trapezoid::getUpperRightNeighbor() const
{
    return upperRightNeighbor;
}

size_t Trapezoid::getLowerLeftNeighbor() const
{
    return lowerLeftNeighbor;
}

size_t Trapezoid::getLowerRightNeighbor() const
{
    return lowerRightNeighbor;
}

size_t Trapezoid::getNodeIndex() const
{
    return nodeIndex;
}

const std::array<size_t, Trapezoid::NUM_OF_NEIGHBORS> Trapezoid::getNeighbors() const
{
    std::array<size_t, Trapezoid::NUM_OF_NEIGHBORS> neighbors;

    neighbors[0] = this->getUpperLeftNeighbor();
    neighbors[1] = this->getLowerLeftNeighbor();
    neighbors[2] = this->getUpperRightNeighbor();
    neighbors[3] = this->getLowerRightNeighbor();

    return neighbors;
}

const std::array<cg3::Point2d, Trapezoid::NUM_OF_VERTICES> Trapezoid::getVertices() const
{
    std::array<cg3::Point2d, Trapezoid::NUM_OF_VERTICES> vertices;

    vertices[0] = cg3::Point2d(leftPoint.x(), GeometryFunctions::getVerticalLineAndSegmentIntersection(leftPoint.x(), top));     // top left
    vertices[1] = cg3::Point2d(rightPoint.x(), GeometryFunctions::getVerticalLineAndSegmentIntersection(rightPoint.x(), top));                // top right
    vertices[2] = cg3::Point2d(rightPoint.x(), GeometryFunctions::getVerticalLineAndSegmentIntersection(rightPoint.x(), bottom));             // bottom right
    vertices[3] = cg3::Point2d(leftPoint.x(), GeometryFunctions::getVerticalLineAndSegmentIntersection(leftPoint.x(), bottom));               // bottom left

    return vertices;
}

void Trapezoid::setTop(const cg3::Segment2d& top)
{
    this->top = top;
}

void Trapezoid::setBottom(const cg3::Segment2d& bottom)
{
    this->bottom = bottom;
}

void Trapezoid::setLeftPoint(const cg3::Point2d& leftPoint)
{
    this->leftPoint = leftPoint;
}

void Trapezoid::setRightPoint(const cg3::Point2d& rightPoint)
{
    this->rightPoint = rightPoint;
}

void Trapezoid::setUpperLeftNeighbor(const size_t upperLeftNeighbor)
{
    this->upperLeftNeighbor = upperLeftNeighbor;
}

void Trapezoid::setUpperRightNeighbor(const size_t upperRightNeighbor)
{
    this->upperRightNeighbor = upperRightNeighbor;
}

void Trapezoid::setLowerLeftNeighbor(const size_t lowerLeftNeighbor)
{
    this->lowerLeftNeighbor = lowerLeftNeighbor;
}

void Trapezoid::setLowerRightNeighbor(const size_t lowerRightNeighbor)
{
    this->lowerRightNeighbor = lowerRightNeighbor;
}

void Trapezoid::setNodeIndex(const size_t nodeIndex)
{
    this->nodeIndex = nodeIndex;
}
