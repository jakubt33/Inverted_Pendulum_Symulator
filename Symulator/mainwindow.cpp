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
//    centerMassPoint = scene->addEllipse( - dBodyCenterXOffset,
//                                         - dBodyCenterYOffset,
//                                        dBodyRadius, dBodyRadius );

    /* Draw center mass point of the cart  */
//    cart = scene->addRect( mMeterToPx(oPendulum->GetCartPosition()) - dCartCenterXOffset, -dCartCenterYOffset, dCartWidth, dCartWidth);

    /* Connect cart and body wiht a line */
    robotLine = scene->addLine(mMeterToPx(oPendulum->GetCartPosition()) - dCartCenterXOffset,
                               0,
                               mMeterToPx(oPendulum->GetMassAbsoluteXPosition()) - dBodyCenterXOffset,
                               mMeterToPx(oPendulum->GetMassAbsoluteYPosition()) - dBodyCenterYOffset);


    /* Init timer that periodically calls pendulum calculations function and updates display */
    /* Task 1ms */
    qTimerTask1ms = new QTimer(this);
    connect(qTimerTask1ms, SIGNAL(timeout()), this, SLOT(Task1ms() ));

    /* Task 32ms */
    qTimerTask32ms = new QTimer(this);
    connect(qTimerTask32ms, SIGNAL(timeout()), this, SLOT(Task32ms() ));

    InitializeMotors();

    chartAngle.show();
    chartAngle.setWindowTitle("Angle");
    chartAngle.setLabelName("Ang Pos","Ang Pos Dst", "Ang Vel x10");
    chartAngle.move(0,0);
    chartAngle.setRange(15.0f);

    chartPosition.show();
    chartPosition.setWindowTitle("Position");
    chartPosition.setLabelName("Position [cm]","Position dst", "Velocity[RPM]x5");
    chartPosition.move(0,400);
    chartPosition.setRange(20.0f);

    chartPWM.show();
    chartPWM.setWindowTitle("PWM");
    chartPWM.setLabelName("PWM");
    chartPWM.move(700,0);
    chartPWM.setHeight(500);
    chartPWM.setRange(800.0f);

    chartNn.show();
    chartNn.setWindowTitle("NN");
    chartNn.setLabelName("Reward", "Iterator", "AngleShift");
    chartNn.move(700,0);
    chartNn.setRange(5.5);

    oFuzzyControllerAngle = std::unique_ptr<FuzzyController>{ new FuzzyController(FUZZY_CONTROLLER::AngleRegulation)};
    oFuzzyControllerPosition = std::unique_ptr<FuzzyController>{ new FuzzyController(FUZZY_CONTROLLER::PositionRegulation)};
}

MainWindow::~MainWindow()
{
    delete ui;
    delete qTimerTask1ms;
    delete qTimerTask32ms;
}

void MainWindow::RedrawPendulum(void)
{
//    cart->setX( mMeterToPx( oPendulum->GetCartPosition() ) );
//    centerMassPoint->setX( mMeterToPx( oPendulum->GetMassAbsoluteXPosition() ) );
//    centerMassPoint->setY( mMeterToPx( oPendulum->GetMassAbsoluteYPosition() ) );

    robotLine->setLine( mMeterToPx( oPendulum->GetCartPosition() ), 0,
                        mMeterToPx( oPendulum->GetMassAbsoluteXPosition() ), mMeterToPx( oPendulum->GetMassAbsoluteYPosition() ) );
}

#define FUZZY_CONTROLLER 1
#define NEURO_CONTROLLER 1
#define PID_CONTROLLER   0

void MainWindow::Task1ms(void)
{
    static int8_t prescaler = 0;
    prescaler++;

    oPendulum->Perform();

    /*! Execute standing functionality */
    float PWM;
    /* ====================== 50ms task ==================== */
    if (prescaler % 50 == 0)
    {
#if PID_CONTROLLER
    /*! Apply PID filter to motors to get required angle (output of omega regulator) */
    oPID_Angle.ApplyPid( &oPID_Angle.Parameters, -oPendulum->GetAngleDegrees() /*oMpuKalman.AngleFiltered*/ );

    PWM = oPID_Angle.Parameters.OutSignal;

    if     ( 0 < PWM && PWM <  100 ) PWM =  ( PWM / 10 ) * ( PWM / 10 );
    else if( 0 > PWM && PWM > -100 ) PWM = -( PWM / 10 ) * ( PWM / 10 );

    /*! Check if PWM is within boundaries */
    ( 1000.0f < PWM ) ? ( PWM = 1000.0f ) : ( ( -1000.0f > PWM ) ? ( PWM = -1000.0f ) : ( PWM ) );

#elif FUZZY_CONTROLLER

#if NEURO_CONTROLLER
        /*  ================  neuro part  ====================  */
        if (oNN.isEpochFinished())
        {
            oPendulum->Initialize();
            /* update the list - it does not consume computing power */
            ui->listOfRewardsWidget->addItem(QString::number(oNN.getReward(),'f', 5));
            ui->numberTries->display((int)oNN.getEpochCounter());
            ui->numberWins->display(ui->numberWins->value() + (int)oNN.isWinningConditionReached());

            RedrawPendulum();

            oNN.initNewEpoch();
            oNN.decreaseEpsilon();
        }
        else
        {
            /* Now critic is fed with new data and output of RL NN can be gathered (it means
             * that robot state parameters will be passed through the network to get the output) */
            oNN.learn(oPendulum->GetAngularPosition(), oPendulum->GetAngularVelocity(),
                      oPendulum->GetCartPosition()*100, oPendulum->GetOmegaRPM());
        }
#endif
    }

    /* ====================== 10ms task ==================== */
    if (prescaler % 10 == 0)
    {
        /*  ================  fuzzy part  ====================  */
#if NEURO_CONTROLLER
        oFuzzyControllerAngle->setDesiredPosition(oNN.getAngleShift());
#else
        oFuzzyControllerPosition->updateInputs(oPendulum->GetCartPosition()*100, oPendulum->GetOmegaRPM());
        oFuzzyControllerPosition->execute();
        oFuzzyControllerAngle->setDesiredPosition(oFuzzyControllerPosition->getOutput());
#endif

        oFuzzyControllerAngle->updateInputs(oPendulum->GetAngularPosition(), oPendulum->GetAngularVelocity());
        oFuzzyControllerAngle->execute();
        PWM = oFuzzyControllerAngle->getOutput();
#endif

        oPendulum->SetForce( (double)PWM/40.0 );// PWM/40 is a radius of a wheel. M_max=1000N*mm, F=M/r
    }

    /* ====================== 20ms task ==================== */
    if (prescaler % 20 == 0)
    {

        /*  ==============  diagrams part  ====================  */
        RedrawPendulum();
        static float iterator = 0.0;
        iterator += 0.02;

        chartAngle.addData( oPendulum->GetAngularPosition(),
                            oFuzzyControllerAngle->getDesiredPosition(),
                            oPendulum->GetAngularVelocity()/10.0,
                            iterator);
        chartPosition.addData( oPendulum->GetCartPosition()*100.0,
                               0,
                               oPendulum->GetOmegaRPM()*5,
                               iterator);

        chartNn.addData( oNN.getReward(),
                         oNN.getIterator()/1000,
                         oNN.getAngleShift(),
                         iterator );

        chartPWM.addData( PWM, iterator );

    }

    if (prescaler == 100) prescaler = 0;
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
    if (qTimerTask1ms->isActive())
    {
        qTimerTask1ms->stop();
        //qTimerTask32ms->stop();
    }
    else
    {
        qTimerTask1ms->start(1);
        //qTimerTask32ms->start(32);
    }
}

void MainWindow::on_buttonReset_clicked()
{
    qTimerTask1ms->stop();
    qTimerTask32ms->stop();
    InitializeMotors();

    oPendulum->Initialize();
    RedrawPendulum();
}

void MainWindow::on_setAngle_clicked()
{
    float newAngle = ui->editAngle->text().toFloat();
    if (newAngle == newAngle)
    {
        oPendulum->SetAngle(newAngle);
    }
    RedrawPendulum();

}

void MainWindow::on_checkBoxTrainingMode_clicked(bool checked)
{
    if (checked)
    {
        qTimerTask1ms->stop();

        int startNumberOfEpoch = oNN.getEpochCounter();
        static int lastEpochNumber = startNumberOfEpoch;
        /* simulate 10 epoch */
        while (oNN.getEpochCounter() < startNumberOfEpoch + 100)
        {
            // simulate 1ms task:
            Task1ms();

            if ( lastEpochNumber != oNN.getEpochCounter())
            {
                QApplication::processEvents();
                lastEpochNumber = oNN.getEpochCounter();
            }
        }

        ui->checkBoxTrainingMode->setChecked(false);
    }
}
