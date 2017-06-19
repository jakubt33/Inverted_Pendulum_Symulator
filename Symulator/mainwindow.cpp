#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pendulum.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->ViewSpace->setScene(scene);

    centerMassPoint = scene->addEllipse(0,0, 10, 10 );
    cart = scene->addRect(0,0,30, 15);
    xLine = scene->addLine(0,0,200, 0);
    yLine = scene->addLine(0,0,0, -200);
    robotLine = scene->addLine(0,0,0,-30);

    qTimer = new QTimer(this);
    connect(qTimer, SIGNAL(timeout()), this, SLOT(UpdateDisplay() ));
    qTimer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateDisplay(void)
{
    oPendulum.Perform();
    PendulumData_T kPendulumData = oPendulum.GetPendulumData();

    cart->setX(kPendulumData.cartPosition*1000);
    robotLine->setRotation(kPendulumData.angularPosition*60);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
//nothing
}
