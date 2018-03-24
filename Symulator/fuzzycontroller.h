#ifndef FUZZYCONTROLLER_H
#define FUZZYCONTROLLER_H

#include "Fuzzy/fl/Headers.h"
namespace FUZZY_CONTROLLER {
enum TypeOfRegulation_T { AngleRegulation, PositionRegulation } ;
}

class FuzzyController
{
public:
    FuzzyController(FUZZY_CONTROLLER::TypeOfRegulation_T typeOfRegulation);

    float getOutput();
    void updateInputs(float currentPosition, float currentVelocity);
    void setDesiredPosition(float newPosition);
    float getDesiredPosition( void );
    void execute(void);

private:
    float dstPosition;

    fl::Engine engine;

    fl::InputVariable inputPosition;
    fl::InputVariable inputVelocity;

    fl::RuleBlock ruleBlock;

    fl::OutputVariable outputVar;
};

#endif // FUZZYCONTROLLER_H
