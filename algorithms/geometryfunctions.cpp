#include "geometryfunctions.h"

namespace GeometryFunctions
{
/**
     * @brief slope calculates the slope of a segment
     * @param segment the segment
     * @return the slope of the segment
     */
    double slope(const cg3::Segment2d segment)
    {
        return (segment.p2().y() - segment.p1().y()) / (segment.p2().x() - segment.p1().x());
    }

    /**
     * @brief getVerticalLineAndSegmentIntersection calculates the y-coordinate of the intersection between a vertical line and a segment
     * @param verticalLineX the x-coordinate of the vertical line
     * @param segment the segment
     * @return the y-coordinate of the intersection between a vertical line and a segment
     */
    double getVerticalLineAndSegmentIntersection(double verticalLineX, cg3::Segment2d segment)
    {
        return (slope(segment)) * (verticalLineX - segment.p1().x()) + segment.p1().y();
    }

    /**
     * @brief getOrderedSegment flips the segment's endpoints if the first endpoint is not the leftmost
     * @param segment the segment
     * @return a segment with left endpoint as first point
     */
    const cg3::Segment2d getOrderedSegment(const cg3::Segment2d segment)
    {
        if (segment.p1().x() > segment.p2().x())
            return cg3::Segment2d(segment.p2(), segment.p1());
        else
            return segment;
    }

}
