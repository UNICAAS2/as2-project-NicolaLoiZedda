#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <cg3/geometry/bounding_box2.h>
#include <data_structures/trapezoid.h>

class TrapezoidalMap
{
public:
    TrapezoidalMap(const cg3::BoundingBox2 bbox);

    const cg3::Point2d& getPointAtIndex(const size_t index) const;
    const cg3::Segment2d& getSegmentAtIndex(const size_t index) const;
    const Trapezoid& getTrapezoidAtIndex(const size_t index) const;
    Trapezoid& getTrapezoidRefAtIndex(const size_t index);

    const std::vector<cg3::Point2d>& getPoints() const;
    const std::vector<cg3::Segment2d>& getSegments() const;
    const std::vector<Trapezoid>& getTrapezoids() const;

    const cg3::BoundingBox2& getBBox() const;

    size_t numberOfPoints() const;
    size_t numberOfSegments() const;
    size_t numberOfTrapezoids() const;

    void addPoint(const cg3::Point2d& point);
    void addPointAtIndex(const cg3::Point2d& point, const size_t index);
    void addSegment(const cg3::Segment2d& segment);
    void addSegmentAtIndex(const cg3::Segment2d& segment, const size_t index);
    void addTrapezoid(const Trapezoid& trapezoid);
    void addTrapezoidAtIndex(const Trapezoid& trapezoid, const size_t index);

    void clear();

    cg3::BoundingBox2 bbox;

private:
    std::vector<cg3::Point2d> points;
    std::vector<cg3::Segment2d> segments;
    std::vector<Trapezoid> trapezoids;
};

#endif // TRAPEZOIDALMAP_H
