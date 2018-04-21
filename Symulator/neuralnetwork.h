#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <memory>
#include "floatfann.h"

namespace NN {

typedef enum {
    dInputAngularPosition,
    dInputAngularVelocity,
    dInputPosition,
    dInputVelocity,

    dInputNumOf,
}InputType_T;

}

class NeuralNetwork
{
public:
    NeuralNetwork();
    ~NeuralNetwork();

    void learn(float inputAngularPosition,
               float inputAngularVelocity,
               float inputPosition,
               float inputVelocity);

    bool isEpochFinished();
    void initNewEpoch();

    float getOutput();
    float getReward();
    float getIterator();
    uint_fast32_t getEpochCounter();

private:
    struct fann *oNn;
    struct fann_train_data *oTrainData;
    float inputsCurrent[NN::dInputNumOf];
    float inputsLast[NN::dInputNumOf];
    float predictedQ;
    float predictedQLast;

    float epsilon; /* determines probability of selecting random or maxQ moves */

    /* incremented at each learning iteration (after every inputs update)*/
    uint_fast32_t uEpochCurrentIteration;
    /* incremented after starting (initializing) new epoch */
    uint_fast32_t uEpochCounter;

    /* used to check if position and angle are in winning position */
    uint_fast32_t epochWhenPositionEnteredWinningPosition;
    uint_fast32_t epochWhenAngleEnteredWinningPosition;

    float positionDst;
    float reward;

    void calculateReward();
    bool losingConditionReached();
    bool winningConditionReached();
    bool isEpochTimeFinished();
};

#endif // NEURALNETWORK_H
