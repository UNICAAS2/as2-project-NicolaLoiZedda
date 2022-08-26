#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <data_structures/trapezoid.h>

class TrapezoidalMap
{
public:
    TrapezoidalMap();

    // getter methods
    const cg3::Point2d& getEndpointAtIndex(const size_t index) const;
    const cg3::Segment2d& getSegmentAtIndex(const size_t index) const;
    const Trapezoid& getTrapezoidAtIndex(const size_t index) const;

    void addEndpoint(const cg3::Point2d& endpoint);
    void addEndpointAtIndex(const cg3::Point2d& endpoint, const size_t index);
    void addSegment(const cg3::Segment2d& segment);
    void addSegmentAtIndex(const cg3::Segment2d& segment, const size_t index);
    void addTrapezoid(const Trapezoid& trapezoid);
    void addTrapezoidAtIndex(const Trapezoid& trapezoid, const size_t index);

private:
    std::vector<cg3::Point2d> endpoints;
    std::vector<cg3::Segment2d> segments;
    std::vector<Trapezoid> trapezoids;
};

#endif // TRAPEZOIDALMAP_H
