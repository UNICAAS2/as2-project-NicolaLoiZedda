#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:

    // constructor
    DrawableTrapezoidalMap(const cg3::BoundingBox2 bbox);

    // drawable inherited methods
    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;

    // getters
    const cg3::Color& getSelectedColor() const;
    const cg3::Color& getSegmentColor() const;
    const std::vector<cg3::Color>& getTrapezoidColors() const;
    const cg3::Color& getTrapezoidColor(const size_t index) const;

    unsigned int getSegmentSize() const;
    unsigned int getTrapezoidSize() const;

    size_t getSelectedTrapezoid() const;

    // setters
    void setSelectedColor(const cg3::Color& value);
    void setSegmentColor(const cg3::Color& value);
    void setTrapezoidColor(const cg3::Color& trapezoidColor, const size_t index);

    void setSegmentSize(unsigned int value);
    void setTrapezoidSize(unsigned int value);

    void setSelectedTrapezoid(const size_t index);

    cg3::Color randomColor() const;
    void initializeTrapezoidColors();


private:

    cg3::Color selectedColor;
    cg3::Color segmentColor;
    std::vector<cg3::Color> trapezoidColors;

    unsigned int segmentSize;
    unsigned int trapezoidSize;

    size_t selectedTrapezoid;
};

#endif // DRAWABLETRAPEZOIDALMAP_H
