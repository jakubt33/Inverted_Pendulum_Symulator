#include "pendulum.h"
#include <QtCore>
#include <QDebug>
#include "math.h"

#define sqr(x) ((x)*(x))

#define mDegToRad(x) (((x)*M_PI)/180)
#define mRadToDeg(x) (((x)*180)/M_PI)

#define dMillimetersmInMeter    (1000.0)
#define dWheelRadius    (40.0/dMillimetersmInMeter) //[mm]->so it must be divided by dMmInM to get in [m]
#define dWheelLenght    (2.0*dWheelRadius*M_PI)
#define dSpeedToRPM     (1.0/dWheelLenght) //v[m/s] * dSpeedToRPM = Omega[rpm]

#define mLinearVelToOmega(x) ((x)*dSpeedToRPM)

Pendulum::Pendulum()
{ 
    this->kPendulumData.mass = 0.5;
    this->kPendulumData.cartMass = 1.0;
    this->kPendulumData.rodLength = 0.2;
    this->kPendulumData.gravity = 9.8;
    this->kPendulumData.linFriction = 0.01; // Proportional to translational friction force
    this->kPendulumData.angFriction = 0.01; // Proportional to rotational friction force
    this->kPendulumData.currentForce = 0.0;
    this->kPendulumData.previousForce = 0.0;
    this->kPendulumData.angularPosition = mDegToRad(5); // z1
    this->kPendulumData.angularVelocity = 0.0; // z2
    this->kPendulumData.cartPosition = 0.0; // z3
    this->kPendulumData.cartVelocity = 0.0; // z4
    this->kPendulumData.timeStep = 0.05;
    this->kPendulumData.timeTag = 0.0;
}

void Pendulum::integrateForwardRungeKutta4(double step)
{
    double K1=0.0f, K2=0.0f, K3=0.0f, K4=0.0f;
    double L1=0.0f, L2=0.0f, L3=0.0f, L4=0.0f;
    double M1=0.0f, M2=0.0f, M3=0.0f, M4=0.0f;
    double N1=0.0f, N2=0.0f, N3=0.0f, N4=0.0f;
    double m=this->kPendulumData.mass, M=this->kPendulumData.cartMass, l=this->kPendulumData.rodLength, f_lin=this->kPendulumData.linFriction, f_ang=this->kPendulumData.angFriction, g=this->kPendulumData.gravity, h=step;
    double z1=0.0, z2=0.0, z3=0.0, z4=0.0;

    // Integration using Forward Runge-Kutta.
    K1 = this->kPendulumData.angularVelocity;
    L1 = (this->kPendulumData.previousForce*cos(this->kPendulumData.angularPosition) - (M+m)*g*sin(this->kPendulumData.angularPosition) + m*l*cos(this->kPendulumData.angularPosition)*sin(this->kPendulumData.angularPosition)*sqr(this->kPendulumData.angularVelocity) + f_lin*cos(this->kPendulumData.angularPosition)*this->kPendulumData.cartVelocity + (M+m)*f_ang/m*this->kPendulumData.angularVelocity)/(m*l*sqr(cos(this->kPendulumData.angularPosition)) - (M+m)*l);
    M1 = this->kPendulumData.cartVelocity;
    N1 = (this->kPendulumData.previousForce + m*l*sin(this->kPendulumData.angularPosition)*sqr(this->kPendulumData.angularVelocity) - m*g*cos(this->kPendulumData.angularPosition)*sin(this->kPendulumData.angularPosition) + cos(this->kPendulumData.angularPosition)*f_ang*this->kPendulumData.angularVelocity)/(M+m - m*sqr(cos(this->kPendulumData.angularPosition)));

    K2 = this->kPendulumData.angularVelocity + h/2.0*L1;
    L2 = ((this->kPendulumData.previousForce+this->kPendulumData.currentForce)/2.0*cos(this->kPendulumData.angularPosition+h/2.0*K1) - (M+m)*g*sin(this->kPendulumData.angularPosition+h/2.0*K1) + m*l*cos(this->kPendulumData.angularPosition+h/2.0*K1)*sin(this->kPendulumData.angularPosition+h/2.0*K1)*sqr(this->kPendulumData.angularVelocity+h/2.0*L1) + f_lin*cos(this->kPendulumData.angularPosition+h/2.0*K1)*(this->kPendulumData.cartVelocity+h/2.0*N1) + (M+m)*f_ang/m*(this->kPendulumData.angularVelocity+h/2.0*L1))/(m*l*sqr(cos(this->kPendulumData.angularPosition+h/2.0*K1)) - (M+m)*l);
    M2 = this->kPendulumData.cartVelocity + h/2.0*N1;
    N2 = ((this->kPendulumData.previousForce+this->kPendulumData.currentForce)/2.0 + m*l*sin(this->kPendulumData.angularPosition+h/2.0*K1)*sqr(this->kPendulumData.angularVelocity+h/2.0*L1) - m*g*cos(this->kPendulumData.angularPosition+h/2.0*K1)*sin(this->kPendulumData.angularPosition+h/2.0*K1) + cos(this->kPendulumData.angularPosition+h/2.0*K1)*f_ang*(this->kPendulumData.angularVelocity+h/2.0*L1))/(M+m - m*sqr(cos(this->kPendulumData.angularPosition+h/2.0*K1)));

    K3 = this->kPendulumData.angularVelocity + h/2.0*L2;
    L3 = ((this->kPendulumData.previousForce+this->kPendulumData.currentForce)/2.0*cos(this->kPendulumData.angularPosition+h/2.0*K2) - (M+m)*g*sin(this->kPendulumData.angularPosition+h/2.0*K2) + m*l*cos(this->kPendulumData.angularPosition+h/2.0*K2)*sin(this->kPendulumData.angularPosition+h/2.0*K2)*sqr(this->kPendulumData.angularVelocity+h/2.0*L2) + f_lin*cos(this->kPendulumData.angularPosition+h/2.0*K2)*(this->kPendulumData.cartVelocity+h/2.0*N2) + (M+m)*f_ang/m*(this->kPendulumData.angularVelocity+h/2.0*L2))/(m*l*sqr(cos(this->kPendulumData.angularPosition+h/2.0*K2)) - (M+m)*l);
    M3 = this->kPendulumData.cartVelocity + h/2.0*N2;
    N3 = ((this->kPendulumData.previousForce+this->kPendulumData.currentForce)/2.0 + m*l*sin(this->kPendulumData.angularPosition+h/2.0*K2)*sqr(this->kPendulumData.angularVelocity+h/2.0*L2) - m*g*cos(this->kPendulumData.angularPosition+h/2.0*K2)*sin(this->kPendulumData.angularPosition+h/2.0*K2) + cos(this->kPendulumData.angularPosition+h/2.0*K2)*f_ang*(this->kPendulumData.angularVelocity+h/2.0*L2))/(M+m - m*sqr(cos(this->kPendulumData.angularPosition+h/2.0*K2)));

    K4 = this->kPendulumData.angularVelocity + h*L3;
    L4 = (this->kPendulumData.previousForce *cos(this->kPendulumData.angularPosition+h*K3) - (M+m)*g*sin(this->kPendulumData.angularPosition+h*K3) + m*l*cos(this->kPendulumData.angularPosition+h*K3)*sin(this->kPendulumData.angularPosition+h*K3)*sqr(this->kPendulumData.angularVelocity+h*L3) + f_lin*cos(this->kPendulumData.angularPosition+h*K3)*(this->kPendulumData.cartVelocity+h*N3) + (M+m)*f_ang/m*(this->kPendulumData.angularVelocity+h*L3))/(m*l*sqr(cos(this->kPendulumData.angularPosition+h*K3)) - (M+m)*l);
    M4 = this->kPendulumData.cartVelocity + h*N3;
    N4 = (this->kPendulumData.previousForce + m*l*sin(this->kPendulumData.angularPosition+h*K3)*sqr(this->kPendulumData.angularVelocity+h*L3) - m*g*cos(this->kPendulumData.angularPosition+h*K3)*sin(this->kPendulumData.angularPosition+h*K3) + cos(this->kPendulumData.angularPosition+h*K3)*f_ang*(this->kPendulumData.angularVelocity+h*L3))/(M+m - m*sqr(cos(this->kPendulumData.angularPosition+h*K3)));

    z1 = this->kPendulumData.angularPosition + h*(1.0/6.0*K1 + 2.0/6.0*K2 + 2.0/6.0*K3 + 1.0/6.0*K4);
    z2 = this->kPendulumData.angularVelocity + h*(1.0/6.0*L1 + 2.0/6.0*L2 + 2.0/6.0*L3 + 1.0/6.0*L4);
    z3 = this->kPendulumData.cartPosition	   + h*(1.0/6.0*M1 + 2.0/6.0*M2 + 2.0/6.0*M3 + 1.0/6.0*M4);
    z4 = this->kPendulumData.cartVelocity    + h*(1.0/6.0*N1 + 2.0/6.0*N2 + 2.0/6.0*N3 + 1.0/6.0*N4);

    this->kPendulumData.angularPosition = z1;
    this->kPendulumData.angularVelocity = z2;
    this->kPendulumData.cartPosition = z3;
    this->kPendulumData.cartVelocity = z4;
    this->kPendulumData.previousForce = this->kPendulumData.currentForce;
    this->kPendulumData.timeTag += this->kPendulumData.timeStep;
}

void Pendulum::Perform()
{
   integrateForwardRungeKutta4(this->kPendulumData.timeStep);
   //qDebug() << this->kPendulumData.cartPosition;
}

PendulumData_T Pendulum::GetPendulumData(void)
{
    return this->kPendulumData;
}

double Pendulum::GetAngle(void)
{
    return this->kPendulumData.angularPosition;
}

double Pendulum::GetAngleDegrees(void)
{
    //it should return [-180, +180]
    return mRadToDeg(this->kPendulumData.angularPosition);
}

double Pendulum::GetOmegaRPM(void)
{
    return mLinearVelToOmega(this->kPendulumData.cartVelocity);
}

double Pendulum::GetCartPosition(void)
{
    return kPendulumData.cartPosition;
}

/*!
 * \brief Pendulum::GetMassAbsoluteXPosition
 * \return double - absolute X position of mass center point
 */
double Pendulum::GetMassAbsoluteXPosition(void)
{
    return ( kPendulumData.rodLength * sin(kPendulumData.angularPosition) + kPendulumData.cartPosition );
}

/*!
 * \brief Pendulum::GetMassAbsoluteYPosition
 * \return double - absolute Y position of mass center point
 */
double Pendulum::GetMassAbsoluteYPosition(void)
{
    return ( -kPendulumData.rodLength * cos(kPendulumData.angularPosition) );
}

/*!
 * \brief Pendulum::setTimeInterval
 * \param TimeStep  - sets time interval used in calculations [ms]
 */
void Pendulum::SetTimeInterval( double TimeStep )
{
    //calculations are slowered 100 times
    kPendulumData.timeStep = TimeStep/100;
}

/*!
 * \brief  Sets force
 * \param
 */
void Pendulum::SetForce( double dForce )
{
    this->kPendulumData.currentForce = dForce;
}

double Pendulum::GetForce(void)
{
    return this->kPendulumData.currentForce;
}
