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
#include <memory>
#include "neuralnetwork.h"

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
    void PerformPendulum(void);
    void Task10ms(void);
    void Task32ms(void);

private slots:
    void on_buttonAddForce_clicked();
    void on_buttonPauseResume_clicked();
    void on_buttonReset_clicked();
    void on_setAngle_clicked();

    void on_checkBoxTrainingMode_clicked(bool checked);

private:
    void RedrawPendulum(void);

    Ui::MainWindow *ui;
    NeuralNetwork oNN;
    std::unique_ptr<Pendulum> oPendulum;
    std::unique_ptr<FuzzyController> oFuzzyControllerAngle;
    std::unique_ptr<FuzzyController> oFuzzyControllerPosition;
    QTimer *qTimerPerformPendulum;
    QTimer *qTimerTask10ms;
    QTimer *qTimerTask32ms;

    QGraphicsScene *scene;
    QGraphicsLineItem *xLine;
    QGraphicsLineItem *yLine;
    QGraphicsLineItem *robotLine;
    QGraphicsEllipseItem *centerMassPoint;
    QGraphicsRectItem *cart;

    Charts chartAngle;
    Charts chartPosition;
    Charts chartNn;
    Charts chartPWM;

    bool bEnabledDisplay;
};

#endif // MAINWINDOW_H
