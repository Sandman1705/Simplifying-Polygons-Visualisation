#ifndef SIMPLEPOLYGON_H
#define SIMPLEPOLYGON_H

#include "point.h"
#include <vector>

class SimplePolygon
{
public:
    SimplePolygon(std::vector<Point *> *points);
    void StartAlgorithm(unsigned n);

private:
    std::vector<Point*>& m_points;
};

#endif // SIMPLEPOLYGON_H
