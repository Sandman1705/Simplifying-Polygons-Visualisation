#ifndef RAMERDOUGLASPEUCKER_H
#define RAMERDOUGLASPEUCKER_H

#include "point.h"
#include <vector>

class RamerDouglasPeucker
{
public:
    RamerDouglasPeucker(std::vector<Point *> *points);
    void StartAlgorithm();

protected:
    void RecursiveCall(unsigned left, unsigned right);
    double distancePointFromLine(double x1, double y1, double x2, double y2, double x0, double y0);
    double distanceBetweenPointsSquared(double x1, double y1, double x2, double y2);
    double distancePointFromLineMod(double x1, double y1, double x2, double y2, double x0, double y0);
    inline double square(double a);
    void sort();

private:
    std::vector<Point*>& m_points;
};

#endif // RAMERDOUGLASPEUCKER_H
