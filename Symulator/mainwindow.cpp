#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pendulum.h"
#include "Embed/PID/PID.h"
#include "Embed/Motors/Motors.h"

/* Defined by user*/
#define dPxInCm     ( 4 )

/* Given in cm */
#define dCartWidth      ( 5/*[cm]*/ * dPxInCm )
#define dBodyRadius     ( 5/*[cm]*/ * dPxInCm )

#define dAxisXHorizon   ( 50/*[cm]*/ * dPxInCm )
#define dAxisYHorizon   ( 30/*[cm]*/ * dPxInCm )

#define dTimeInterval   ( 1/*[ms]*/ )

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
    this->move(0, 300);

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
    qTimerUpdateDisplay = new QTimer(this);
    connect(qTimerUpdateDisplay, SIGNAL(timeout()), this, SLOT(UpdateDisplay() ));
    oPendulum.SetTimeInterval(dTimeInterval);

    /* Task 8ms */
    qTimerTask8ms = new QTimer(this);
    connect(qTimerTask8ms, SIGNAL(timeout()), this, SLOT(Task8ms() ));

    /* Task 32ms */
    qTimerTask32ms = new QTimer(this);
    connect(qTimerTask32ms, SIGNAL(timeout()), this, SLOT(Task32ms() ));

    InitializeMotors();

    chartAngle.show();
    chartAngle.setWindowTitle("Angle");
    chartAngle.move(0,0);
    chartAngle.setRange(15.0f);

    chartPWM.show();
    chartPWM.setWindowTitle("PWM");
    chartPWM.move(700,0);
    chartPWM.setHeight(500);
    chartPWM.setRange(800.0f);
}

MainWindow::~MainWindow()
{
    delete qTimerUpdateDisplay;
    delete qTimerTask8ms;
    delete qTimerTask32ms;
    delete scene;
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
#define FUZZY_CONTROLLER 0
#define PID_CONTROLLER 1
void MainWindow::Task8ms(void)
{
    /*! Execute standing functionality */
    float PWM;
#if PID_CONTROLLER
    /*! Apply PID filter to motors to get required angle (output of omega regulator) */
    oPID_Angle.ApplyPid( &oPID_Angle.Parameters, -oPendulum.GetAngleDegrees() /*oMpuKalman.AngleFiltered*/ );

    PWM = oPID_Angle.Parameters.OutSignal;

    if     ( 0 < PWM && PWM <  100 ) PWM =  ( PWM / 10 ) * ( PWM / 10 );
    else if( 0 > PWM && PWM > -100 ) PWM = -( PWM / 10 ) * ( PWM / 10 );

    /*! Check if PWM is within boundaries */
    ( 1000.0f < PWM ) ? ( PWM = 1000.0f ) : ( ( -1000.0f > PWM ) ? ( PWM = -1000.0f ) : ( PWM ) );

#elif FUZZY_CONTROLLER
    oFuzzyController.updateInputs(oPendulum.GetAngleDegrees());
    PWM = oFuzzyController.getOutput();
#endif

    oPendulum.SetForce( (double)PWM/40.0 );// PWM/40 is a radius of a wheel. M_max=1000N*mm, F=M/r

    /* Plot diagrams */
    float angle = oPendulum.GetAngleDegrees();
    chartAngle.addData( angle );
    chartPWM.addData( PWM );
}

void MainWindow::Task32ms(void)
{
    /*! Calculate mean omega of the robot */
    //oEncoders.Perform();
    float OmegaMean = (float)oPendulum.GetOmegaRPM();
    //float OmegaMean = ( oEncoders.GetOmegaLeft() + oEncoders.GetOmegaRight() ) / 2;
    //float OmegaDiff = ( oEncoders.GetOmegaLeft() - oEncoders.GetOmegaRight() );

    /*! Apply PID filter to motors to get required omega */
    oPID_Omega.ApplyPid   ( &oPID_Omega.Parameters,    -OmegaMean );
    //oPID_Rotation.ApplyPid( &oPID_Rotation.Parameters, OmegaDiff );
    oPID_Angle.SetDstValue      ( &oPID_Angle.Parameters,       oPID_Omega.Parameters.OutSignal /*+ AngleOffset*/ );
  //oPID_AngleMoving.SetDstValue( &oPID_AngleMoving.Parameters, oPID_Omega.Parameters.OutSignal + AngleOffset );
}

void MainWindow::on_buttonAddForce_clicked()
{
    oPendulum.SetForce(10);
}

void MainWindow::on_buttonPauseResume_clicked()
{
    if(qTimerUpdateDisplay->isActive())
    {
        qTimerTask8ms->stop();
        qTimerTask32ms->stop();
        qTimerUpdateDisplay->stop();
    }else
    {
        qTimerTask8ms->start(8);
        qTimerTask32ms->start(32);
        qTimerUpdateDisplay->start(dTimeInterval);
    }
}
