#ifndef COLORWINDOW_H
#define COLORWINDOW_H

#include <QDialog>
#include <QColor>
#include <QFrame>

namespace Ui {
class ColorWindow;
}

class Display;

class ColorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ColorWindow(QColor background, QColor vertices, QColor polygon, QColor rdp, QColor vw,
                         Display* display, QWidget *parent = 0);
    ~ColorWindow();

public slots:
    void chooseBackground();
    void chooseVertices();
    void choosePolygon();
    void chooseRDP();
    void chooseVW();
    void apply();
    void accept();
    //void reject();
    void chooseDefault();

protected:
    void putColorOnFrame(QFrame *frame, QColor color);

private:
    Ui::ColorWindow *ui;
    Display* m_display;

    QColor m_old_background;
    QColor m_old_vertices;
    QColor m_old_polygon;
    QColor m_old_rdp;
    QColor m_old_vw;
    QColor m_new_background;
    QColor m_new_vertices;
    QColor m_new_polygon;
    QColor m_new_rdp;
    QColor m_new_vw;
};

#endif // COLORWINDOW_H
