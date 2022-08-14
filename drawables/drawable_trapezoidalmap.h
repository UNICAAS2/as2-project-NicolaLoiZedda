#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap();

    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;


};

#endif // DRAWABLETRAPEZOIDALMAP_H