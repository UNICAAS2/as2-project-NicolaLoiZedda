#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <cg3/geometry/bounding_box2.h>
#include <data_structures/trapezoid.h>

/*
 * it contains 3 vectors, one containing all the points, one containing all the segments and another one containing all the trapezoids
 * it also holds the bounding box to generate the starting trapezoid of the map and
 * an index used to locate the last merged trapezoid
 * this allows newly created trapezoids to “take” the index of unused trapezoids guaranteeing the size of the trapezoids vector
 * to be equal to n+1 in the worst case, where n is the number of active trapezoids
 */
class TrapezoidalMap
{
public:

    //constructor
    TrapezoidalMap(const cg3::BoundingBox2 bbox);

    // getters
    const cg3::Point2d& getPointAtIndex(const size_t index) const;
    const cg3::Segment2d& getSegmentAtIndex(const size_t index) const;
    const Trapezoid& getTrapezoidAtIndex(const size_t index) const;
    Trapezoid& getTrapezoidRefAtIndex(const size_t index);

    const std::vector<cg3::Point2d>& getPoints() const;
    const std::vector<cg3::Segment2d>& getSegments() const;
    const std::vector<Trapezoid>& getTrapezoids() const;

    const cg3::BoundingBox2& getBBox() const;
    size_t getMergedTrapezoid() const;

    size_t numberOfPoints() const;
    size_t numberOfSegments() const;
    size_t numberOfTrapezoids() const;

    // setters
    void addPoint(const cg3::Point2d& point);
    void addPointAtIndex(const cg3::Point2d& point, const size_t index);
    void addSegment(const cg3::Segment2d& segment);
    void addSegmentAtIndex(const cg3::Segment2d& segment, const size_t index);
    void addTrapezoid(const Trapezoid& trapezoid);
    void addTrapezoidAtIndex(const Trapezoid& trapezoid, const size_t index);

    void setMergedTrapezoid(const size_t index);

    void clear();

private:

    std::vector<cg3::Point2d> points;
    std::vector<cg3::Segment2d> segments;
    std::vector<Trapezoid> trapezoids;

    cg3::BoundingBox2 bbox;
    size_t mergedTrapezoid;
};

#endif // TRAPEZOIDALMAP_H
