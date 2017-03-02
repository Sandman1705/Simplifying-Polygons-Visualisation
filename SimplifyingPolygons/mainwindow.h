#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setTickMarks(int value);
    void setLabel1(int value);
    void setLabel2(int value);
    void setLabel13(double value);
    void setLabel23(double value);
    void interactiveInputModeChange();
    void interactiveInputPassPoint();
    void showAboutWindow();

private:
    Ui::MainWindow *ui;
    bool m_interactive_input_mode;
};

#endif // MAINWINDOW_H
