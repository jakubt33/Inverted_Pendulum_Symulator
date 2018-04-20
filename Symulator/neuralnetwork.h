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

    /*!
     * \brief   Updates inputs. State of object must be passed as a parameter
     * \param   u32TimeDelta - time in ms since last parameters update
     */
    void learn(float inputAngularPosition,
               float inputAngularVelocity,
               float inputPosition,
               float inputVelocity);


    bool stopConditionReached();
    void setEpochTime(uint_fast32_t uEpochDuration);

    float getOutput();
    float getReward();
    float getIterator();

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
    uint_fast32_t uEpochDuration;

    /* used to check if position and angle are in winning position */
    uint_fast32_t epochWhenPositionEnteredWinningPosition;
    uint_fast32_t epochWhenAngleEnteredWinningPosition;

    float positionDst;
    float reward;
    void calculateReward();
    bool loosingConditionReached();
    bool winningConditionReached();
    bool isEpochTimeFinished();



    float calculateErrorValue();
    float getAdaptationSpeed();

    float errorTotalLast;
    float errorGradient;
    float errorTotalCurrent;
    float errorImportanceFactor;

public:
    float train(float inputError);
};

#endif // NEURALNETWORK_H
