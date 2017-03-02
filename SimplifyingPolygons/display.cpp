#include "display.h"

#include <QPainter>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include "colorwindow.h"

Display::Display(QWidget *parent)
  : QWidget(parent),
    m_polygon(nullptr),
    m_display_start_polygon(true),
    m_display_points(true),
    m_display_alg1_polygon(true),
    m_display_alg2_polygon(true),
    m_input_in_progress(false),
    m_enable_aa(true),
    m_pallete(palette()),
    m_color_background(Qt::gray),
    m_color_polygon(Qt::blue),
    m_color_vertices(Qt::black),
    m_color_rdp(Qt::green),
    m_color_vw(Qt::red)
{
    setAutoFillBackground(true);
}

void Display::paintEvent(QPaintEvent *)
{
    m_pallete.setColor(QPalette::Background, m_color_background);
    setPalette(m_pallete);

    if (m_input_in_progress)
    {
        partialPaint();
        return;
    }
    if ((m_polygon == nullptr) || (m_polygon->getSize()<3))
        return;

    int width = this->width();
    int height = this->height();
    int mod = 5; // margina
    int width_mod = width - 2*mod;
    int height_mod = height - 2*mod;

    double x_trans = 0.0 - m_polygon->getMinX();
    double x_scal = width_mod / (m_polygon->getMaxX()-m_polygon->getMinX());
    double y_trans = 0.0 - m_polygon->getMinY();
    double y_scal = height_mod / (m_polygon->getMaxY()-m_polygon->getMinY());

    double x_mod = mod;
    double y_mod = mod;
    if (x_scal>y_scal)
    {
        x_scal=y_scal;
        x_mod += (width_mod - (m_polygon->getMaxX() - m_polygon->getMinX()) * x_scal) / 2;
    }
    else
    {
        y_scal=x_scal;
        y_mod += (height_mod - (m_polygon->getMaxY() - m_polygon->getMinY()) * y_scal) / 2;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, m_enable_aa);

    QPen pen0,pen1,pen2,penpoint;
    penpoint.setWidth(3); penpoint.setColor(m_color_vertices);
    pen0.setWidth(1); pen0.setColor(m_color_polygon);
    pen1.setWidth(1); pen1.setColor(m_color_rdp);
    pen2.setWidth(1); pen2.setColor(m_color_vw);

    //Crtanje poligona
    if (m_display_start_polygon)
    {
        Polygon::Iterator it(m_polygon);
        Point& tmp = it.GetPoint();
        Point first(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        Point previous(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        for (++it; !it.Finished(); ++it)
        {
               Point& current = it.GetPoint();
               painter.setPen(pen0);
               painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                                (current.get_x()+x_trans)*x_scal+x_mod,(current.get_y()+y_trans)*y_scal+y_mod);
               previous.set_x(current.get_x());
               previous.set_y(current.get_y());
               previous.set_i1(current.get_i1());
               previous.set_i2(current.get_i2());
        }
        painter.setPen(pen0);
        painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                         (first.get_x()+x_trans)*x_scal+x_mod,(first.get_y()+y_trans)*y_scal+y_mod);
    }

    //Crtanje temena
    if (m_display_points)
    {
        painter.setPen(penpoint);
        for (Polygon::Iterator it(m_polygon); !it.Finished(); ++it)
        {
               Point& current = it.GetPoint();
               painter.drawPoint((current.get_x()+x_trans)*x_scal+x_mod,(current.get_y()+y_trans)*y_scal+y_mod);
        }
    }

    //Crtanje rezultata Ramer-Douglas-Peucker
    if (m_display_alg1_polygon)
    {
        painter.setPen(pen1);
        Polygon::Iterator_I1 it(m_polygon,m_alg1_points);
        Point& tmp = it.GetPoint();
        Point first(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        Point previous(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        double min_d1 = tmp.get_d1();
        for (++it; !it.Finished(); ++it)
        {
               Point& current = it.GetPoint();
               painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                                (current.get_x()+x_trans)*x_scal+x_mod,(current.get_y()+y_trans)*y_scal+y_mod);
               previous.set_x(current.get_x());
               previous.set_y(current.get_y());
               previous.set_i1(current.get_i1());
               previous.set_i2(current.get_i2());
               if (current.get_d1()<min_d1)
                   min_d1 = current.get_d1();
        }
        painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                         (first.get_x()+x_trans)*x_scal+x_mod,(first.get_y()+y_trans)*y_scal+y_mod);
        emit setMinTolerance(min_d1);
    }

    //Crtanje rezultata Visvalingam-Whyatt
    if (m_display_alg2_polygon)
    {
        painter.setPen(pen2);
        Polygon::Iterator_I2 it(m_polygon,m_alg2_points);
        Point& tmp = it.GetPoint();
        Point first(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        Point previous(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        unsigned max_index = tmp.get_i2();
        double last_area = tmp.get_d2();
        for (++it; !it.Finished(); ++it)
        {
               Point& current = it.GetPoint();
               painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                                (current.get_x()+x_trans)*x_scal+x_mod,(current.get_y()+y_trans)*y_scal+y_mod);
               previous.set_x(current.get_x());
               previous.set_y(current.get_y());
               previous.set_i1(current.get_i1());
               previous.set_i2(current.get_i2());
               if(current.get_i2()>max_index)
               {
                   max_index=current.get_i2();
                   last_area=current.get_d2();
               }
        }
        painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                         (first.get_x()+x_trans)*x_scal+x_mod,(first.get_y()+y_trans)*y_scal+y_mod);
        emit setLastAreaChange(last_area);
    }
}

void Display::partialPaint()
{
    if (m_polygon->getSize()<2)
        return;
    int width = this->width();
    int height = this->height();
    int mod = 5;
    int width_mod = width - 2*mod;
    int height_mod = height - 2*mod;

    double x_trans = 0 - m_polygon->getMinX();
    double x_scal = width_mod / (m_polygon->getMaxX()-m_polygon->getMinX());
    double y_trans = 0 - m_polygon->getMinY();
    double y_scal = height_mod / (m_polygon->getMaxY()-m_polygon->getMinY());

    double x_mod = mod;
    double y_mod = mod;
    if (x_scal>y_scal)
    {
        x_scal=y_scal;
        x_mod += (width_mod - (m_polygon->getMaxX() - m_polygon->getMinX()) * x_scal) / 2;
    }
    else
    {
        y_scal=x_scal;
        y_mod += (height_mod - (m_polygon->getMaxY() - m_polygon->getMinY()) * y_scal) / 2;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, m_enable_aa);

    QPen pen0,penlast,penpoint;
    penpoint.setWidth(3); penpoint.setColor(m_color_vertices);
    penlast.setWidth(1); penlast.setColor(m_color_vw); penlast.setStyle(Qt::DotLine);
    pen0.setWidth(1); pen0.setColor(m_color_polygon);

    if (m_display_start_polygon)
    {
        Polygon::Iterator it(m_polygon);
        Point& tmp = it.GetPoint();
        Point first(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        Point previous(tmp.get_x(),tmp.get_y(),tmp.get_i1(),tmp.get_i2());
        for (++it; !it.Finished(); ++it)
        {
               Point& current = it.GetPoint();
               painter.setPen(pen0);
               painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                                (current.get_x()+x_trans)*x_scal+x_mod,(current.get_y()+y_trans)*y_scal+y_mod);
               previous.set_x(current.get_x());
               previous.set_y(current.get_y());
               previous.set_i1(current.get_i1());
               previous.set_i2(current.get_i2());
        }
        painter.setPen(penlast);
        painter.drawLine((previous.get_x()+x_trans)*x_scal+x_mod,(previous.get_y()+y_trans)*y_scal+y_mod,
                         (first.get_x()+x_trans)*x_scal+x_mod,(first.get_y()+y_trans)*y_scal+y_mod);
    }

    if (m_display_points)
    {
        painter.setPen(penpoint);
        for (Polygon::Iterator it(m_polygon); !it.Finished(); ++it)
        {
               Point& current = it.GetPoint();
               painter.drawPoint((current.get_x()+x_trans)*x_scal+x_mod,(current.get_y()+y_trans)*y_scal+y_mod);
        }
    }

}



void Display::displayStartPolygon(bool changed)
{
    m_display_start_polygon = changed;
    update();
}

void Display::displayPoints(bool changed)
{
    m_display_points = changed;
    update();
}

void Display::displayAlg1Polygon(bool changed)
{
    m_display_alg1_polygon = changed;
    update();
}

void Display::displayAlg2Polygon(bool changed)
{
    m_display_alg2_polygon = changed;
    update();
}

void Display::setAlg1Points(int value)
{
    m_alg1_points = value;
    update();
}

void Display::setAlg2Points(int value)
{
    m_alg2_points = value;
    update();
}

void Display::readFromFile()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Izaberite fajl"), tr("../TestPrimeri"));
    if (!file.isEmpty())
    {
        delete m_polygon;
        m_polygon = new Polygon();
        m_polygon->readFormFile(file); //procitaj i izracunaj

        //qDebug() << "emit updateSliderValues(m_polygon->getSize()); " << m_polygon->getSize();
        emit updateSliderValues(m_polygon->getSize());
        update();
    }
}

void Display::setRandomPoints(int value)
{
    m_random_points = value;
}

void Display::generateRandom()
{
    delete m_polygon;
    m_polygon = new Polygon();
    m_polygon->generateRandom(m_random_points);
    //qDebug() << "emit updateSliderValues(m_polygon->getSize()); " << m_polygon->getSize();
    emit updateSliderValues(m_polygon->getSize());
    update();
}

void Display::interactiveStart()
{
    delete m_polygon;
    m_polygon = new Polygon();
    m_input_in_progress = true;
    update();
}

void Display::interactiveAddPoint(double x, double y)
{
    m_polygon->addPoint(x,y);
    update();
}

void Display::interactiveFinish()
{
    m_input_in_progress=false;
    if (m_polygon!=nullptr && m_polygon->getSize()>=3)
    {
        m_polygon->interactiveEnd();
        emit updateSliderValues(m_polygon->getSize());
        update();
    }
}

void Display::setColorBackground(QColor color)
{
    m_color_background = color;
}

void Display::setColorVertices(QColor color)
{
    m_color_vertices = color;
}

void Display::setColorPolygon(QColor color)
{
    m_color_polygon = color;
}

void Display::setColorRDP(QColor color)
{
    m_color_rdp = color;
}

void Display::setColorVW(QColor color)
{
    m_color_vw = color;
}

void Display::changeColors()
{
    ColorWindow c(m_color_background,m_color_vertices,m_color_polygon,m_color_rdp,m_color_vw,(Display*)this);
    c.show();
    c.exec();
}

void Display::toggleAA(bool changed)
{
    m_enable_aa = changed;
    update();
}

