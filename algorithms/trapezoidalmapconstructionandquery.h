#ifndef TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H
#define TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H

#include <cg3/geometry/utils2.h>

#include <data_structures/directedacyclicgraph.h>
#include <data_structures/trapezoidalmap.h>

namespace TrapezoidalMapConstructionAndQuery
{
    size_t getTrapezoidFromPoint(const TrapezoidalMap& tm, const DirectedAcyclicGraph& dag, const cg3::Point2d& queryPoint);
    size_t getFirstTrapezoidIntersectedBySegment(const TrapezoidalMap& tm, const DirectedAcyclicGraph& dag, const cg3::Segment2d& newSegment);
    size_t trapezoidIndexToNodeIndex(const TrapezoidalMap& tm, const size_t index);
    size_t nodeIndexToTrapezoidIndex(const DirectedAcyclicGraph& dag, const size_t index);
    const std::vector<size_t> followSegment(const TrapezoidalMap& tm, const DirectedAcyclicGraph& dag, const cg3::Segment2d& segment);
    size_t merge(TrapezoidalMap &tm, const size_t leftTrapezoidIndex, const size_t rightTrapezoidIndex);
    void splitTrapezoids(TrapezoidalMap &tm, DirectedAcyclicGraph &dag, std::vector<size_t> trapezoidIndexes, const cg3::Segment2d &segment);
    void incrementalStep(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment);
    void clearStructures(TrapezoidalMap& tm, DirectedAcyclicGraph& dag);
}

#endif // TRAPEZOIDALMAPCONSTRUCTIONANDQUERY_H
