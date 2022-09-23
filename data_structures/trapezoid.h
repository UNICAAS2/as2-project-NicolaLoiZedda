#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

/*
 * a trapezoid is defined by 2 segments and 2 points which bound it respectively vertically and horizontally
 * it also holds 4 indexes representing each of its possible neighbors
 * finally, it keeps another index which refers to the position of the node associated to the trapezoid in the dag vector
 */
class Trapezoid
{
public:

    // constant attribute
    static const size_t NUM_OF_VERTICES = 4;

    // constructors
    Trapezoid();
    Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint);
    Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint, const size_t nodeIndex);
    Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint,
              const size_t upperLeftNeighbor, const size_t upperRightNeighbor, const size_t lowerLeftNeighbor, const size_t lowerRightNeighbor);
    Trapezoid(const cg3::Segment2d& top, const cg3::Segment2d& bottom, const cg3::Point2d& leftPoint, const cg3::Point2d& rightPoint,
              const size_t upperLeftNeighbor, const size_t upperRightNeighbor, const size_t lowerLeftNeighbor, const size_t lowerRightNeighbor,
              const size_t nodeIndex);

    // getters
    const cg3::Segment2d& getTop() const;
    const cg3::Segment2d& getBottom() const;
    const cg3::Point2d& getLeftPoint() const;
    const cg3::Point2d& getRightPoint() const;
    size_t getUpperLeftNeighbor() const;
    size_t getUpperRightNeighbor() const;
    size_t getLowerLeftNeighbor() const;
    size_t getLowerRightNeighbor() const;
    size_t getNodeIndex() const;

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
    void setNodeIndex(const size_t nodeIndex);

private:

    // segments and points which define the trapezoid
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    // indexes of the trapezoid's neighbors in the map
    size_t upperLeftNeighbor;
    size_t upperRightNeighbor;
    size_t lowerLeftNeighbor;
    size_t lowerRightNeighbor;

    // index of the trapezoid in the dag
    size_t nodeIndex;
};

#endif // TRAPEZOID_H
