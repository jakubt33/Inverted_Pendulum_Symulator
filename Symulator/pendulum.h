#ifndef PENDULUM_H
#define PENDULUM_H

#include <QtCore>

#define pendulumAngleOffset 3.95f

typedef struct
{
    double mass;
    double cartMass;
    double rodLength;
    double gravity;
    double linFriction;
    double angFriction;
    double currentForce;
    double previousForce;
    double angularPosition;
    double angularVelocity;
    double cartPosition;
    double cartVelocity;
    double timeStep;
    double timeTag;
} PendulumData_T;

class Pendulum : public QObject
{
    Q_OBJECT

public:
    Pendulum();
    virtual ~Pendulum(){}
    void Perform();

    PendulumData_T GetPendulumData(void);
    double GetCartPosition(void);
    double GetMassAbsoluteXPosition(void);
    double GetMassAbsoluteYPosition(void);
    double GetAngle(void);
    double GetAngleDegrees(void);
    double GetOmegaRPM(void);
    void SetTimeInterval( double TimeStep );
    void SetForce( double dForce );
    double GetForce(void);

private:
    void integrateForwardRungeKutta4(double step);
    PendulumData_T kPendulumData;
};

#endif // PENDULUM_H
