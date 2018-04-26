#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <memory>
#include "floatfann.h"

namespace NN {

#define dEXPERIENCE_REPLAY_BATCH_SIZE   1U

typedef enum {
//    dInputAngularPosition,
//    dInputAngularVelocity,
    dInputPosition,
    dInputVelocity,

    dInputNumOf,
}InputType_T;

typedef enum {
    dActionDecriment,
    dActionNoChange,
    dActionIncrement,

    dActionNumOf,
}ActionType_T;

typedef struct
{
    float inputsOld[NN::dInputNumOf];
    NN::ActionType_T action;
    float inputsNew[NN::dInputNumOf];
    float reward;
} ExperienceReplayStruct_T;

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
    void decreaseEpsilon();

    float getAngleShift();
    float getReward();
    float getIterator();
    uint_fast32_t getEpochCounter();
    bool isLosingConditionReached();
    bool isWinningConditionReached();
    bool isEpochTimeFinished();

private:
    struct fann *oNn;
    struct fann_train_data *oTrainData;
    float inputsCurrent[NN::dInputNumOf];
    float inputsLast[NN::dInputNumOf];
    float predictedQ[NN::dActionNumOf];
    float predictedQLast[NN::dActionNumOf];

    NN::ActionType_T action;
    NN::ActionType_T actionLast;

    NN::ExperienceReplayStruct_T experienceReplay[dEXPERIENCE_REPLAY_BATCH_SIZE];
    int experienceReplayIndex;
    bool bIsExperienceReplayFull;
    void storeExperience();

    float angleShift;
    float angleShiftLast;

    /* determines probability of selecting random or maxQ moves */
    float epsilon;

    /* incremented at each learning iteration (after every inputs update)*/
    uint_fast32_t uEpochCurrentIteration;
    /* incremented after starting (initializing) new epoch */
    uint_fast32_t uEpochCounter;

    /* used to check if position is in the winning position */
    uint_fast32_t numberOfConsecutiveWinningPositions;

    float positionDst;
    float reward;

    void calculateReward();

    NN::ActionType_T getBestAction(const float * const QValues);
    float getMaxQ(const float *const QValues);
};

#endif // NEURALNETWORK_H
