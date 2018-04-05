#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <memory>
#include "floatfann.h"

class NeuralNetwork
{
public:
    NeuralNetwork();
    ~NeuralNetwork();
    void updateInputs(float inputAngularPosition,
                      float inputAngularVelocity,
                      float inputPosition,
                      float inputVelocity,
                      float inputError);
    void execute();
    float getOutput();

private:
    //FANN::neural_net oNn;
    struct fann * oNn;

    float inputAngularPosition;
    float inputAngularVelocity;
    float inputPosition;
    float inputVelocity;

    float inputError;

    float outputAngleShift;
};

#endif // NEURALNETWORK_H
