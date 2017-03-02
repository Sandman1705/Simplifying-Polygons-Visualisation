#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"
#include <vector>
#include <QString>

class Polygon
{
public:
    Polygon();
    ~Polygon();

    void readFormFile(QString file);
    void generateRandom(int n);
    void interactiveEnd();

    double getMinX() const;
    double getMaxX() const;
    double getMinY() const;
    double getMaxY() const;
    int getSize() const;

    void addPoint(double x, double y);

protected:
    void updateMinMax();
    void calculate();

private:
    std::vector<Point*> m_points;
    double m_min_x;
    double m_min_y;
    double m_max_x;
    double m_max_y;

public:
    class Iterator
    {
    public:
        Iterator(Polygon* polygon);
        void Reset();
        bool Finished() const;
        Point& GetPoint() const;
        Iterator& operator++ ();

    private:
        std::vector<Point*>* m_points_pointer;
        std::vector<Point*>::iterator it;

        inline std::vector<Point*>::iterator Begin() const;
        inline std::vector<Point*>::iterator End() const;
        inline std::vector<Point*>::iterator Current() const;
        inline std::vector<Point*>::iterator Next();
    };

    class Iterator_I1
    {
    public:
        Iterator_I1(Polygon* polygon, unsigned limit);
        void Reset();
        bool Finished() const;
        Point& GetPoint() const;
        Iterator_I1& operator++ ();

    private:
        std::vector<Point*>* m_points_pointer;
        std::vector<Point*>::iterator it;
        unsigned m_limit;
        bool m_done;

        inline std::vector<Point*>::iterator Begin() const;
        inline std::vector<Point*>::iterator End() const;
        inline std::vector<Point*>::iterator Current() const;
        inline std::vector<Point*>::iterator Next();
    };

    class Iterator_I2
    {
    public:
        Iterator_I2(Polygon* polygon, unsigned limit);
        void Reset();
        bool Finished() const;
        Point& GetPoint() const;
        Iterator_I2& operator++ ();

    private:
        std::vector<Point*>* m_points_pointer;
        std::vector<Point*>::iterator it;
        unsigned m_limit;
        bool m_done;

        inline std::vector<Point*>::iterator Begin() const;
        inline std::vector<Point*>::iterator End() const;
        inline std::vector<Point*>::iterator Current() const;
        inline std::vector<Point*>::iterator Next();
    };

};

#endif // POLYGON_H
