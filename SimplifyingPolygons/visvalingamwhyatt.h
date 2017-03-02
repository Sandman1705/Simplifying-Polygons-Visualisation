#ifndef VISVALINGAMWHYATT_H
#define VISVALINGAMWHYATT_H

#include "point.h"
#include <vector>

class VisvalingamWhyatt
{
public:
    VisvalingamWhyatt(std::vector<Point *> *points);
    void StartAlgorithm();

private:
    std::vector<Point*>& m_points;
};

#endif // VISVALINGAMWHYATT_H
