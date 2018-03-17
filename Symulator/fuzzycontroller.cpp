#include "fuzzycontroller.h"

FuzzyController::FuzzyController()
{
    using namespace fl;

    engine.setName("Balnsyner");
    engine.setDescription("");

    inputAngle.setName("inAngle");
    inputAngle.setDescription("");
    inputAngle.setEnabled(true);
    inputAngle.setRange(-60.0, 60.0);
    inputAngle.setLockValueInRange(false);
    inputAngle.addTerm(new Trapezoid("HLA", -61.0f, -60.0f, -20.0f, -5.0f));
    inputAngle.addTerm(new Triangle("LLA", -8.0f, -4.0f, 0.0f));
    inputAngle.addTerm(new Triangle("SA", -3.0f, 0.0f, 3.0f));
    inputAngle.addTerm(new Triangle("LRA", 0.0f, 4.0f, 8.0f));
    inputAngle.addTerm(new Trapezoid("HRA", 5.0f, 20.0f, 60.0f, 61.0f));
    engine.addInputVariable(&inputAngle);

    inputOmega.setName("inOmega");
    inputOmega.setDescription("");
    inputOmega.setEnabled(true);
    inputOmega.setRange(-500.0, 500.0);
    inputOmega.setLockValueInRange(true);
    inputOmega.addTerm(new Trapezoid("HLO", -500.0f, -500.0f, -300.0f, -75.0f));
    inputOmega.addTerm(new Triangle("LLO", -150.0f, -75.0f, 0.0f));
    inputOmega.addTerm(new Triangle("SO", -50.0f, 0.0f, 50.0f));
    inputOmega.addTerm(new Triangle("LRO", 0.0f, 75.0f, 150.0f));
    inputOmega.addTerm(new Trapezoid("HRO", 75.0f, 300.0f, 500.0f, 500.0f));
    engine.addInputVariable(&inputOmega);

    inputPosition.setName("inPosition");
    inputPosition.setDescription("");
    inputPosition.setEnabled(true);
    inputPosition.setRange(-40.0, 40.0);//[cm]
    inputPosition.setLockValueInRange(false);
    inputPosition.addTerm(new Trapezoid("HLP", -41.0f, -40.0f, -25.0f, -5.0f));
    inputPosition.addTerm(new Triangle("SP", -10.0f, 0.0f, 10.0f));
    inputPosition.addTerm(new Trapezoid("HRP", 5.0f, 25.0f, 40.0f, 41.0f));
    //engine.addInputVariable(&inputPosition);

    outputPWM.setName("outputPWM");
    outputPWM.setDescription("");
    outputPWM.setEnabled(true);
    outputPWM.setRange(-800.0, 800.0);
    outputPWM.setLockValueInRange(false);
    outputPWM.setAggregation(new Maximum);
    outputPWM.setDefuzzifier(new Centroid);
    outputPWM.setDefaultValue(0.0);
    outputPWM.setLockPreviousValue(false);
    outputPWM.addTerm(new Trapezoid("HLOUT", -901.0f, -900.0f, -800.0f, -500.0f));
    outputPWM.addTerm(new Triangle("LLOUT",  -650.0f, -500.0f,  0.0f));
    outputPWM.addTerm(new Trapezoid("SOUT",  -200.0f,  -80.0f,  80.0f, 200.0f));
    outputPWM.addTerm(new Triangle("LROUT",     0.0f,  500.0f,  650.0f));
    outputPWM.addTerm(new Trapezoid("HROUT",  500.0f,  800.0f,  900.0f, 901.0f));
    engine.addOutputVariable(&outputPWM);

    ruleBlock.setName("mamdani");
    ruleBlock.setDescription("");
    ruleBlock.setEnabled(true);
    ruleBlock.setConjunction(new Minimum);//AND
    ruleBlock.setDisjunction(new Maximum);//OR
    ruleBlock.setImplication(new AlgebraicProduct);
    ruleBlock.setActivation(new General);
#if 0
    ruleBlock.addRule(Rule::parse("if inAngle is HLA and inPosition is HLP then outputPWM is SOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LLA and inPosition is HLP then outputPWM is LROUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is SA and inPosition is HLP then outputPWM is LROUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LRA and inPosition is HLP then outputPWM is HROUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is HRA and inPosition is HLP then outputPWM is HROUT", &engine));

    ruleBlock.addRule(Rule::parse("if inAngle is HLA and inPosition is SP then outputPWM is HLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LLA and inPosition is SP then outputPWM is LLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is SA and inPosition is SP then outputPWM is SOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LRA and inPosition is SP then outputPWM is LROUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is HRA and inPosition is SP then outputPWM is HROUT", &engine));

    ruleBlock.addRule(Rule::parse("if inAngle is HLA and inPosition is HRP then outputPWM is HLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LLA and inPosition is HRP then outputPWM is HLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is SA and inPosition is HRP then outputPWM is LLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LRA and inPosition is HRP then outputPWM is LLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is HRA and inPosition is HRP then outputPWM is SOUT", &engine));
#else

    ruleBlock.addRule(Rule::parse("if inAngle is HLA then outputPWM is HLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LLA then outputPWM is LLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is SA then outputPWM is SOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LRA then outputPWM is LROUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is HRA then outputPWM is HROUT", &engine));

    ruleBlock.addRule(Rule::parse("if inOmega is HLO then outputPWM is HLOUT with 0.5", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is LLO then outputPWM is LLOUT with 0.5", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is SO then outputPWM is SOUT with 0.5", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is LRO then outputPWM is LROUT with 0.5", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is HRO then outputPWM is HROUT with 0.5", &engine));

    //ruleBlock.addRule(Rule::parse("if inPosition is HLP then outputPWM is HROUT with 0.5", &engine));
   // ruleBlock.addRule(Rule::parse("if inPosition is SP then outputPWM is SOUT with 0.5", &engine));
   // ruleBlock.addRule(Rule::parse("if inPosition is HRP then outputPWM is HLOUT with 0.5", &engine));
#endif
    engine.addRuleBlock(&ruleBlock);
}

void FuzzyController::updateInputs(float Angle, float Omega, float Position, float Velocity)
{
    this->inputAngle.setValue(Angle);
    this->inputOmega.setValue(Omega);
    this->inputPosition.setValue(Position);
    this->inputVelocity.setValue(Velocity);
}

void FuzzyController::updateInputs(float Angle, float Omega, float Position)
{
    this->inputAngle.setValue(Angle);
    this->inputOmega.setValue(Omega);
    this->inputPosition.setValue(Position);
}
