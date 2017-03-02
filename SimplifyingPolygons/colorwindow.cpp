#include "colorwindow.h"
#include "ui_colorwindow.h"
#include <QPalette>
#include <QColorDialog>
#include "display.h"

ColorWindow::ColorWindow(QColor background, QColor vertices, QColor polygon, QColor rdp, QColor vw,
                         Display *display, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ColorWindow),
    m_display(display),
    m_old_background(background),
    m_old_vertices(vertices),
    m_old_polygon(polygon),
    m_old_rdp(rdp),
    m_old_vw(vw),
    m_new_background(background),
    m_new_vertices(vertices),
    m_new_polygon(polygon),
    m_new_rdp(rdp),
    m_new_vw(vw)
{
    ui->setupUi(this);

    putColorOnFrame(ui->frameBackgroundOld,m_old_background);
    putColorOnFrame(ui->frameVerticesOld,m_old_vertices);
    putColorOnFrame(ui->framePolygonOld,m_old_polygon);
    putColorOnFrame(ui->frameRDPOld,m_old_rdp);
    putColorOnFrame(ui->frameVWOld,m_old_vw);

    putColorOnFrame(ui->frameBackgroundNew,m_new_background);
    putColorOnFrame(ui->frameVerticesNew,m_new_vertices);
    putColorOnFrame(ui->framePolygonNew,m_new_polygon);
    putColorOnFrame(ui->frameRDPNew,m_new_rdp);
    putColorOnFrame(ui->frameVWNew,m_new_vw);
}

ColorWindow::~ColorWindow()
{
    delete ui;
}

void ColorWindow::chooseBackground()
{
    QColor new_color = QColorDialog::getColor(m_new_background,this,"Izaberite boju za pozadinu");
    if (new_color.isValid())
        putColorOnFrame(ui->frameBackgroundNew,m_new_background = new_color);
}

void ColorWindow::chooseVertices()
{
    QColor new_color = QColorDialog::getColor(m_new_vertices,this,"Izaberite boju za temena");
    if (new_color.isValid())
        putColorOnFrame(ui->frameVerticesNew,m_new_vertices = new_color);
}

void ColorWindow::choosePolygon()
{
    QColor new_color = QColorDialog::getColor(m_new_polygon,this,"Izaberite boju za ceo poligon");
    if (new_color.isValid())
        putColorOnFrame(ui->framePolygonNew,m_new_polygon = new_color);
}

void ColorWindow::chooseRDP()
{
    QColor new_color = QColorDialog::getColor(m_new_rdp,this,"Izaberite boju za RDP");
    if (new_color.isValid())
        putColorOnFrame(ui->frameRDPNew,m_new_rdp = new_color);
}

void ColorWindow::chooseVW()
{
    QColor new_color = QColorDialog::getColor(m_new_vw,this,"Izaberite boju za VW");
    if (new_color.isValid())
        putColorOnFrame(ui->frameVWNew,m_new_vw = new_color);
}

void ColorWindow::apply()
{
    m_display->setColorBackground(m_new_background);
    m_display->setColorVertices(m_new_vertices);
    m_display->setColorPolygon(m_new_polygon);
    m_display->setColorRDP(m_new_rdp);
    m_display->setColorVW(m_new_vw);
    m_display->update();
    putColorOnFrame(ui->frameBackgroundOld,m_old_background=m_new_background);
    putColorOnFrame(ui->frameVerticesOld,m_old_vertices=m_new_vertices);
    putColorOnFrame(ui->framePolygonOld,m_old_polygon=m_new_polygon);
    putColorOnFrame(ui->frameRDPOld,m_old_rdp=m_new_rdp);
    putColorOnFrame(ui->frameVWOld,m_old_vw=m_new_vw);
}

void ColorWindow::accept()
{
    m_display->setColorBackground(m_new_background);
    m_display->setColorVertices(m_new_vertices);
    m_display->setColorPolygon(m_new_polygon);
    m_display->setColorRDP(m_new_rdp);
    m_display->setColorVW(m_new_vw);
    QDialog::accept();
}

void ColorWindow::chooseDefault()
{
    putColorOnFrame(ui->frameBackgroundNew, m_new_background = Qt::gray );
    putColorOnFrame(ui->frameVerticesNew,   m_new_vertices   = Qt::black);
    putColorOnFrame(ui->framePolygonNew,    m_new_polygon    = Qt::blue );
    putColorOnFrame(ui->frameRDPNew,        m_new_rdp        = Qt::green);
    putColorOnFrame(ui->frameVWNew,         m_new_vw         = Qt::red );
}

void ColorWindow::putColorOnFrame(QFrame* frame, QColor color)
{
    QPalette pallete = frame->palette();
    pallete.setColor(QPalette::Background,color);
    frame->setPalette(pallete);
    frame->setAutoFillBackground(true);
}
