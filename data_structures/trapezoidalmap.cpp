#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap()
{

}

void TrapezoidalMap::addTrapezoid(Trapezoid& trapezoid)
{
    this->trapezoids.push_back(trapezoid);
}

void TrapezoidalMap::addTrapezoidAtIndex(Trapezoid& trapezoid, const uint index)
{
    this->trapezoids[index] = trapezoid;
}

void TrapezoidalMap::splitInFour(Trapezoid& trapezoid, const cg3::Segment2d &segment)
{
    Trapezoid topTrapezoid = Trapezoid();
    Trapezoid bottomTrapezoid = Trapezoid();
    Trapezoid leftTrapezoid = Trapezoid();
    Trapezoid rightTrapezoid = Trapezoid();

    topTrapezoid.setTop(trapezoid.getTop());
    topTrapezoid.setBottom(segment);
    topTrapezoid.setLeftPoint(segment.p1());
    topTrapezoid.setRightPoint(segment.p2());

    bottomTrapezoid.setTop(segment);
    bottomTrapezoid.setBottom(trapezoid.getBottom());
    bottomTrapezoid.setLeftPoint(trapezoid.getLeftPoint());
    bottomTrapezoid.setRightPoint(trapezoid.getRightPoint());

    leftTrapezoid.setTop(trapezoid.getTop());
    leftTrapezoid.setBottom(trapezoid.getBottom());
    leftTrapezoid.setLeftPoint(trapezoid.getLeftPoint());
    leftTrapezoid.setRightPoint(segment.p1());

    rightTrapezoid.setTop(trapezoid.getTop());
    rightTrapezoid.setBottom(trapezoid.getBottom());
    rightTrapezoid.setLeftPoint(segment.p2());
    rightTrapezoid.setRightPoint(trapezoid.getRightPoint());

    //we should replace trapezoid!!!
    addTrapezoid(topTrapezoid);
    addTrapezoid(bottomTrapezoid);
    addTrapezoid(leftTrapezoid);
    addTrapezoid(rightTrapezoid);
}
