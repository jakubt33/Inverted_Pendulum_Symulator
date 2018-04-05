#include "neuralnetwork.h"

NeuralNetwork::NeuralNetwork()
{
    oNn = fann_create_standard(4, 2, 8, 9, 1);
}

NeuralNetwork::~NeuralNetwork()
{
    fann_destroy(oNn);
}

void NeuralNetwork::updateInputs(float inputAngularPosition,
                                 float inputAngularVelocity,
                                 float inputPosition,
                                 float inputVelocity,
                                 float inputError)
{
    this->inputAngularPosition = inputAngularPosition;
    this->inputAngularVelocity = inputAngularVelocity;
    this->inputPosition = inputPosition;
    this->inputVelocity = inputVelocity;
    this->inputError = inputError;
}

void NeuralNetwork::execute()
{

}

float NeuralNetwork::getOutput()
{
    return this->outputAngleShift;
}
