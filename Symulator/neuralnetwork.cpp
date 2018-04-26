#include "neuralnetwork.h"

/*! ACHTUNG : check how it will work on STM!!!!!!!!!!!!!!! */
#include <time.h>   //needed for rand() func
#include <stdlib.h> //needed for rand() func
#include "string.h" //memset
#include "float.h" //float min value

#define POSITION_MAX            5.0f
#define ANGLE_MAX               30.0f
#define dITERAION_NUMBER_MAX    75U
#define dITERAION_TO_WIN        20U

#define dNUM_LAYERS             3U
#define dNUM_NEURONS_HIDDEN1    800U
#define dNUM_NEURONS_HIDDEN2    20U
#define dGAMMA                  0.99f

#define dMINI_BATCH_SIZE        1U

NeuralNetwork::NeuralNetwork()
{
    /* 4 inputs, 100 hidden neurons, 3 output */
    oNn = fann_create_standard(dNUM_LAYERS,
                               NN::dInputNumOf,
                               dNUM_NEURONS_HIDDEN1,
                               NN::dActionNumOf);

    /* stepwise is more then two times faster
       use symmetric to deal with -1.0 and 1.0 (or normal for 0.0 to 1.0) */
    fann_set_activation_function_hidden(oNn, FANN_SIGMOID_SYMMETRIC_STEPWISE);
    fann_set_activation_function_output(oNn, FANN_SIGMOID_SYMMETRIC_STEPWISE);

    /* fann_set_training_algorithm(oNn, FANN_TRAIN_QUICKPROP); */
    fann_set_learning_momentum(oNn, 0.02f);

    /* by default. Real learning rate should depend on measured error */
    fann_set_learning_rate(oNn, 0.1f);

    /* update weight once for whole epoch */
    fann_set_training_algorithm(oNn, FANN_TRAIN_BATCH);

    /* set some random weights */
    fann_randomize_weights(oNn, -0.01f, 0.01f);

    /* create training data */
    oTrainData = fann_create_train(dMINI_BATCH_SIZE, NN::dInputNumOf, NN::dActionNumOf);

    /* init new epoch by default */
    uEpochCounter = 0;
    experienceReplayIndex = 0;
    bIsExperienceReplayFull = false;
    epsilon = 1.0f; /*!< start with random searching of optimal movements */
    initNewEpoch();
    srand(time(NULL));   // initialize rand()
}

NeuralNetwork::~NeuralNetwork()
{
    fann_destroy(oNn);
    fann_destroy_train(oTrainData);
}

void NeuralNetwork::run(float inputPosition,
                        float inputVelocity)
{
    inputsCurrent[NN::dInputPosition] = inputPosition;
    inputsCurrent[NN::dInputVelocity] = inputVelocity;

    /* We are in state S_n. Let's run the network and see if returned Q will be predicted correctly */
    memcpy(predictedQ, fann_run(oNn, inputsCurrent), sizeof(float) * NN::dActionNumOf);

    /* trust that currently learned Q is good enough and use it */
    action = getBestAction(predictedQ);

    /* increment or decrement angleShift by 0.1 degrees */
    angleShift += ((NN::dActionIncrement == action) - (NN::dActionDecriment == action)) / 10.0f;

    calculateReward();
}

void NeuralNetwork::learn(float inputAngularPosition,
                          float inputAngularVelocity,
                          float inputPosition,
                          float inputVelocity)
{
    /* inputs that are achieved by using predicted Q in last iteration */
    //inputsCurrent[NN::dInputAngularPosition] = inputAngularPosition;
    //inputsCurrent[NN::dInputAngularVelocity] = inputAngularVelocity;
    inputsCurrent[NN::dInputPosition] = inputPosition;
    inputsCurrent[NN::dInputVelocity] = inputVelocity;

    /* We are in state S_n. Let's run the network and see if returned Q will be predicted correctly */
    memcpy(predictedQ, fann_run(oNn, inputsCurrent), sizeof(float) * NN::dActionNumOf);

    /* generate random number in range 0..1. */
    //float randNumber = (float)rand() / RAND_MAX;
    //if (randNumber < epsilon) /* start exploration and see if the algorythm will find some peachy solution! */
    {
        /* choose random action */;
    //    action = (NN::ActionType_T)(rand() % (int)NN::dActionNumOf);
    }
    //else /* trust that currently learned Q is good enough and use it */
    {
        action = getBestAction(predictedQ);
    }

    /* increment or decrement angleShift by 0.1 degrees */
    angleShift += ((NN::dActionIncrement == action) - (NN::dActionDecriment == action)) / 10.0f;

    /* count iterations of the function. If this is the first iteration then no prior
     * data are stored so it is not feasible to continue - action first have to be done */
    if (uEpochCurrentIteration++ == 0U)
    {
        /* just store predicted Q value as an old one and wait for next iteration */
        actionLast = action;
        mempcpy(predictedQLast, predictedQ, sizeof(float) * NN::dActionNumOf);
        mempcpy(inputsLast, inputsCurrent, sizeof(float) * NN::dInputNumOf);
        return;
    }

    /* from now we won't be changing predicted Q values. It is calculated for the this iteration, the case is over.
     * Now it is time to handle previously calculated Q and learn something for the future purposes.

    /* get reward in this epoch iteration for using LAST action and obtaining inputsCurrent */
    calculateReward();


    /* to prevent from catastrophal forgetting, store some learned values in memory and learn basing on them */

    /* if experience replay memory is not filled, just add the new one */
    storeExperience();
    if (bIsExperienceReplayFull)
    {
        for (int miniBatchIterator=0; miniBatchIterator < dMINI_BATCH_SIZE; miniBatchIterator++)
        {
            /* select one random experience */
            int randomExperienceIndex = rand() % dEXPERIENCE_REPLAY_BATCH_SIZE;

            float *predictedQNew = fann_run(oNn, experienceReplay[randomExperienceIndex].inputsNew);
            float maxQ = getMaxQ(predictedQNew);

            float *predictedQOld = fann_run(oNn, experienceReplay[randomExperienceIndex].inputsOld);

            /* if it is not a terminal state then set the old values as train data (output) - use new max in equation */
            if (!isEpochFinished())
            {
                *(predictedQOld + experienceReplay[randomExperienceIndex].action)
                        = experienceReplay[randomExperienceIndex].reward + (dGAMMA * maxQ);
            }
            else
            {
                *(predictedQOld + experienceReplay[randomExperienceIndex].action)
                        = experienceReplay[randomExperienceIndex].reward;
            }

            /* store the adjusted data in mini batch fann train file */
            memcpy(oTrainData->input[miniBatchIterator], experienceReplay[randomExperienceIndex].inputsOld, sizeof(float) * NN::dInputNumOf);
            memcpy(oTrainData->output[miniBatchIterator], predictedQOld, sizeof(float) * NN::dActionNumOf);
        }

        /* train the data sets */
        fann_train_on_data(oNn, oTrainData, dMINI_BATCH_SIZE, 0, 0.001);
    }

    /* from now on the current prediction of Q becomes obsolete */
    actionLast = action;
    angleShiftLast = angleShift;
    mempcpy(predictedQLast, predictedQ, sizeof(float) * NN::dActionNumOf);
    mempcpy(inputsLast, inputsCurrent, sizeof(float) * NN::dInputNumOf);
}

#define dMAX_EPOCHS_TO_LEARN 1000.0f
void NeuralNetwork::decreaseEpsilon()
{
    /* Decrease epsilon to base more on learned values */
    if (epsilon > 0.01f) epsilon -= (1.0f / dMAX_EPOCHS_TO_LEARN);
}

void NeuralNetwork::storeExperience()
{
    if (experienceReplayIndex >= dEXPERIENCE_REPLAY_BATCH_SIZE)
    {
        experienceReplayIndex = 0;
    }

    mempcpy(experienceReplay[experienceReplayIndex].inputsOld, inputsLast, sizeof(float) * NN::dInputNumOf);
    experienceReplay[experienceReplayIndex].action = actionLast;
    mempcpy(experienceReplay[experienceReplayIndex].inputsNew, inputsCurrent, sizeof(float) * NN::dInputNumOf);
    experienceReplay[experienceReplayIndex].reward = reward;

    if (++experienceReplayIndex >= dEXPERIENCE_REPLAY_BATCH_SIZE)
    {
        bIsExperienceReplayFull = true;
    }
}

NN::ActionType_T NeuralNetwork::getBestAction(const float *const QValues)
{
    float biggestQ = -FLT_MAX;
    NN::ActionType_T bestAction = (NN::ActionType_T)0;

    for (int i = 0; i < (int)NN::dActionNumOf; i++)
    {
        if (*(QValues + i) > biggestQ)
        {
            biggestQ = *(QValues + i);
            bestAction = (NN::ActionType_T)i;
        }
    }
    return bestAction;
}

float NeuralNetwork::getMaxQ(const float * const QValues)
{
    float biggestQ = -FLT_MAX;
    for (int i = 0; i < (int)NN::dActionNumOf; i++)
    {
        if (*(QValues + i) > biggestQ)
        {
            biggestQ = *(QValues + i);
        }
    }
    return biggestQ;
}

#include "math.h"
#define dTOTAL_POSSIBLE_PUNISHMENT      200.0f
#define dWIN_LOSE_TO_PUNISHMENT_RATIO   5.0f
void NeuralNetwork::calculateReward()
{
    if (!isEpochFinished())
    {
        /* temporal difference algorithm is used so reward is calculated and added after each action taken */

        /* get immediate reward/punishment for the current position error.
         * r = -|x|/10 for |x| = [0 to 10]cm
         * r = -1      for (x > 10cm) || (x < -10cm)
         */
        float positionDiff = ( fabsf(positionDst - inputsLast[NN::dInputPosition])
                             - fabsf(positionDst - inputsCurrent[NN::dInputPosition])
                             ) * 30.0f;
        float rewardPosition = (positionDiff < -1.0f) ? -1.0f : ((positionDiff > 1.0f) ? 1.0f : positionDiff);

        /* get immediate reward/punishment for the current velocity error.
         * r = -|fi|/10 for |fi| = [0 to 10]deg
         * r = -1       for (fi > 10deg) || (fi < -10deg)
         */
        float velocityDiff = ( fabsf(0 - inputsLast[NN::dInputVelocity])
                             - fabsf(0 - inputsCurrent[NN::dInputVelocity])
                             ) * 30.0f;
        float rewardVelocity = (velocityDiff < -1.0f) ? -1.0f : ((velocityDiff > 1.0f) ? 1.0f : velocityDiff);

        /* total punishment is just an mean value of its components divided by ratio parameter.
         * i.e: if there is 200 iterations then max punishment is -200.0 and if ratio is 5,
         *      then total punishment will be 0.2 (to keep ratio 5 for loosing or
         *      winning the game where +-1 point is given)
         */
        float rewardTotal = ((rewardPosition + rewardVelocity) / 2.0f)
                          / (/*dTOTAL_POSSIBLE_PUNISHMENT* */dWIN_LOSE_TO_PUNISHMENT_RATIO);
        reward = rewardTotal;
    }
    else
    {
        if (isWinningConditionReached())
        {
            /* give a cookie for a good behaviour
             * TODO: this can be changed to a funtion instead of constant 1 value
             *       to award more better states
             */
            reward = 3.0f;
        }
        else /* losing */
        {
            /* punish with maximum possible value. It must be checked somewhere that
             * training has failed and new learning proccess should be started. */
            reward = -1.0f;
        }
    }
}

/*!
 * \brief NeuralNetwork::failConditionReached
 * \return bool - if true then a punishment must be applied as the net doesn't work properly!
 */
bool NeuralNetwork::isLosingConditionReached()
{
    bool positionFail = fabsf(inputsCurrent[NN::dInputPosition]) > POSITION_MAX;
    bool angleFail = 0; //fabsf(inputsCurrent[NN::dInputAngularPosition]) > ANGLE_MAX;

    return positionFail || angleFail;
}

#define dPOSITION_WINNING               1.0f
#define dVELOCITY_WINNING               0.05f
bool NeuralNetwork::isWinningConditionReached()
{
    bool bPositionWin = fabsf(positionDst - inputsCurrent[NN::dInputPosition]) < dPOSITION_WINNING;
    bool bVelocityWin = fabsf(inputsCurrent[NN::dInputVelocity]) < dVELOCITY_WINNING;

    bool bIsWin = (bPositionWin && bVelocityWin);

    if (!bIsWin) numberOfConsecutiveWinningPositions = 0U;
    else numberOfConsecutiveWinningPositions++;

    return bIsWin;
}

bool NeuralNetwork::isEpochTimeFinished()
{
    return uEpochCurrentIteration >= dITERAION_NUMBER_MAX;
}

bool NeuralNetwork::isEpochFinished()
{
    bool bIsLost = isEpochTimeFinished() || isLosingConditionReached();
    bool bIsWin = numberOfConsecutiveWinningPositions >= dITERAION_TO_WIN;

    return bIsLost || bIsWin;
}

void NeuralNetwork::initNewEpoch()
{
    reward = 0.0f;
    uEpochCurrentIteration = 0U;
    numberOfConsecutiveWinningPositions = 0U;
    memset(inputsCurrent, 0, sizeof(inputsCurrent[0]) * NN::dInputNumOf);
    memset(inputsLast, 0, sizeof(inputsLast[0]) * NN::dInputNumOf);
    memset(predictedQ, 0, sizeof(predictedQ[0]) * NN::dActionNumOf);
    memset(predictedQLast, 0, sizeof(predictedQLast[0]) * NN::dActionNumOf);
    positionDst = 0.0f;
    angleShift = 0.0f;
    angleShiftLast = 0.0f;
    uEpochCounter++;
}

float NeuralNetwork::getReward()
{
    return reward;
}

float NeuralNetwork::getIterator()
{
    return uEpochCurrentIteration;
}

uint_fast32_t NeuralNetwork::getEpochCounter()
{
    return uEpochCounter;
}

/*!
 * \brief NeuralNetwork::getOutput - used to get output from previously trained network
 * \return - angle shift
 */
float NeuralNetwork::getAngleShift()
{
    return angleShift;
}
