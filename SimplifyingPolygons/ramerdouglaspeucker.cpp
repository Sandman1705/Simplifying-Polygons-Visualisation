#include "ramerdouglaspeucker.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
//#include <QtDebug>

RamerDouglasPeucker::RamerDouglasPeucker(std::vector<Point*>* points)
    : m_points(*points)
{
}

//Algoritam se primenjuje na poligonalnu linuju.
//Mi nas poligon delimo na dva dela i izvrsavamo algoritam nad oba.
//Podela se vrsi u najudaljenijoj tacki od 0-te.
//Jedan deo je od prve do najudaljenije, a drugi od najudaljenije do prve.
void RamerDouglasPeucker::StartAlgorithm()
{
    //Trazimo najudaljeniju tacku od prve
    std::vector<Point*>::size_type farthest = 1;
    double max_distance = distanceBetweenPointsSquared(m_points[0]->get_x(),m_points[0]->get_y(),
                                                       m_points[1]->get_x(),m_points[1]->get_y());
    for (std::vector<Point*>::size_type i = 2; i < m_points.size(); ++i)
    {
        double distance = distanceBetweenPointsSquared(m_points[0]->get_x(),m_points[0]->get_y(),
                                                       m_points[i]->get_x(),m_points[i]->get_y());
        if (distance > max_distance)
        {
            max_distance = distance;
            farthest = i;
        }
    }

    //Rastojanje prve dve tacke
    double f2d = sqrt(distanceBetweenPointsSquared(m_points[0]->get_x(),m_points[0]->get_y(),
                                                   m_points[farthest]->get_x(),m_points[farthest]->get_y()));
    m_points[0]->set_d1(f2d);        //std::numeric_limits<double>::max() //include<limits>
    m_points[farthest]->set_d1(f2d); //std::numeric_limits<double>::max() //include<limits>
    m_points[0]->set_i1(0);
    m_points[farthest]->set_i1(1);

    // kopiramo prvu tacku radi jednostavnije provere izmedju zadnje tacke i prve
    Point* first_copy = new Point(m_points[0]->get_x(),m_points[0]->get_y());
    m_points.push_back(first_copy);

    // pokrecemo algoritam za oba dela
    RecursiveCall(0,farthest);
    RecursiveCall(farthest,m_points.size()-1);

    // ukalnjamo zadnju tacku koja je zapravo kopija prve
    m_points.pop_back();
    delete first_copy;

    sort();
}

void RamerDouglasPeucker::RecursiveCall(unsigned left, unsigned right)
{
    if (right-left>1)
    {
        double max_distance = 0;
        std::vector<Point*>::size_type farthest = left+1;
        for(std::vector<Point*>::size_type i = left+1; i<right; ++i)
        {

            double distance = distancePointFromLineMod(m_points[left]->get_x(),m_points[left]->get_y(),
                                                       m_points[right]->get_x(),m_points[right]->get_y(),
                                                       m_points[i]->get_x(),m_points[i]->get_y());
            if (distance > max_distance)
            {
                max_distance = distance;
                farthest = i;
            }
        }
        //for(std::vector<Point*>::size_type i = left+1; i<right; ++i)
        //{
        //    m_points[i]->set_d1(max_distance);
        //}
        m_points[farthest]->set_d1(max_distance
                                   / sqrt( square(m_points[right]->get_y()-m_points[left]->get_y())
                                         + square(m_points[right]->get_x()-m_points[left]->get_x()) ));
        //qDebug() << max_distance << " " << m_points[farthest]->get_d1();
        RecursiveCall(left,farthest);
        RecursiveCall(farthest,right);
    }
}

double RamerDouglasPeucker::distancePointFromLine(double x1, double y1, double x2, double y2, double x0, double y0)
{
    return abs( (y2-y1)*x0 - (x2-x1)*y0 + x2*y1 - y2*x1 ) / sqrt( square(y2-y1) + square(x2-x1) );
}

double RamerDouglasPeucker::distancePointFromLineMod(double x1, double y1, double x2, double y2, double x0, double y0)
{
    return abs( (y2-y1)*x0 - (x2-x1)*y0 + x2*y1 - y2*x1 );
}

double RamerDouglasPeucker::distanceBetweenPointsSquared(double x1, double y1, double x2, double y2)
{
    return square(x2-x1)+square(y2-y1);
}

inline double RamerDouglasPeucker::square(double a)
{
    return a*a;
}

//koristi se za sortiranje
typedef struct node_eps
{
    unsigned index;
    double eps;

    node_eps(unsigned i, double e)
        : index(i), eps(e)
    {
    }

    int Compare(const node_eps& t) const
    {
        if (eps<t.eps)
            return -1;
        else if (eps==t.eps)
        {
            if (index<t.index)
                return -1;
            else if (index==t.index)
                return 0;
            else
                return 1;
        }
        else
            return 1;

    }

    bool operator==(const node_eps& t) const
    {
        return !Compare(t);
    }

    bool operator<(const node_eps& t) const
    {
        return Compare(t)<0;
    }

}NodeEps;

void RamerDouglasPeucker::sort()
{
    std::vector<NodeEps> sorter;
    sorter.reserve(m_points.size());

    for (std::vector<Point*>::size_type i = 0; i < m_points.size(); ++i)
    {
        sorter.push_back( NodeEps(i,m_points[i]->get_d1()) );
    }

    std::sort(sorter.rbegin(),sorter.rend());

    for (std::vector<NodeEps>::size_type i = 0; i < sorter.size(); ++i)
    {
        m_points[ sorter[i].index ]->set_i1(i);
    }

}
