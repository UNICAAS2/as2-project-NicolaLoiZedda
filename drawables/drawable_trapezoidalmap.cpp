#include "drawable_trapezoidalmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

DrawableTrapezoidalMap::DrawableTrapezoidalMap(const cg3::BoundingBox2 bbox) :
    TrapezoidalMap(bbox),
    pointColor(80, 180, 80),
    segmentColor(200, 5, 5),
    selectedTrapezoid(std::numeric_limits<size_t>::max()),
    pointSize(5),
    segmentSize(3),
    trapezoidSize(1)
{
    colorTrapezoids();
}

void DrawableTrapezoidalMap::draw() const
{
    size_t merged = getMergedTrapezoid();
    uint i = 0;
    for (const Trapezoid& t : getTrapezoids())
    {
        if (i != merged)
        {
            std::array<cg3::Point2d, Trapezoid::NUM_OF_VERTICES> vertices = t.getVertices();
            cg3::Color color;

            cg3::opengl::drawLine2(vertices[0], vertices[3], segmentColor, static_cast<int>(segmentSize));
            cg3::opengl::drawLine2(vertices[1], vertices[2], segmentColor, static_cast<int>(segmentSize));
            //cg3::opengl::drawLine2(vertices[0], vertices[1], segmentColor, static_cast<int>(segmentSize));
            //cg3::opengl::drawLine2(vertices[3], vertices[2], segmentColor, static_cast<int>(segmentSize));
            if (i == selectedTrapezoid)
                color = cg3::Color(200, 200, 200, 255);
            else
                color = cg3::Color(30, 20, 30, 255);
                //color = trapezoidColor[i];
            cg3::opengl::drawQuad2(vertices, color, static_cast<int>(trapezoidSize), true);
        }

        i++;
    }
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

bool DrawableTrapezoidalMap::isColored(const size_t index) const
{
    return coloredVector[index];
}

size_t DrawableTrapezoidalMap::getSelectedTrapezoid() const
{
    return selectedTrapezoid;
}

void DrawableTrapezoidalMap::setColored(const bool colored, const size_t index)
{
    coloredVector[index] = colored;
}

void DrawableTrapezoidalMap::setSelectedTrapezoid(const size_t index)
{
    selectedTrapezoid = index;
}

cg3::Color DrawableTrapezoidalMap::randomColor() const
{
    return cg3::Color(rand() % 256, rand() % 256, rand() % 256);
}

void DrawableTrapezoidalMap::colorTrapezoids()
{
    size_t size = numberOfTrapezoids();

    trapezoidColor.resize(size);
    coloredVector.resize(size);

    for (size_t i = 0; i < size; i++)
    {
        if (!isColored(i))
        {
            trapezoidColor[i] = randomColor();
            setColored(true, i);
        }
    }
}

