#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
//#include <QtDebug>
#include "aboutwindow.h"

#include <QColor>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_interactive_input_mode(false)
{
    ui->setupUi(this);

    ui->horizontalSlider1->setTickInterval(1);
    ui->horizontalSlider1->setMinimum(2);
    ui->horizontalSlider1->setEnabled(true);
    ui->horizontalSlider2->setTickInterval(1);
    ui->horizontalSlider2->setMinimum(2);
    ui->horizontalSlider2->setEnabled(true);
    ui->lineEditX->setValidator(new QDoubleValidator(-9999,9999,5,this));
    ui->lineEditY->setValidator(new QDoubleValidator(-9999,9999,5,this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTickMarks(int value)
{
    ui->horizontalSlider1->setMaximum(value);
    ui->horizontalSlider2->setMaximum(value);
}

void MainWindow::setLabel1(int value)
{
    ui->label11->setText(QString::number(value));
}

void MainWindow::setLabel2(int value)
{
    if (value==2)
        value=3;
    ui->label21->setText(QString::number(value));
}

void MainWindow::setLabel13(double value)
{
    ui->label13->setText(QString::number(value));
}

void MainWindow::setLabel23(double value)
{
    ui->label23->setText(QString::number(value));
}

void MainWindow::interactiveInputModeChange()
{
    m_interactive_input_mode = !m_interactive_input_mode;
    if (m_interactive_input_mode)
    {
        ui->pushButtonUnos1->setDisabled(true);
        ui->pushButtonUnos2->setDisabled(true);
        ui->pushButtonUnos30->setText("Kraj");
        ui->pushButtonUnos31->setEnabled(true);
        ui->lineEditX->setEnabled(true);
        ui->lineEditY->setEnabled(true);
        ui->display->interactiveStart();
    }
    else
    {
        ui->pushButtonUnos1->setEnabled(true);
        ui->pushButtonUnos2->setEnabled(true);
        ui->pushButtonUnos30->setText("Interaktivan unos");
        ui->pushButtonUnos31->setDisabled(true);
        ui->lineEditX->setDisabled(true);
        ui->lineEditY->setDisabled(true);
        ui->display->interactiveFinish();
    }
}

void MainWindow::interactiveInputPassPoint()
{
    double x = ui->lineEditX->text().toDouble();
    double y = ui->lineEditY->text().toDouble();
    ui->display->interactiveAddPoint(x,y);
}

void MainWindow::showAboutWindow()
{
    AboutWindow a;
    a.setWindowFlags(Qt::Window);
    a.show();
    a.exec();
}
