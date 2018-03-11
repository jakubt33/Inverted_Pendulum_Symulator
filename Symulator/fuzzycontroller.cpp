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
    inputAngle.addTerm(new Trapezoid("HLA", -61.0f, -60.0f, -35.0f, -5.0f));
    inputAngle.addTerm(new Triangle("LLA", -10.0f, -5.0f, 0.0f));
    inputAngle.addTerm(new Triangle("SA", -5.0f, 0.0f, 5.0f));
    inputAngle.addTerm(new Triangle("LRA", 0.0f, 5.0f, 10.0f));
    inputAngle.addTerm(new Trapezoid("HRA", 5.0f, 35.0f, 60.0f, 61.0f));
    engine.addInputVariable(&inputAngle);

    outputPWM.setName("outputPWM");
    outputPWM.setDescription("");
    outputPWM.setEnabled(true);
    outputPWM.setRange(-800.0, 800.0);
    outputPWM.setLockValueInRange(false);
    outputPWM.setAggregation(new Maximum);
    outputPWM.setDefuzzifier(new MeanOfMaximum);
    outputPWM.setDefaultValue(0.0);
    outputPWM.setLockPreviousValue(false);
    outputPWM.addTerm(new Trapezoid("HLOUT", -900.0f, -800.0f, -600.0f, -100.0f));
    outputPWM.addTerm(new Triangle("LLOUT",  -350.0f, -150.0f,    0.0f));
    outputPWM.addTerm(new Triangle("SOUT",   -150.0f,    0.0f,  150.0f));
    outputPWM.addTerm(new Triangle("LROUT",     0.0f,  150.0f,  350.0f));
    outputPWM.addTerm(new Trapezoid("HROUT",  100.0f,  600.0f,  800.0f, 900.0f));
    engine.addOutputVariable(&outputPWM);

    ruleBlock.setName("mamdani");
    ruleBlock.setDescription("");
    ruleBlock.setEnabled(true);
    ruleBlock.setConjunction(fl::null);//AND
    ruleBlock.setDisjunction(fl::null);//OR
    ruleBlock.setImplication(new AlgebraicProduct);
    ruleBlock.setActivation(new General);
    ruleBlock.addRule(Rule::parse("if inAngle is HLA then outputPWM is HLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LLA then outputPWM is LLOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is SA then outputPWM is SOUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is LRA then outputPWM is LROUT", &engine));
    ruleBlock.addRule(Rule::parse("if inAngle is HRA then outputPWM is HROUT", &engine));
    engine.addRuleBlock(&ruleBlock);
}

void FuzzyController::updateInputs(float Angle, float Omega, float Position, float Velocity)
{
    this->inputAngle.setValue(Angle);
    this->inputOmega.setValue(Omega);
    this->inputPosition.setValue(Position);
    this->inputVelocity.setValue(Velocity);
}

void FuzzyController::updateInputs(float Angle)
{
    this->inputAngle.setValue(Angle);
}
