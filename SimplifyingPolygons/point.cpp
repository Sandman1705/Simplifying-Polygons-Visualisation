#include "point.h"

Point::Point()
{
}

Point::Point(double x, double y)
    : m_x(x), m_y(y), m_index1(1), m_index2(1), m_distance(0.0), m_area(0.0)
{
}

Point::Point(double x, double y, unsigned i1, unsigned i2)
    : m_x(x), m_y(y), m_index1(i1), m_index2(i2), m_distance(0.0), m_area(0.0)
{
}

double Point::get_x() const
{
    return m_x;
}

double Point::get_y() const
{
    return m_y;
}

unsigned Point::get_i1() const
{
    return m_index1;
}

unsigned Point::get_i2() const
{
    return m_index2;
}

double Point::get_d1() const
{
    return m_distance;
}

double Point::get_d2() const
{
    return m_area;
}

void Point::set_x(double x)
{
    m_x=x;
}

void Point::set_y(double y)
{
    m_y=y;
}

void Point::set_i1(double i1)
{
    m_index1=i1;
}

void Point::set_i2(double i2)
{
    m_index2=i2;
}

void Point::set_d1(double d1)
{
    m_distance = d1;
}

void Point::set_d2(double d2)
{
    m_area = d2;
}

