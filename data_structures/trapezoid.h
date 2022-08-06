#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

class Trapezoid
{
public:
    // constructors
    Trapezoid();
    Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint);
    Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint,
              Trapezoid* upperLeftNeighbor, Trapezoid* upperRightNeighbor, Trapezoid* lowerLeftNeighbor, Trapezoid* lowerRightNeighbor);

    // getters
    const cg3::Segment2d& getTop() const;
    const cg3::Segment2d& getBottom() const;
    const cg3::Point2d& getLeftPoint() const;
    const cg3::Point2d& getRightPoint() const;
    const Trapezoid& getUpperLeftNeighbor() const;
    const Trapezoid& getUpperRightNeighbor() const;
    const Trapezoid& getLowerLeftNeighbor() const;
    const Trapezoid& getLowerRightNeighbor() const;

    // setters
    void setTop(const cg3::Segment2d& top);
    void setBottom(const cg3::Segment2d& bottom);
    void setLeftPoint(const cg3::Point2d& leftPoint);
    void setRightPoint(const cg3::Point2d& rightPoint);
    void setUpperLeftNeighbor(Trapezoid* upperLeftNeighbor);
    void setUpperRightNeighbor(Trapezoid* upperRightNeighbor);
    void setLowerLeftNeighbor(Trapezoid* lowerLeftNeighbor);
    void setLowerRightNeighbor(Trapezoid* lowerRightNeighbor);

private:
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    Trapezoid* upperLeftNeighbor;
    Trapezoid* upperRightNeighbor;
    Trapezoid* lowerLeftNeighbor;
    Trapezoid* lowerRightNeighbor;
};

#endif // TRAPEZOID_H
