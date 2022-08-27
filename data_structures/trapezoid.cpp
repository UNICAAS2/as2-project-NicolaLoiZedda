#include "trapezoid.h"

Trapezoid::Trapezoid()
{

}

Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint)
{
    Trapezoid::top = top;
    Trapezoid::bottom = bottom;
    Trapezoid::leftPoint = leftPoint;
    Trapezoid::rightPoint = rightPoint;
}

Trapezoid::Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint,
          size_t upperLeftNeighbor, size_t upperRightNeighbor, size_t lowerLeftNeighbor, size_t lowerRightNeighbor)
{
    Trapezoid::top = top;
    Trapezoid::bottom = bottom;
    Trapezoid::leftPoint = leftPoint;
    Trapezoid::rightPoint = rightPoint;

    Trapezoid::upperLeftNeighbor = upperLeftNeighbor;
    Trapezoid::upperRightNeighbor = upperRightNeighbor;
    Trapezoid::lowerLeftNeighbor = lowerLeftNeighbor;
    Trapezoid::lowerRightNeighbor = lowerRightNeighbor;
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

void Trapezoid::setTop(const cg3::Segment2d& top)
{
    Trapezoid::top = top;
}

void Trapezoid::setBottom(const cg3::Segment2d& bottom)
{
    Trapezoid::bottom = bottom;
}

void Trapezoid::setLeftPoint(const cg3::Point2d& leftPoint)
{
    Trapezoid::leftPoint = leftPoint;
}

void Trapezoid::setRightPoint(const cg3::Point2d& rightPoint)
{
    Trapezoid::rightPoint = rightPoint;
}

void Trapezoid::setUpperLeftNeighbor(const size_t upperLeftNeighbor)
{
    Trapezoid::upperLeftNeighbor = upperLeftNeighbor;
}

void Trapezoid::setUpperRightNeighbor(size_t upperRightNeighbor)
{
    Trapezoid::upperRightNeighbor = upperRightNeighbor;
}

void Trapezoid::setLowerLeftNeighbor(size_t lowerLeftNeighbor)
{
    Trapezoid::lowerLeftNeighbor = lowerLeftNeighbor;
}

void Trapezoid::setLowerRightNeighbor(size_t lowerRightNeighbor)
{
    Trapezoid::lowerRightNeighbor = lowerRightNeighbor;
}
