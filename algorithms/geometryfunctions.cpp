#include "geometryfunctions.h"

namespace GeometryFunctions
{
    double getVerticalLineAndSegmentIntersection(double verticalLineX, cg3::Segment2d segment)
    {
        return ((segment.p2().y() - segment.p1().y()) / (segment.p2().x() - segment.p1().x())) *
                (verticalLineX - segment.p1().x()) + segment.p1().y();
    }

    const cg3::Segment2d getOrderedSegment(const cg3::Segment2d segment)
    {
        if (segment.p1().x() > segment.p2().x())
            return cg3::Segment2d(segment.p2(), segment.p1());
        else
            return segment;
    }
}
