#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap(const cg3::BoundingBox2 bbox);

    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;


private:

    cg3::Color pointColor;
    cg3::Color segmentColor;
    cg3::Color trapezoidColor;

    unsigned int pointSize;
    unsigned int segmentSize;
    unsigned int trapezoidSize;
};

#endif // DRAWABLETRAPEZOIDALMAP_H
