#include "trapezoid.h"

#include <algorithms/geometryfunctions.h>

Trapezoid::Trapezoid()
{

}

Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint) :
    top(top),
    bottom(bottom),
    leftPoint(leftPoint),
    rightPoint(rightPoint)
{

}

Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint,
          size_t upperLeftNeighbor, size_t upperRightNeighbor, size_t lowerLeftNeighbor, size_t lowerRightNeighbor) :
    top(top),
    bottom(bottom),
    leftPoint(leftPoint),
    rightPoint(rightPoint),
    upperLeftNeighbor(upperLeftNeighbor),
    upperRightNeighbor(upperRightNeighbor),
    lowerLeftNeighbor(lowerLeftNeighbor),
    lowerRightNeighbor(lowerRightNeighbor)
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

void Trapezoid::setUpperRightNeighbor(size_t upperRightNeighbor)
{
    this->upperRightNeighbor = upperRightNeighbor;
}

void Trapezoid::setLowerLeftNeighbor(size_t lowerLeftNeighbor)
{
    this->lowerLeftNeighbor = lowerLeftNeighbor;
}

void Trapezoid::setLowerRightNeighbor(size_t lowerRightNeighbor)
{
    this->lowerRightNeighbor = lowerRightNeighbor;
}
