#include "fuzzycontroller.h"

FuzzyController::FuzzyController(FUZZY_CONTROLLER::TypeOfRegulation_T typeOfRegulation)
{
    dstPosition = 0.0f;

    using namespace fl;
    if(typeOfRegulation == FUZZY_CONTROLLER::AngleRegulation)
    {
        /* ====================================================*/
        /* -----------------------ANGLE------------------------*/
        /* ====================================================*/
        engine.setName("ANGLE");
        engine.setDescription("");

        inputPosition.setName("inAngle");
        inputPosition.setDescription("");
        inputPosition.setEnabled(true);
        inputPosition.setRange(-60.0, 60.0);
        inputPosition.setLockValueInRange(false);
        inputPosition.addTerm(new Trapezoid("HLA", -61.0f, -60.0f, -20.0f, -5.0f));
        inputPosition.addTerm(new Triangle("LLA", -8.0f, -4.0f, 0.0f));
        inputPosition.addTerm(new Triangle("SA", -3.0f, 0.0f, 3.0f));
        inputPosition.addTerm(new Triangle("LRA", 0.0f, 4.0f, 8.0f));
        inputPosition.addTerm(new Trapezoid("HRA", 5.0f, 20.0f, 60.0f, 61.0f));
        engine.addInputVariable(&inputPosition);

        inputVelocity.setName("inOmega");
        inputVelocity.setDescription("");
        inputVelocity.setEnabled(true);
        inputVelocity.setRange(-350.0, 350.0);
        inputVelocity.setLockValueInRange(true);
        inputVelocity.addTerm(new Trapezoid("HLO", -351.0f, -350.0f, -200.0f, -40.0f));
        inputVelocity.addTerm(new Triangle("LLO", -80.0f, -40.0f, 0.0f));
        inputVelocity.addTerm(new Triangle("SO", -50.0f, 0.0f, 50.0f));
        inputVelocity.addTerm(new Triangle("LRO", 0.0f, 40.0f, 80.0f));
        inputVelocity.addTerm(new Trapezoid("HRO", 40.0f, 200.0f, 350.0f, 351.0f));
        engine.addInputVariable(&inputVelocity);

        outputVar.setName("outputPWM");
        outputVar.setDescription("");
        outputVar.setEnabled(true);
        outputVar.setRange(-1000.0, 1000.0);
        outputVar.setLockValueInRange(true);
        outputVar.setAggregation(new Maximum);
        outputVar.setDefuzzifier(new Centroid);
        outputVar.setDefaultValue(0.0);
        outputVar.setLockPreviousValue(false);
        outputVar.addTerm(new Trapezoid("HLOUT", -1000.0f, -1000.0f, -700.0f, -500.0f));
        outputVar.addTerm(new Triangle("LLOUT",  -700.0f, -600.0f,  0.0f));
        outputVar.addTerm(new Trapezoid("SOUT",  -200.0f,  -80.0f,  80.0f, 200.0f));
        outputVar.addTerm(new Triangle("LROUT",     0.0f,  600.0f,  700.0f));
        outputVar.addTerm(new Trapezoid("HROUT",  500.0f,  700.0f,  1000.0f, 1000.0f));
        engine.addOutputVariable(&outputVar);

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

        engine.addRuleBlock(&ruleBlock);
    }
    else if(typeOfRegulation == FUZZY_CONTROLLER::PositionRegulation)
    {
        /* ====================================================*/
        /* ----------------------POSITION----------------------*/
        /* ====================================================*/
        engine.setName("POSITION");
        engine.setDescription("");

        inputPosition.setName("inPosition");
        inputPosition.setDescription("");
        inputPosition.setEnabled(true);
        inputPosition.setRange(-30.0, 30.0);//[cm]
        inputPosition.setLockValueInRange(true);
        inputPosition.addTerm(new Trapezoid("LP", -31.0f, -30.0f, -15.0f, 0.0f));
        inputPosition.addTerm(new Triangle("SP",  -15.0f,  0.0f,   15.0f));
        inputPosition.addTerm(new Trapezoid("RP",  0.0f,  15.0f,   30.0f, 31.0f));
        engine.addInputVariable(&inputPosition);

        inputVelocity.setName("inVelocity");
        inputVelocity.setDescription("");
        inputVelocity.setEnabled(true);
        inputVelocity.setRange(-4.0, 4.0);//[RPM]
        inputVelocity.setLockValueInRange(true);
        inputVelocity.addTerm(new Trapezoid("LV", -4.1f, -4.0f, -1.0f, 0.0f));
        inputVelocity.addTerm(new Triangle("SV", -0.5f, 0.0f, 0.5f));
        inputVelocity.addTerm(new Trapezoid("RV", 0.0f, 1.0f, 4.0f, 4.1f));
        engine.addInputVariable(&inputVelocity);

        outputVar.setName("dstANGLE");
        outputVar.setDescription("");
        outputVar.setEnabled(true);
        outputVar.setRange(-10.0, 10.0);
        outputVar.setLockValueInRange(true);
        outputVar.setAggregation(new Maximum);
        outputVar.setDefuzzifier(new Centroid);
        outputVar.setDefaultValue(0.0);
        outputVar.setLockPreviousValue(false);
        outputVar.addTerm(new Trapezoid("LOUT",  -10.1f, -10.0f, -5.0f, -1.0f));
        outputVar.addTerm(new Trapezoid("SOUT",  -3.0f,  -1.0f,   1.0f,  3.0f));
        outputVar.addTerm(new Trapezoid("ROUT",   1.0f,   5.0f,   10.0f, 10.1f));
        engine.addOutputVariable(&outputVar);

        ruleBlock.setName("mamdani");
        ruleBlock.setDescription("");
        ruleBlock.setEnabled(true);
        ruleBlock.setConjunction(new Minimum);//AND
        ruleBlock.setDisjunction(new Maximum);//OR
        ruleBlock.setImplication(new AlgebraicProduct);
        ruleBlock.setActivation(new General);

        ruleBlock.addRule(Rule::parse("if inPosition is LP then dstANGLE is ROUT", &engine));
        ruleBlock.addRule(Rule::parse("if inPosition is SP then dstANGLE is SOUT", &engine));
        ruleBlock.addRule(Rule::parse("if inPosition is RP then dstANGLE is LOUT", &engine));

        ruleBlock.addRule(Rule::parse("if inVelocity is LV then dstANGLE is ROUT", &engine));
        ruleBlock.addRule(Rule::parse("if inVelocity is SV then dstANGLE is SOUT", &engine));
        ruleBlock.addRule(Rule::parse("if inVelocity is RV then dstANGLE is LOUT", &engine));

        engine.addRuleBlock(&ruleBlock);
    }
}
float FuzzyController::getOutput()
{
    return (float)outputVar.getValue();
}

void FuzzyController::updateInputs(float currentPosition, float currentVelocity)
{
    this->inputPosition.setValue(currentPosition - dstPosition);
    this->inputVelocity.setValue(currentVelocity);
}

void FuzzyController::setDesiredPosition(float newPosition)
{
    dstPosition = newPosition;
}

float FuzzyController::getDesiredPosition( void )
{
    return dstPosition;
}

void FuzzyController::execute(void)
{
    engine.process();
}

