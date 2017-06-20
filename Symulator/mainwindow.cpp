#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pendulum.h"

/* Defined by user*/
#define dPxInCm     ( 4 )

/* Given in cm */
#define dCartWidth      ( 5/*[cm]*/ * dPxInCm )
#define dBodyRadius     ( 5/*[cm]*/ * dPxInCm )

#define dAxisXHorizon   ( 50/*[cm]*/ * dPxInCm )
#define dAxisYHorizon   ( 30/*[cm]*/ * dPxInCm )

#define dTimeInterval   ( 0.05f/*[s]*/ )

/* Private, const defines */
#define dCmInMeter          ( 100 )
#define dCartCenterXOffset  ( dCartWidth / 2.0 )
#define dCartCenterYOffset  ( dCartWidth / 2.0 )
#define dBodyCenterXOffset  ( dBodyRadius / 2.0 )
#define dBodyCenterYOffset  ( dBodyRadius / 2.0 )

#define mMeterToPx(x) ( (x)/*[m]*/ * dCmInMeter * dPxInCm )

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->ViewSpace->setScene(scene);

    /* Draw cartezian axis */
    xLine = scene->addLine(0 - dAxisXHorizon, 0, 0 + dAxisXHorizon, 0);
    yLine = scene->addLine(0, 0 - dAxisYHorizon, 0, 0 + dAxisYHorizon);

    /* Draw center mass point of the body of pendulum */
    centerMassPoint = scene->addEllipse( - dBodyCenterXOffset,
                                         - dBodyCenterYOffset,
                                        dBodyRadius, dBodyRadius );

    /* Draw center mass point of the cart  */
    cart = scene->addRect( mMeterToPx(oPendulum.GetCartPosition()) - dCartCenterXOffset, -dCartCenterYOffset, dCartWidth, dCartWidth);

    /* Connect cart and body wiht a line */
    robotLine = scene->addLine(mMeterToPx(oPendulum.GetCartPosition()) - dCartCenterXOffset,
                               0,
                               mMeterToPx(oPendulum.GetMassAbsoluteXPosition()) - dBodyCenterXOffset,
                               mMeterToPx(oPendulum.GetMassAbsoluteYPosition()) - dBodyCenterYOffset);

    /* Init timer that periodically calls pendulum calculations function and updates display */
    qTimer = new QTimer(this);
    connect(qTimer, SIGNAL(timeout()), this, SLOT(UpdateDisplay() ));
    qTimer->start(dTimeInterval);

    oPendulum.SetTimeInterval(dTimeInterval);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateDisplay(void)
{
    oPendulum.Perform();

    cart->setX( mMeterToPx( oPendulum.GetCartPosition() ) );
    centerMassPoint->setX( mMeterToPx( oPendulum.GetMassAbsoluteXPosition() ) );
    centerMassPoint->setY( mMeterToPx( oPendulum.GetMassAbsoluteYPosition() ) );

    robotLine->setLine( mMeterToPx( oPendulum.GetCartPosition() ), 0,
                        mMeterToPx( oPendulum.GetMassAbsoluteXPosition() ), mMeterToPx( oPendulum.GetMassAbsoluteYPosition() ) );

}
