#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtWidgets>
#include <QtCore>
#include "pendulum.h"
#include <QGraphicsScene>
#include "charts.h"
#include "fuzzycontroller.h"

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
    void UpdateDisplay(void);
    void Task8ms(void);
    void Task32ms(void);

private slots:
    void on_buttonAddForce_clicked();
    void on_buttonPauseResume_clicked();

    void on_buttonReset_clicked();

    void on_setAngle_clicked();

private:
    void RedrawPendulum(void);

    Ui::MainWindow *ui;
    Pendulum *oPendulum;
    FuzzyController oFuzzyController;
    QTimer *qTimerUpdateDisplay;
    QTimer *qTimerTask8ms;
    QTimer *qTimerTask32ms;

    QGraphicsScene *scene;
    QGraphicsLineItem *xLine;
    QGraphicsLineItem *yLine;
    QGraphicsLineItem *robotLine;
    QGraphicsEllipseItem *centerMassPoint;
    QGraphicsRectItem *cart;

    Charts chartAngle;
    Charts chartPosition;
    Charts chartPWM;
};

#endif // MAINWINDOW_H
