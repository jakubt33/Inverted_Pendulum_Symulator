#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtWidgets>
#include <QtCore>
#include "pendulum.h"
#include <QGraphicsScene>

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

protected:
    void paintEvent( QPaintEvent *e );

private:
    Ui::MainWindow *ui;
    Pendulum oPendulum;
    QTimer *qTimer;

    QGraphicsScene *scene;
    QGraphicsLineItem *xLine;
    QGraphicsLineItem *yLine;
    QGraphicsLineItem *robotLine;
    QGraphicsEllipseItem *centerMassPoint;
    QGraphicsRectItem *cart;
};

#endif // MAINWINDOW_H
