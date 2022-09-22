#include "drawable_trapezoidalmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

/**
 * @brief DrawableTrapezoidalMap::DrawableTrapezoidalMap drawable trapezoidal map constructor passes the bbox to the trapezoidal map superclass constructor
 * @param bbox bounding box
 */
DrawableTrapezoidalMap::DrawableTrapezoidalMap(const cg3::BoundingBox2 bbox) :
    TrapezoidalMap(bbox),
    selectedColor(255, 255, 255),
    segmentColor(200, 5, 5),
    segmentSize(3),
    trapezoidSize(1),
    selectedTrapezoid(std::numeric_limits<size_t>::max())
{
    srand(time(nullptr));
    initializeTrapezoidColors();
}

/**
 * @brief DrawableTrapezoidalMap::draw draws all the trapezoids and the vertical lines
 */
void DrawableTrapezoidalMap::draw() const
{
    size_t merged = getMergedTrapezoid();

    uint i = 0;
    for (const Trapezoid& t : getTrapezoids())
    {
        // ignore the merged trapezoid
        if (i != merged)
        {
            std::array<cg3::Point2d, Trapezoid::NUM_OF_VERTICES> vertices = t.getVertices();
            cg3::Color trapezoidColor;

            // if this is the selected trapezoid we use the specific color
            if (i == selectedTrapezoid)
                trapezoidColor = selectedColor;
            // otherwise we use the color associated to the trapezoid
            else
                trapezoidColor = trapezoidColors[i];

            // degenerate case - left side is a point, trapezoid becomes triangle
            if (vertices[0] == vertices[3])
            {
                // we draw the right vertical extension unless it coincides with the bbox boundary
                if (vertices[1].x() != getBBox().max().x())
                    cg3::opengl::drawLine2(vertices[1], vertices[2], segmentColor, static_cast<int>(segmentSize));

                // we draw the triangle
                cg3::opengl::drawTriangle2(vertices[0], vertices[1], vertices[2], trapezoidColor, static_cast<int>(trapezoidSize), true);
            }
            // degenerate case - right side is a point, trapezoid becomes triangle
            else if (vertices[1] == vertices[2])
            {
                // we draw the left vertical extension unless it coincides with the bbox boundary
                if (vertices[0].x() != getBBox().min().x())
                    cg3::opengl::drawLine2(vertices[0], vertices[3], segmentColor, static_cast<int>(segmentSize));

                // we draw the triangle
                cg3::opengl::drawTriangle2(vertices[0], vertices[1], vertices[3], trapezoidColor, static_cast<int>(trapezoidSize), true);
            }
            // default case - both sides are vertical lines
            else
            {
                // we draw the left vertical extension unless it coincides with the bbox boundary
                if (vertices[0].x() != getBBox().min().x())
                    cg3::opengl::drawLine2(vertices[0], vertices[3], segmentColor, static_cast<int>(segmentSize));

                // we draw the right vertical extension unless it coincides with the bbox boundary
                if (vertices[1].x() != getBBox().max().x())
                    cg3::opengl::drawLine2(vertices[1], vertices[2], segmentColor, static_cast<int>(segmentSize));

                // we draw the trapezoid
                cg3::opengl::drawQuad2(vertices, trapezoidColor, static_cast<int>(trapezoidSize), true);
            }
        }

        i++;
    }
}

/**
 * @brief DrawableTrapezoidalMap::sceneCenter gets center of the scene
 * @return a point representing the center of the scene
 */
cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = this->getBBox();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

/**
 * @brief DrawableTrapezoidalMap::sceneRadius gets radius of the scene
 * @return a double representing the radius of the scene
 */
double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = this->getBBox();
    return boundingBox.diag();
}

/**
 * @brief DrawableTrapezoidalMap::getSelectedColor gets the color used when drawing the selected trapezoid
 * @return the color used when drawing the selected trapezoid
 */
const cg3::Color& DrawableTrapezoidalMap::getSelectedColor() const
{
    return selectedColor;
}

/**
 * @brief DrawableTrapezoidalMap::getSegmentColor gets the color used when drawing segments
 * @return the color used when drawing segments
 */
const cg3::Color& DrawableTrapezoidalMap::getSegmentColor() const
{
    return segmentColor;
}

/**
 * @brief DrawableTrapezoidalMap::getTrapezoidColors gets the vector containing all trapezoid colors
 * @return the vector containing all trapezoid colors
 */
const std::vector<cg3::Color>& DrawableTrapezoidalMap::getTrapezoidColors() const
{
    return trapezoidColors;
}

/**
 * @brief DrawableTrapezoidalMap::getTrapezoidColor gets the color of the trapezoid at the index
 * @param index index of the trapezoid in the vector
 * @return the color assigned to the trapezoid at the index in the vector
 */
const cg3::Color& DrawableTrapezoidalMap::getTrapezoidColor(const size_t index) const
{
    return trapezoidColors[index];
}

/**
 * @brief DrawableTrapezoidalMap::getSegmentSize gets width used when drawing segments
 * @return uint which indicates the width used when drawing segments
 */
unsigned int DrawableTrapezoidalMap::getSegmentSize() const
{
    return segmentSize;
}

/**
 * @brief DrawableTrapezoidalMap::getTrapezoidSize gets gets width used when drawing trapezoids
 * @return uint which indicates the width used when drawing trapezoids
 */
unsigned int DrawableTrapezoidalMap::getTrapezoidSize() const
{
    return trapezoidSize;
}

/**
 * @brief DrawableTrapezoidalMap::getSelectedTrapezoid gets trapezoid currently selected
 * @return the index in the map of the trapezoid currently selected
 */
size_t DrawableTrapezoidalMap::getSelectedTrapezoid() const
{
    return selectedTrapezoid;
}

/**
 * @brief DrawableTrapezoidalMap::setSelectedColor sets color used when drawing the selected trapezoid
 * @param selectedColor color
 */
void DrawableTrapezoidalMap::setSelectedColor(const cg3::Color& selectedColor)
{
    this->selectedColor = selectedColor;
}

/**
 * @brief DrawableTrapezoidalMap::setSegmentColorsets color used when drawing segments
 * @param segmentColor color
 */
void DrawableTrapezoidalMap::setSegmentColor(const cg3::Color& segmentColor)
{
    this->segmentColor = segmentColor;
}

/**
 * @brief DrawableTrapezoidalMap::setTrapezoidColor sets color of the trapezoid at the index in the vector
 * @param flag true if the trapezoid is being colored, false otherwise
 * @param color color to use when drawing the trapezoid at the index in the vector
 * @param index index of the trapezoid at the index in the vector
 */
void DrawableTrapezoidalMap::setTrapezoidColor(const cg3::Color& trapezoidColor, const size_t index)
{
    if (index == trapezoidColors.size())
        trapezoidColors.push_back(trapezoidColor);
    else
        trapezoidColors[index] = trapezoidColor;
}

/**
 * @brief DrawableTrapezoidalMap::setSegmentSize sets segment size
 * @param segmentSize segment size
 */
void DrawableTrapezoidalMap::setSegmentSize(unsigned int segmentSize)
{
    this->segmentSize = segmentSize;
}

/**
 * @brief DrawableTrapezoidalMap::setTrapezoidSize sets trapezoid size
 * @param trapezoidSize trapezoid size
 */
void DrawableTrapezoidalMap::setTrapezoidSize(unsigned int trapezoidSize)
{
    this->trapezoidSize = trapezoidSize;
}

/**
 * @brief DrawableTrapezoidalMap::setSelectedTrapezoid sets trapezoid currently selected
 * @param index in the map of the trapezoid currently selected
 */
void DrawableTrapezoidalMap::setSelectedTrapezoid(const size_t index)
{
    selectedTrapezoid = index;
}

/**
 * @brief DrawableTrapezoidalMap::randomColor generates a random color
 * @return a random color
 */
cg3::Color DrawableTrapezoidalMap::randomColor() const
{
    // we try not to generate too light colors to prevent confusions with the selected trapezoid
    return cg3::Color(rand() % 200, rand() % 200, rand() % 200);
}

/**
 * @brief DrawableTrapezoidalMap::initializeTrapezoidColors initializes the trapezoid colors vector with the first trapezoid
 */
void DrawableTrapezoidalMap::initializeTrapezoidColors()
{
    trapezoidColors.resize(1);
    trapezoidColors[0] = randomColor();
}

