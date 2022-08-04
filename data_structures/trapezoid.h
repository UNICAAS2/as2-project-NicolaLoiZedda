#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

class Trapezoid
{
public:
    Trapezoid();

    const cg3::Segment2d getTop();
    const cg3::Segment2d getBottom();
    const cg3::Point2d getLeftPoint();
    const cg3::Point2d getRightPoint();

    const Trapezoid getUpperLeftNeighbor();
    const Trapezoid getUpperRightNeighbor();
    const Trapezoid getLowerLeftNeighbor();
    const Trapezoid getLowerRightNeighbor();

private:
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    Trapezoid upperLeftNeighbor;
    Trapezoid upperRightNeighbor;
    Trapezoid lowerLeftNeighbor;
    Trapezoid lowerRightNeighbor;
};

#endif // TRAPEZOID_H
