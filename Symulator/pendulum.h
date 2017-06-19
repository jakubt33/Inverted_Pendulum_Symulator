#ifndef PENDULUM_H
#define PENDULUM_H

#include <QtCore>

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
    PendulumData_T GetPendulumData(void);
    void Perform();

private:
    void integrateForwardRungeKutta4(double step);
    PendulumData_T kPendulumData;
};

#endif // PENDULUM_H
