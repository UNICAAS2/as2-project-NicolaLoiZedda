#ifndef GEOMETRYFUNCTIONS_H
#define GEOMETRYFUNCTIONS_H

#include <cg3/geometry/utils2.h>

namespace GeometryFunctions
{
    double slope(const cg3::Segment2d segment);
    double getVerticalLineAndSegmentIntersection(double verticalLineX, cg3::Segment2d segment);
    const cg3::Segment2d getOrderedSegment(const cg3::Segment2d segment);
}

#endif // GEOMETRYFUNCTIONS_H
