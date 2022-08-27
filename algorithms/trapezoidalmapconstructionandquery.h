#ifndef TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H
#define TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H

#include <cg3/geometry/utils2.h>

#include <data_structures/directedacyclicgraph.h>
#include <data_structures/trapezoidalmap.h>

namespace TrapezoidalMapConstructionAndQuery
{
    void splitInFour(TrapezoidalMap &tm, DirectedAcyclicGraph& dag, const size_t trapezoidIndex, const cg3::Segment2d &segment);
    size_t getTrapezoidFromPoint(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, cg3::Point2d& queryPoint);
    const std::vector<size_t> followSegment(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment);
    void incrementalStep(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, cg3::Segment2d& segment);
}

#endif // TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H
