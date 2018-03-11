#ifndef FUZZYCONTROLLER_H
#define FUZZYCONTROLLER_H

#include "Fuzzy/fl/Headers.h"

class FuzzyController
{
public:
    FuzzyController();

    float getOutput(){
        engine.process();
        return (float)outputPWM.getValue();
    }
    void  updateInputs(float Angle, float Omega, float Position, float Velocity);
    void  updateInputs(float Angle);

private:
    fl::Engine engine;

    fl::InputVariable inputAngle;
    fl::InputVariable inputOmega;
    fl::InputVariable inputPosition;
    fl::InputVariable inputVelocity;

    fl::RuleBlock ruleBlock;

    fl::OutputVariable outputPWM;
};

#endif // FUZZYCONTROLLER_H
