#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap()
{
    Trapezoid trapezoid = Trapezoid();
    trapezoids.push_back(trapezoid);
}

const cg3::Point2d& TrapezoidalMap::getEndpointAtIndex(const size_t index) const
{
    return endpoints[index];
}

const cg3::Segment2d& TrapezoidalMap::getSegmentAtIndex(const size_t index) const
{
    return segments[index];
}

const Trapezoid& TrapezoidalMap::getTrapezoidAtIndex(const size_t index) const
{
    return trapezoids[index];
}

void TrapezoidalMap::addEndpoint(const cg3::Point2d& endpoint)
{
    endpoints.push_back(endpoint);
}

void TrapezoidalMap::addEndpointAtIndex(const cg3::Point2d& endpoint, const size_t index)
{
    endpoints[index] = endpoint;
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
