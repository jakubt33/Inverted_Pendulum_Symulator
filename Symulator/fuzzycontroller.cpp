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
    inputOmega.setRange(-350.0, 350.0);
    inputOmega.setLockValueInRange(true);
    inputOmega.addTerm(new Trapezoid("HLO", -351.0f, -350.0f, -200.0f, -40.0f));
    inputOmega.addTerm(new Triangle("LLO", -80.0f, -40.0f, 0.0f));
    inputOmega.addTerm(new Triangle("SO", -50.0f, 0.0f, 50.0f));
    inputOmega.addTerm(new Triangle("LRO", 0.0f, 40.0f, 80.0f));
    inputOmega.addTerm(new Trapezoid("HRO", 40.0f, 200.0f, 350.0f, 351.0f));
    engine.addInputVariable(&inputOmega);

    inputPosition.setName("inPosition");
    inputPosition.setDescription("");
    inputPosition.setEnabled(true);
    inputPosition.setRange(-20.0, 20.0);//[cm]
    inputPosition.setLockValueInRange(true);
    inputPosition.addTerm(new Trapezoid("HLP", -21.0f, -20.0f, -7.0f, -0.0f));
    inputPosition.addTerm(new Triangle("SP", -7.0f, 0.0f, 7.0f));
    inputPosition.addTerm(new Trapezoid("HRP", 0.0f, 7.0f, 20.0f, 21.0f));
    engine.addInputVariable(&inputPosition);

    inputVelocity.setName("inVelocity");
    inputVelocity.setDescription("");
    inputVelocity.setEnabled(true);
    inputVelocity.setRange(-5.0, 5.0);//[RPM]
    inputVelocity.setLockValueInRange(true);
    inputVelocity.addTerm(new Trapezoid("HLV", -5.1f, -5.0f, -2.5f, -0.0f));
    inputVelocity.addTerm(new Triangle("SV", -1.5f, 0.0f, 1.5f));
    inputVelocity.addTerm(new Trapezoid("HRV", 0.0f, 2.5f, 5.0f, 5.1f));
    engine.addInputVariable(&inputVelocity);

    outputPWM.setName("outputPWM");
    outputPWM.setDescription("");
    outputPWM.setEnabled(true);
    outputPWM.setRange(-1000.0, 1000.0);
    outputPWM.setLockValueInRange(true);
    outputPWM.setAggregation(new Maximum);
    outputPWM.setDefuzzifier(new Centroid);
    outputPWM.setDefaultValue(0.0);
    outputPWM.setLockPreviousValue(false);
    outputPWM.addTerm(new Trapezoid("HLOUT", -1000.0f, -1000.0f, -700.0f, -500.0f));
    outputPWM.addTerm(new Triangle("LLOUT",  -700.0f, -600.0f,  0.0f));
    outputPWM.addTerm(new Trapezoid("SOUT",  -200.0f,  -80.0f,  80.0f, 200.0f));
    outputPWM.addTerm(new Triangle("LROUT",     0.0f,  600.0f,  700.0f));
    outputPWM.addTerm(new Trapezoid("HROUT",  500.0f,  700.0f,  1000.0f, 1000.0f));
    engine.addOutputVariable(&outputPWM);

    ruleBlock.setName("mamdani");
    ruleBlock.setDescription("");
    ruleBlock.setEnabled(true);
    ruleBlock.setConjunction(new Minimum);//AND
    ruleBlock.setDisjunction(new Maximum);//OR
    ruleBlock.setImplication(new AlgebraicProduct);
    ruleBlock.setActivation(new General);

    ruleBlock.addRule(Rule::parse("if inAngle is HLA then outputPWM is HLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LLA then outputPWM is LLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is SA then outputPWM is SOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LRA then outputPWM is LROUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is HRA then outputPWM is HROUT", &engine));

    ruleBlock.addRule(Rule::parse("if inOmega is HLO and inAngle is not HLA then outputPWM is HLOUT with 0.75", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is LLO and inAngle is not HLA  then outputPWM is LLOUT with 0.75", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is SO then outputPWM is SOUT with 0.75", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is LRO and inAngle is not HRA then outputPWM is LROUT with 0.75", &engine));
    ruleBlock.addRule(Rule::parse("if inOmega is HRO and inAngle is not HRA then outputPWM is HROUT with 0.75", &engine));

#if 1
    ruleBlock.addRule(Rule::parse("if inPosition is HLP then outputPWM is LLOUT with 0.0", &engine));
    ruleBlock.addRule(Rule::parse("if inPosition is SP then outputPWM is SOUT with 0.0", &engine));
    ruleBlock.addRule(Rule::parse("if inPosition is HRP then outputPWM is LROUT with 0.0", &engine));
    ruleBlock.addRule(Rule::parse("if inVelocity is HLV then outputPWM is HLOUT with 0.0", &engine));
    ruleBlock.addRule(Rule::parse("if inVelocity is SV then outputPWM is SOUT with 0.0", &engine));
    ruleBlock.addRule(Rule::parse("if inVelocity is HRV then outputPWM is HROUT with 0.0", &engine));
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
