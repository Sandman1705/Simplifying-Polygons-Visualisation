#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include "polygon.h"
#include <QPalette>
#include <QColor>

class Display : public QWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);

    void interactiveAddPoint(double x, double y);
    void interactiveStart();
    void interactiveFinish();

    void setColorBackground(QColor color);
    void setColorVertices(QColor color);
    void setColorPolygon(QColor color);
    void setColorRDP(QColor color);
    void setColorVW(QColor color);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void partialPaint();

signals:
    void updateSliderValues(int size);
    void setMinTolerance(double min_d1);
    void setLastAreaChange(double last_area);

public slots:
    void displayStartPolygon(bool changed);
    void displayPoints(bool changed);
    void displayAlg1Polygon(bool changed);
    void displayAlg2Polygon(bool changed);
    void setAlg1Points(int value);
    void setAlg2Points(int value);

    void readFromFile();
    void setRandomPoints(int value);
    void generateRandom();

    void changeColors();
    void toggleAA(bool changed);

private:
    Polygon* m_polygon;
    bool m_display_start_polygon;
    bool m_display_points;
    bool m_display_alg1_polygon;
    bool m_display_alg2_polygon;
    bool m_input_in_progress;
    int m_alg1_points;
    int m_alg2_points;
    int m_random_points;
    bool m_enable_aa;

    QPalette m_pallete;
    QColor m_color_background;
    QColor m_color_polygon;
    QColor m_color_vertices;
    QColor m_color_rdp;
    QColor m_color_vw;
};

#endif // DISPLAY_H
