#ifndef TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H
#define TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H

#include <cg3/geometry/utils2.h>

#include <data_structures/directedacyclicgraph.h>
#include <data_structures/trapezoidalmap.h>

namespace TrapezoidalMapConstructionAndQuery
{
    const std::vector<cg3::Segment2d>& segmentsRandomPermutation(std::vector<cg3::Segment2d>& segments);
    const Trapezoid& searchPointInDAG(DirectedAcyclicGraph& dag, cg3::Point2d& queryPoint);
    const std::vector<Trapezoid> followSegment(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment);
    void buildTrapezoidalMap(TrapezoidalMap tm, DirectedAcyclicGraph dag, cg3::Segment2d& segment);

}

#endif // TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H
