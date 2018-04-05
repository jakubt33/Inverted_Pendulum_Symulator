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

#define dAxisXHorizon   ( 60/*[cm]*/ * dPxInCm )
#define dAxisYHorizon   ( 20/*[cm]*/ * dPxInCm )

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

    ui->editForce->setText("200");
    oPendulum = std::unique_ptr<Pendulum>{new Pendulum};

    scene = new QGraphicsScene(this);
    ui->ViewSpace->setScene(scene);
    this->move(650, 520);

    /* Draw cartezian axis */
    xLine = scene->addLine(0 - dAxisXHorizon, 0, 0 + dAxisXHorizon, 0);
    yLine = scene->addLine(0, -dAxisYHorizon, 0, 0 + dAxisYHorizon/5);

    /* Draw center mass point of the body of pendulum */
    centerMassPoint = scene->addEllipse( - dBodyCenterXOffset,
                                         - dBodyCenterYOffset,
                                        dBodyRadius, dBodyRadius );

    /* Draw center mass point of the cart  */
    cart = scene->addRect( mMeterToPx(oPendulum->GetCartPosition()) - dCartCenterXOffset, -dCartCenterYOffset, dCartWidth, dCartWidth);

    /* Connect cart and body wiht a line */
    robotLine = scene->addLine(mMeterToPx(oPendulum->GetCartPosition()) - dCartCenterXOffset,
                               0,
                               mMeterToPx(oPendulum->GetMassAbsoluteXPosition()) - dBodyCenterXOffset,
                               mMeterToPx(oPendulum->GetMassAbsoluteYPosition()) - dBodyCenterYOffset);

    /* Init timer that periodically calls pendulum calculations function and updates display */
    qTimerUpdateDisplay = new QTimer(this);
    connect(qTimerUpdateDisplay, SIGNAL(timeout()), this, SLOT(UpdateDisplay() ));
    oPendulum->SetTimeInterval(dTimeInterval);

    /* Task 8ms */
    qTimerTask8ms = new QTimer(this);
    connect(qTimerTask8ms, SIGNAL(timeout()), this, SLOT(Task8ms() ));

    /* Task 32ms */
    qTimerTask32ms = new QTimer(this);
    connect(qTimerTask32ms, SIGNAL(timeout()), this, SLOT(Task32ms() ));

    InitializeMotors();

    chartAngle.show();
    chartAngle.setWindowTitle("Angle");
    chartAngle.setLabelName("Ang Pos", "Ang Vel x10");
    chartAngle.move(0,0);
    chartAngle.setRange(15.0f);

    chartPosition.show();
    chartPosition.setWindowTitle("Position");
    chartPosition.setLabelName("Position [cm]", "Velocity[RPM]x5");
    chartPosition.move(0,400);
    chartPosition.setRange(20.0f);

    chartPWM.show();
    chartPWM.setWindowTitle("PWM");
    chartPWM.setLabelName("PWM");
    chartPWM.move(700,0);
    chartPWM.setHeight(500);
    chartPWM.setRange(800.0f);

    this->UpdateDisplay();

    oFuzzyControllerAngle = std::unique_ptr<FuzzyController>{ new FuzzyController(FUZZY_CONTROLLER::AngleRegulation)};
    oFuzzyControllerPosition = std::unique_ptr<FuzzyController>{ new FuzzyController(FUZZY_CONTROLLER::PositionRegulation)};
}

MainWindow::~MainWindow()
{
    delete ui;
    delete qTimerUpdateDisplay;
    delete qTimerTask8ms;
    delete qTimerTask32ms;
}

void MainWindow::UpdateDisplay(void)
{
    oPendulum->Perform();
    this->RedrawPendulum();
}

void MainWindow::RedrawPendulum(void)
{
    cart->setX( mMeterToPx( oPendulum->GetCartPosition() ) );
    centerMassPoint->setX( mMeterToPx( oPendulum->GetMassAbsoluteXPosition() ) );
    centerMassPoint->setY( mMeterToPx( oPendulum->GetMassAbsoluteYPosition() ) );

    robotLine->setLine( mMeterToPx( oPendulum->GetCartPosition() ), 0,
                        mMeterToPx( oPendulum->GetMassAbsoluteXPosition() ), mMeterToPx( oPendulum->GetMassAbsoluteYPosition() ) );

    scene->setActivePanel(cart);
}

#define FUZZY_CONTROLLER 1
#define PID_CONTROLLER   0

void MainWindow::Task8ms(void)
{
    /*! Execute standing functionality */
    float PWM;
#if PID_CONTROLLER
    /*! Apply PID filter to motors to get required angle (output of omega regulator) */
    oPID_Angle.ApplyPid( &oPID_Angle.Parameters, -oPendulum->GetAngleDegrees() /*oMpuKalman.AngleFiltered*/ );

    PWM = oPID_Angle.Parameters.OutSignal;

    if     ( 0 < PWM && PWM <  100 ) PWM =  ( PWM / 10 ) * ( PWM / 10 );
    else if( 0 > PWM && PWM > -100 ) PWM = -( PWM / 10 ) * ( PWM / 10 );

    /*! Check if PWM is within boundaries */
    ( 1000.0f < PWM ) ? ( PWM = 1000.0f ) : ( ( -1000.0f > PWM ) ? ( PWM = -1000.0f ) : ( PWM ) );

#elif FUZZY_CONTROLLER
    /*  ================  neuro part  ====================  */
    float angularPosition = oPendulum->GetAngularPosition();
    float angularVelocity = oPendulum->GetAngularVelocity();
    float position = oPendulum->GetCartPosition()*100;
    float velocity = oPendulum->GetOmegaRPM();

    /* Calculate mean squared error from last 1 second */
    oNN.updateInputs(angularPosition, angularVelocity,
                     position, velocity,
                     position*position /*temp error*/ );
    oNN.execute();
    float angleShift = oNN.getOutput();

    /*  ================  fuzzy part  ====================  */
    oFuzzyControllerPosition->updateInputs(position, velocity);
    oFuzzyControllerPosition->execute();
    oFuzzyControllerAngle->setDesiredPosition(oFuzzyControllerPosition->getOutput() + angleShift);

    oFuzzyControllerAngle->updateInputs(angularPosition,angularVelocity);
    oFuzzyControllerAngle->execute();
    PWM = oFuzzyControllerAngle->getOutput();
#endif

    //if(PWM<50) PWM = 0;
    oPendulum->SetForce( (double)PWM/40.0 );// PWM/40 is a radius of a wheel. M_max=1000N*mm, F=M/r


    /* Plot diagrams */
    static float iterator = 0.0;
    iterator += 0.008;
    chartAngle.addData( oPendulum->GetAngularPosition(),
                        oFuzzyControllerAngle->getDesiredPosition(),
                        oPendulum->GetAngularVelocity()/10.0,
                        iterator);
    chartPosition.addData( oPendulum->GetCartPosition()*100.0,
                           0,
                           oPendulum->GetOmegaRPM()*5,
                           iterator);
    chartPWM.addData( PWM, iterator );
}
#define AngleOffset pendulumAngleOffset
void MainWindow::Task32ms(void)
{
#if PID_CONTROLLER
    /*! Calculate mean omega of the robot */
    //oEncoders.Perform();
    float OmegaMean = (float)oPendulum->GetOmegaRPM();
    //float OmegaMean = ( oEncoders.GetOmegaLeft() + oEncoders.GetOmegaRight() ) / 2;
    //float OmegaDiff = ( oEncoders.GetOmegaLeft() - oEncoders.GetOmegaRight() );

    /*! Apply PID filter to motors to get required omega */
    oPID_Omega.ApplyPid   ( &oPID_Omega.Parameters,    -OmegaMean );
    //oPID_Rotation.ApplyPid( &oPID_Rotation.Parameters, OmegaDiff );
    oPID_Angle.SetDstValue      ( &oPID_Angle.Parameters,       oPID_Omega.Parameters.OutSignal + AngleOffset );
    //oPID_AngleMoving.SetDstValue( &oPID_AngleMoving.Parameters, oPID_Omega.Parameters.OutSignal + AngleOffset );
#elif FUZZY_CONTROLLER

#endif
}

void MainWindow::on_buttonAddForce_clicked()
{
    float newForce = ui->editForce->text().toFloat();
    if(newForce == newForce)
    {
        oPendulum->SetForce(newForce);
    }
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

void MainWindow::on_buttonReset_clicked()
{
    qTimerTask8ms->stop();
    qTimerTask32ms->stop();
    qTimerUpdateDisplay->stop();
    InitializeMotors();

    oPendulum->Initialize();
    RedrawPendulum();
    oPendulum->SetTimeInterval(dTimeInterval);
}

void MainWindow::on_setAngle_clicked()
{
    float newAngle = ui->editAngle->text().toFloat();
    if(newAngle == newAngle)
    {
        oPendulum->SetAngle(newAngle);
    }
    RedrawPendulum();
}
