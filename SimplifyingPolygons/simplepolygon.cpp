#include "simplepolygon.h"
#include <cmath>
#include <algorithm>
#include <list>

#include <cstdlib>
#include <ctime>

typedef struct node_angle
{
    double x;
    double y;
    double angle;

    node_angle(double x_coord, double y_coord)
        : x(x_coord), y(y_coord)
    {
    }

    void findAngle(double x_start, double y_start)
    {
        angle = atan2(y_start - y, x_start - x);
    }

    int Compare(const node_angle& t) const
    {
        if (angle<t.angle)
            return -1;
        else if (angle==t.angle)
        {
            if (y<t.y)
                return -1;
            else if (y==t.y)
            {
                if (x<t.x)
                    return -1;
                else if (x==t.x)
                    return 0;
                else
                    return 1;
            }
            else
                return 1;
        }
        else
            return 1;
    }

    bool operator==(const node_angle& t) const
    {
        return !Compare(t);
    }

    bool operator<(const node_angle& t) const
    {
        return Compare(t)<0;
    }

} NodeAngle;

SimplePolygon::SimplePolygon(std::vector<Point *> *points)
    : m_points(*points)
{  
    srand(time(NULL));
}

void SimplePolygon::StartAlgorithm(unsigned n)
{
    if (n<3)
        n=3;
    else if (n>500)
        n=500;

    std::list<NodeAngle> nodes;
    for(unsigned i=0; i<n; ++i)
    {
        int x = rand() % 100;
        int y = rand() % 100;
        nodes.push_back(NodeAngle(x,y));
    }

    std::list<NodeAngle>::iterator it;
    std::list<NodeAngle>::iterator bottom_right;
    it=nodes.begin();
    bottom_right=nodes.begin();
    for(++it; it!=nodes.end(); ++it)
    {
        if (it->y < bottom_right->y)
            bottom_right=it;
        else if(it->y == bottom_right->y && it->x > bottom_right->x)
            bottom_right=it;
    }

    NodeAngle start(bottom_right->x,bottom_right->y);
    nodes.erase(bottom_right);

    for(std::list<NodeAngle>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        it->findAngle(start.x,start.y);
    }

    nodes.sort();;

    m_points.clear();
    m_points.push_back(new Point(start.x,start.y));
    for(std::list<NodeAngle>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        m_points.push_back(new Point(it->x,it->y));
    }
}
