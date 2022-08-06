#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

#include <data_structures/trapezoid.h>
#include <data_structures/directedacyclicgraph.h>

class TrapezoidalMap
{
public:
    TrapezoidalMap();

private:
    uint numberOfSegments;
    std::list<cg3::Segment2d> segments;
    std::list<cg3::Point2d> endpoints;
    std::list<Trapezoid> trapezoids;
    DirectedAcyclicGraph DAG;
};

#endif // TRAPEZOIDALMAP_H
