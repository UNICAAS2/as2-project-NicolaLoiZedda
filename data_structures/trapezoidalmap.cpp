#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(const cg3::BoundingBox2 bbox) :
    bbox(bbox)
{
    Trapezoid bboxTrapezoid = Trapezoid(
                cg3::Segment2d(cg3::Point2d(bbox.min().x(), bbox.max().y()), bbox.max()),       //top
                cg3::Segment2d(bbox.min(), cg3::Point2d(bbox.max().x(), bbox.min().y())),       //bottom
                bbox.min(),                                                                     //leftP
                bbox.max());                                                                    //rightP

    trapezoids.push_back(bboxTrapezoid);
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

const std::vector<cg3::Point2d>& TrapezoidalMap::getPoints() const
{
    return points;
}

const std::vector<cg3::Segment2d>& TrapezoidalMap::getSegments() const
{
    return segments;
}

const std::vector<Trapezoid>& TrapezoidalMap::getTrapezoids() const
{
    return trapezoids;
}

const cg3::BoundingBox2& TrapezoidalMap::getBBox() const
{
    return bbox;
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

void TrapezoidalMap::clear()
{
    points.clear();
    segments.clear();
    trapezoids.clear();
}
