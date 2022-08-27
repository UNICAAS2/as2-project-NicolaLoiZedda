#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap()
{
    Trapezoid trapezoid = Trapezoid();
    trapezoids.push_back(trapezoid);
}

const cg3::Point2d& TrapezoidalMap::getPointAtIndex(const size_t index) const
{
    return points[index];
}

const cg3::Segment2d& TrapezoidalMap::getSegmentAtIndex(const size_t index) const
{
    return segments[index];
}

const Trapezoid& TrapezoidalMap::getTrapezoidAtIndex(const size_t index) const
{
    return trapezoids[index];
}

size_t TrapezoidalMap::numberOfPoints() const
{
    return points.size();
}

size_t TrapezoidalMap::numberOfSegments() const
{
    return segments.size();
}

size_t TrapezoidalMap::numberOfTrapezoids() const
{
    return trapezoids.size();
}


void TrapezoidalMap::addPoint(const cg3::Point2d& point)
{
    points.push_back(point);
}

void TrapezoidalMap::addPointAtIndex(const cg3::Point2d& point, const size_t index)
{
    points[index] = point;
}

void TrapezoidalMap::addSegment(const cg3::Segment2d& segment)
{
    segments.push_back(segment);
}

void TrapezoidalMap::addSegmentAtIndex(const cg3::Segment2d& segment, const size_t index)
{
    segments[index] = segment;
}

void TrapezoidalMap::addTrapezoid(const Trapezoid& trapezoid)
{
    trapezoids.push_back(trapezoid);
}

void TrapezoidalMap::addTrapezoidAtIndex(const Trapezoid& trapezoid, const size_t index)
{
    trapezoids[index] = trapezoid;
}
