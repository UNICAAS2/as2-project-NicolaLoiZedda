#include "trapezoidalmap.h"

/**
 * @brief TrapezoidalMap::TrapezoidalMap trapezoidal map constructor
 * @param bbox bounding box
 */
TrapezoidalMap::TrapezoidalMap(const cg3::BoundingBox2 bbox) :
    bbox(bbox),
    mergedTrapezoid(std::numeric_limits<size_t>::max())
{
    // creates starting trapezoid
    Trapezoid bboxTrapezoid = Trapezoid(
                cg3::Segment2d(cg3::Point2d(bbox.min().x(), bbox.max().y()), bbox.max()),       // top
                cg3::Segment2d(bbox.min(), cg3::Point2d(bbox.max().x(), bbox.min().y())),       // bottom
                bbox.min(),                                                                     // leftP
                bbox.max(),                                                                     // rightP
                static_cast<size_t>(0));                                                        // node index

    trapezoids.push_back(bboxTrapezoid);
}

/**
 * @brief TrapezoidalMap::getPointAtIndex gets point stored at the index
 * @param index index
 * @return the point stored at the index
 */
const cg3::Point2d& TrapezoidalMap::getPointAtIndex(const size_t index) const
{
    return points[index];
}

/**
 * @brief TrapezoidalMap::getSegmentAtIndex gets segment stored at the index
 * @param index index
 * @return the segment stored at the index
 */
const cg3::Segment2d& TrapezoidalMap::getSegmentAtIndex(const size_t index) const
{
    return segments[index];
}

/**
 * @brief TrapezoidalMap::getTrapezoidAtIndex gets trapezoid stored at the index
 * @param index index
 * @return the trapezoid stored at the index
 */
const Trapezoid& TrapezoidalMap::getTrapezoidAtIndex(const size_t index) const
{
    return trapezoids[index];
}

/**
 * @brief TrapezoidalMap::getTrapezoidRefAtIndex gets reference to the trapezoid stored at the index
 * @param index index
 * @return reference to the trapezoid stored at the index
 */
Trapezoid& TrapezoidalMap::getTrapezoidRefAtIndex(const size_t index)
{
    return trapezoids[index];
}

/**
 * @brief TrapezoidalMap::getPoints gets point stored in the map
 * @return an array containing all points stored in the map
 */
const std::vector<cg3::Point2d>& TrapezoidalMap::getPoints() const
{
    return points;
}

/**
 * @brief TrapezoidalMap::getSegments gets segment stored in the map
 * @return an array containing all segments stored in the map
 */
const std::vector<cg3::Segment2d>& TrapezoidalMap::getSegments() const
{
    return segments;
}

/**
 * @brief TrapezoidalMap::getTrapezoids gets trapezoid stored in the map
 * @return an array containing all trapezoids stored in the map
 */
const std::vector<Trapezoid>& TrapezoidalMap::getTrapezoids() const
{
    return trapezoids;
}

/**
 * @brief TrapezoidalMap::getMergedTrapezoid gets merged trapezoid
 * @return the index of the merged trapezoid in the map
 */
size_t TrapezoidalMap::getMergedTrapezoid() const
{
    return mergedTrapezoid;
}

/**
 * @brief TrapezoidalMap::getBBox gets bounding box
 * @return the bounding box
 */
const cg3::BoundingBox2& TrapezoidalMap::getBBox() const
{
    return bbox;
}

/**
 * @brief TrapezoidalMap::numberOfPoints gets the number of points stored in the map
 * @return the number of points stored in the map
 */
size_t TrapezoidalMap::numberOfPoints() const
{
    return points.size();
}

/**
 * @brief TrapezoidalMap::numberOfSegments gets the number of segments stored in the map
 * @return the number of segments stored in the map
 */
size_t TrapezoidalMap::numberOfSegments() const
{
    return segments.size();
}

/**
 * @brief TrapezoidalMap::numberOfTrapezoids gets the number of trapezoids stored in the map
 * @return the number of trapezoids stored in the map
 */
size_t TrapezoidalMap::numberOfTrapezoids() const
{
    return trapezoids.size();
}

/**
 * @brief TrapezoidalMap::addPoint adds point at the back of the map
 * @param point point to be added
 */
void TrapezoidalMap::addPoint(const cg3::Point2d& point)
{
    points.push_back(point);
}

/**
 * @brief TrapezoidalMap::addSegment adds segment at the back of the map
 * @param segment segment to be added
 */
void TrapezoidalMap::addSegment(const cg3::Segment2d& segment)
{
    segments.push_back(segment);
}

/**
 * @brief TrapezoidalMap::addTrapezoid adds trapezoid at the back of the map
 * @param trapezoid trapezoid to be added
 */
void TrapezoidalMap::addTrapezoid(const Trapezoid& trapezoid)
{
    trapezoids.push_back(trapezoid);
}

/**
 * @brief TrapezoidalMap::addTrapezoidAtIndex adds trapezoid at a given index
 * @param trapezoid trapezoid to be added
 * @param index where the trapezoid will be inserted
 */
void TrapezoidalMap::addTrapezoidAtIndex(const Trapezoid& trapezoid, const size_t index)
{
    if (index == trapezoids.size())
        trapezoids.push_back(trapezoid);
    else
        trapezoids[index] = trapezoid;
}

/**
 * @brief TrapezoidalMap::setMergedTrapezoid sets merged trapezoid
 * @param index index in the map of the merged trapezoid
 */
void TrapezoidalMap::setMergedTrapezoid(const size_t index)
{
    mergedTrapezoid = index;
}

/**
 * @brief TrapezoidalMap::clear clear all data in the map
 */
void TrapezoidalMap::clear()
{
    points.clear();
    segments.clear();
    trapezoids.clear();
    mergedTrapezoid = std::numeric_limits<size_t>::max();
}
