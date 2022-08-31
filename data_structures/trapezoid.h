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
              const size_t upperLeftNeighbor, const size_t upperRightNeighbor, const size_t lowerLeftNeighbor, const size_t lowerRightNeighbor);

    static const size_t NUM_OF_VERTICES = 4;

    // getters
    const cg3::Segment2d& getTop() const;
    const cg3::Segment2d& getBottom() const;
    const cg3::Point2d& getLeftPoint() const;
    const cg3::Point2d& getRightPoint() const;
    size_t getUpperLeftNeighbor() const;
    size_t getUpperRightNeighbor() const;
    size_t getLowerLeftNeighbor() const;
    size_t getLowerRightNeighbor() const;

    const std::array<cg3::Point2d, NUM_OF_VERTICES> getVertices() const;

    // setters
    void setTop(const cg3::Segment2d& top);
    void setBottom(const cg3::Segment2d& bottom);
    void setLeftPoint(const cg3::Point2d& leftPoint);
    void setRightPoint(const cg3::Point2d& rightPoint);
    void setUpperLeftNeighbor(const size_t upperLeftNeighbor);
    void setUpperRightNeighbor(const size_t upperRightNeighbor);
    void setLowerLeftNeighbor(const size_t lowerLeftNeighbor);
    void setLowerRightNeighbor(const size_t lowerRightNeighbor);

private:
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    size_t upperLeftNeighbor;
    size_t upperRightNeighbor;
    size_t lowerLeftNeighbor;
    size_t lowerRightNeighbor;
};

#endif // TRAPEZOID_H
