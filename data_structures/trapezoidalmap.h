#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

#include <data_structures/trapezoid.h>

class TrapezoidalMap
{
public:
    TrapezoidalMap();

private:
    uint numberOfSegments;
    std::vector<cg3::Segment2d> segments;
    std::vector<cg3::Point2d> endpoints;
    std::vector<Trapezoid> trapezoids;
};

#endif // TRAPEZOIDALMAP_H
