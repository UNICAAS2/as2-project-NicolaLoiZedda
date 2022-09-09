#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(const cg3::BoundingBox2 bbox) :
    bbox(bbox)
{
    Trapezoid bboxTrapezoid = Trapezoid(
                cg3::Segment2d(cg3::Point2d(bbox.min().x(), bbox.max().y()), bbox.max()),       //top
                cg3::Segment2d(bbox.min(), cg3::Point2d(bbox.max().x(), bbox.min().y())),       //bottom
                bbox.min(),                                                                     //leftP
                bbox.max(),                                                                     //rightP
                static_cast<size_t>(0));

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

Trapezoid& TrapezoidalMap::getTrapezoidRefAtIndex(const size_t index)
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
/*
void TrapezoidalMap::updateOldNeighbors(std::array<size_t, Trapezoid::NUM_OF_NEIGHBORS> oldNeighbors)
{
    size_t index = oldNeighbors[0];
    for (size_t i = 0; i < oldNeighbors.size(); i++)
    {
        if (oldNeighbors[i] != std::numeric_limits<size_t>::max())
        {
            if (i == 0)
                trapezoid[i].setUpperRightNeighbor();
            else if (i == 1)
                    trapezoid[i].setLowerRightNeighbor();
            else if (i == 2)
                    trapezoid[i].setUpperLeftNeighbor();
            else if (i == 3)
                    trapezoid[i].setLowerLeftNeighbor();
        }
    }
}
*/
void TrapezoidalMap::clear()
{
    points.clear();
    segments.clear();
    trapezoids.clear();
}
