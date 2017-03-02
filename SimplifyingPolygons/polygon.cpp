#include "polygon.h"

#include <iostream>
#include <fstream>
#include <QtDebug>
#include <cstdlib>
#include <cmath>
#include "ramerdouglaspeucker.h"
#include "visvalingamwhyatt.h"
#include "simplepolygon.h"

Polygon::Polygon()
{
}

Polygon::~Polygon()
{
    for (std::vector<Point*>::iterator it=m_points.begin(); it!=m_points.end(); ++it)
    {
        delete (*it);
    }
}

void Polygon::readFormFile(QString file)
{
    m_points.clear();
    std::fstream myfile(file.toStdString(), std::ios_base::in);

    double x,y;
    while (myfile >> x >> y)
    {
        m_points.push_back(new Point(x,y));
    }

    myfile.close();

    calculate();
}

void Polygon::generateRandom(int n)
{
//    if (n<3)
//        n=3;
//    else if (n>100)
//        n=100;
//    m_points.clear();
//    for(int i=0; i<n; ++i)
//    {
//        int x = qrand() % 100;
//        int y = qrand() % 100;
//        m_points.push_back(new Point(x,y));
//    }
    m_points.clear();
    SimplePolygon alg(&m_points);
    alg.StartAlgorithm(n);

    calculate();
}

void Polygon::interactiveEnd()
{
    calculate();
}

void Polygon::calculate()
{
    updateMinMax();
    /*
    qDebug() << "FULL OUTPUT";
    for(std::vector<Point*>::iterator it=m_points.begin(); it!=m_points.end(); ++it)
    {
        qDebug() << (*it)->get_x() << " " << (*it)->get_y() << " "
                 << (*it)->get_i1() << " " << (*it)->get_d1() << " "
                 << (*it)->get_i2() << " " << (*it)->get_d2();
    }
    qDebug() << "FULL OUTPUT END";
    */

    VisvalingamWhyatt alg2(&m_points);
    alg2.StartAlgorithm();
    RamerDouglasPeucker alg1(&m_points);
    alg1.StartAlgorithm();

    /*
    qDebug() << "FULL OUTPUT";
    for(std::vector<Point*>::iterator it=m_points.begin(); it!=m_points.end(); ++it)
    {
        qDebug() << (*it)->get_x() << " " << (*it)->get_y() << " "
                 << (*it)->get_i1() << " " << (*it)->get_d1() << " "
                 << (*it)->get_i2() << " " << (*it)->get_d2();
    }
    qDebug() << "FULL OUTPUT END";
    */
}

void Polygon::updateMinMax()
{
    std::vector<Point*>::iterator it=m_points.begin();
    m_min_x = (*it)->get_x();
    m_min_y = (*it)->get_y();
    m_max_x = (*it)->get_x();
    m_max_y = (*it)->get_y();
    for(++it; it!=m_points.end(); ++it)
    {
        if ((*it)->get_x() < m_min_x)
            m_min_x = (*it)->get_x();
        else if ((*it)->get_x() > m_max_x)
            m_max_x = (*it)->get_x();

        if ((*it)->get_y() < m_min_y)
            m_min_y = (*it)->get_y();
        else if ((*it)->get_y() > m_max_y)
            m_max_y = (*it)->get_y();
    }
    //qDebug() << "MinX: " << m_min_x;
    //qDebug() << "MinY: " << m_min_y;
    //qDebug() << "MaxX: " << m_max_x;
    //qDebug() << "MaxY: " << m_max_y;
}


/******************* GETERS *******************/

double Polygon::getMinX() const
{
    return m_min_x;
}

double Polygon::getMaxX() const
{
    return m_max_x;
}

double Polygon::getMinY() const
{
    return m_min_y;
}

double Polygon::getMaxY() const
{
    return m_max_y;
}

int Polygon::getSize() const
{
    return m_points.size();
}

void Polygon::addPoint(double x, double y)
{
    m_points.push_back(new Point(x,y));
    if (m_points.size()==1)
    {
        m_min_x=x;
        m_max_x=x;
        m_min_y=y;
        m_max_y=y;
    }
    else
    {
        if (x<m_min_x)
            m_min_x=x;
        else if (x>m_max_x)
            m_max_x=x;
        if (y<m_min_y)
            m_min_y=y;
        else if (y>m_max_y)
            m_max_y=y;
    }
}

/******************* ITERATOR *******************/
/* za ceo poligon */

Polygon::Iterator::Iterator(Polygon* polygon)
    : m_points_pointer(&(polygon->m_points)),
      it (m_points_pointer->begin())
{
}

void Polygon::Iterator::Reset()
{
    it = m_points_pointer->begin();
}

bool Polygon::Iterator::Finished() const
{
    return Current() == End();
}

Point& Polygon::Iterator::GetPoint() const
{
    return **it;
}

Polygon::Iterator& Polygon::Iterator::operator++()
{
    Next();
    return *this;
}

std::vector<Point*>::iterator Polygon::Iterator::Begin() const
{
    return m_points_pointer->begin();
}

std::vector<Point*>::iterator Polygon::Iterator::End() const
{
    return m_points_pointer->end();
}

std::vector<Point*>::iterator Polygon::Iterator::Current() const
{
     return it;
}

std::vector<Point*>::iterator Polygon::Iterator::Next()
{
    if (it == End())
        return it;
    else
        return ++it;
}


/******************* ITERATOR I1 *******************/
/* za Ramer-Douglas-Peucker */

Polygon::Iterator_I1::Iterator_I1(Polygon* polygon, unsigned limit)
    : m_points_pointer(&(polygon->m_points)),
      it (m_points_pointer->begin()),
      m_limit(limit),
      m_done(false)
{
    //if (m_limit < 3)
    //    m_limit = 3;
    Reset();
}

void Polygon::Iterator_I1::Reset()
{
    it = m_points_pointer->begin();
    if (GetPoint().get_i1() >= m_limit)
        Next();
}

bool Polygon::Iterator_I1::Finished() const
{
    return m_done || (Current() == End());
}

Point& Polygon::Iterator_I1::GetPoint() const
{
    return **it;
}

Polygon::Iterator_I1& Polygon::Iterator_I1::operator++()
{
    Next();
    return *this;
}

std::vector<Point*>::iterator Polygon::Iterator_I1::Begin() const
{
    return m_points_pointer->begin();
}

std::vector<Point*>::iterator Polygon::Iterator_I1::End() const
{
    return m_points_pointer->end();
}

std::vector<Point*>::iterator Polygon::Iterator_I1::Current() const
{
     return it;
}

std::vector<Point*>::iterator Polygon::Iterator_I1::Next()
{
    if (it == End())
    {
        m_done = true;
        return it;
    }
    else
    {
        ++it;
        while (!Finished() && (GetPoint().get_i1() >= m_limit))
            ++it;
        return it;
    }
}


/******************* ITERATOR I2 *******************/
/* za VW */

Polygon::Iterator_I2::Iterator_I2(Polygon* polygon, unsigned limit)
    : m_points_pointer(&(polygon->m_points)),
      it (m_points_pointer->begin()),
      m_limit(limit),
      m_done(false)
{
    if (m_limit < 3)
        m_limit = 3;
    Reset();
}

void Polygon::Iterator_I2::Reset()
{
    it = m_points_pointer->begin();
    if (GetPoint().get_i2() >= m_limit)
        Next();
}

bool Polygon::Iterator_I2::Finished() const
{
    return m_done || (Current() == End());
}

Point& Polygon::Iterator_I2::GetPoint() const
{
    return **it;
}

Polygon::Iterator_I2& Polygon::Iterator_I2::operator++()
{
    Next();
    return *this;
}

std::vector<Point*>::iterator Polygon::Iterator_I2::Begin() const
{
    return m_points_pointer->begin();
}

std::vector<Point*>::iterator Polygon::Iterator_I2::End() const
{
    return m_points_pointer->end();
}

std::vector<Point*>::iterator Polygon::Iterator_I2::Current() const
{
     return it;
}

std::vector<Point*>::iterator Polygon::Iterator_I2::Next()
{
    if (it == End())
    {
        m_done = true;
        return it;
    }
    else
    {
        ++it;
        while (!Finished() && (GetPoint().get_i2() >= m_limit))
            ++it;
        return it;
    }
}

