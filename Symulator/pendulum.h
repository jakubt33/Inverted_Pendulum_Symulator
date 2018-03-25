#ifndef PENDULUM_H
#define PENDULUM_H

#include <QtCore>

#if 0
#define pendulumAngleOffset 3.95f
#else
#define pendulumAngleOffset 0.0f
#endif
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
    void Initialize(void);

    PendulumData_T GetPendulumData(void);
    double GetCartPosition(void);
    double GetMassAbsoluteXPosition(void);
    double GetMassAbsoluteYPosition(void);
    double GetAngularPosition(void);
    double GetAngularVelocity(void);
    double GetOmegaRPM(void);
    void SetTimeInterval( double TimeStep );
    void SetForce( double dForce );
    double GetForce(void);
    void SetAngle(float newAngle);

private:
    void integrateForwardRungeKutta4(double step);
    PendulumData_T kPendulumData;
};

#endif // PENDULUM_H
