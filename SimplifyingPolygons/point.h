#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point();
    Point(double x, double y);
    Point(double x, double y, unsigned i1, unsigned i2);

    double get_x() const;
    double get_y() const;
    unsigned get_i1() const;
    unsigned get_i2() const;
    double get_d1() const;
    double get_d2() const;

    void set_x(double x);
    void set_y(double y);
    void set_i1(double i1);
    void set_i2(double i2);
    void set_d1(double d1);
    void set_d2(double d2);

protected:

private:
    double m_x;
    double m_y;
    int m_index1;
    int m_index2;
    double m_distance;
    double m_area;
};

#endif // POINT_H
