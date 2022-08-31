#include "drawable_trapezoidalmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

DrawableTrapezoidalMap::DrawableTrapezoidalMap(const cg3::BoundingBox2 bbox) :
    pointColor(80, 180, 80),
    segmentColor(80, 80, 180),
    trapezoidColor(40, 80, 180),
    pointSize(5),
    segmentSize(3),
    trapezoidSize(2),
    TrapezoidalMap(bbox)
{

}

void DrawableTrapezoidalMap::draw() const
{
/*
    for (const cg3::Point2d& p : getPoints())
        cg3::opengl::drawPoint2(p, pointColor, static_cast<int>(pointSize));

    for (const cg3::Segment2d& seg : getSegments())
        cg3::opengl::drawLine2(seg.p1(), seg.p2(), segmentColor, static_cast<int>(segmentSize));
*/
    for (const Trapezoid& t : getTrapezoids())
        cg3::opengl::drawQuad2(t.getVertices(), trapezoidColor, static_cast<int>(trapezoidSize), false);
}

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = this->getBBox();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = this->getBBox();
    return boundingBox.diag();
}
