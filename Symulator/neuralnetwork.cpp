#include "neuralnetwork.h"

/*! ACHTUNG : check how it will work on STM!!!!!!!!!!!!!!! */
#include <time.h>   //needed for rand() func
#include <stdlib.h> //needed for rand() func

#define POSITION_MAX            30.0f
#define ANGLE_MAX               30.0f
#define dITERAION_NUMBER_MAX    200U

#define dNUM_LAYERS         3U
#define dNUM_OUTPUTS        1U
#define dNUM_NEURONS_HIDDEN 100U
#define dDESIRED_ERROR      0.1f
#define dTRAIN_MEMORY       dITERAION_NUMBER_MAX
#define dERROR_NN_TRAINED   5.0f


NeuralNetwork::NeuralNetwork()
{
    /* 4 inputs, 100 hidden neurons, 1 output */
    oNn = fann_create_standard(dNUM_LAYERS, NN::dInputNumOf, dNUM_NEURONS_HIDDEN, dNUM_OUTPUTS);

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
    oTrainData = fann_create_train(dTRAIN_MEMORY, NN::dInputNumOf, dNUM_OUTPUTS);


    errorTotalLast = NAN;

    /* initialize default epoch duration */
    uEpochDuration = dITERAION_NUMBER_MAX;
    uEpochCurrentIteration = 0;
    epochWhenPositionEnteredWinningPosition = 0;
    epochWhenAngleEnteredWinningPosition = 0;
    epsilon = 1.0f; /*!< start with random searching of optimal movements */

    srand(time(NULL));   // initialize rand()
}

NeuralNetwork::~NeuralNetwork()
{
    fann_destroy(oNn);
    fann_destroy_train(oTrainData);
}

/*!
 * \brief setEpochTime
 * \param u32EpochDuration - epoch duration given in ms
 */
void NeuralNetwork::setEpochTime(uint_fast32_t u32EpochDuration)
{
    this->uEpochDuration = u32EpochDuration;
}


void NeuralNetwork::learn(float inputAngularPosition,
                           float inputAngularVelocity,
                           float inputPosition,
                           float inputVelocity)
{
    /* inputs that are achieved by using predicted Q in last iteration */
    inputsCurrent[NN::dInputAngularPosition] = inputAngularPosition;
    inputsCurrent[NN::dInputAngularVelocity] = inputAngularVelocity;
    inputsCurrent[NN::dInputPosition] = inputPosition;
    inputsCurrent[NN::dInputVelocity] = inputVelocity;

    /* We are in state S_n. Let's run the network and see if returned Q will be predicted correctly */
    predictedQ = *fann_run(oNn, inputsCurrent);

    /* count iterations of the function. If this is the first iteration then no prior
     * data are stored so it is not feasible to continue.  */
    if(uEpochCurrentIteration++ == 0U)
    {
        /* just store predicted Q value as an old one and wait for next iteration */
        predictedQLast = predictedQ;
        return;// predictedQ;
    }

    /* generate random number in range 0..1. */
    float randNumber = (float)rand() / RAND_MAX;
    if (randNumber < epsilon) /* start exploration and see if the algorythm will find some peachy solution! */
    {
        /* transpose randNumber from range 0..1 to -1..1. TODO maybe other range should be used */
        randNumber = (randNumber - 0.5f) * 2.0f;
        /* predicted Q is also an output. What a brilliant idea to simplify life! */
        predictedQ += randNumber;
    }
    /* else: trust that currently learned Q is good enough and use it */


    /* from now we won't be changing predicted Q. It is calculated for the this iteration, the case is over
     * Not it is time to handle previously calculated Q and learn something for the future purposes.

    /* get reward in this epoch iteration for using predictedQLast and obtaining inputsCurrent */
    calculateReward();

    fann_train(oNn, inputsLast, &predictedQ);//or rather input angle?

    /* from now on the current prediction of Q becomes obsolete */
    predictedQLast = predictedQ;
    inputsLast[NN::dInputAngularPosition] = inputsCurrent[NN::dInputAngularPosition];
    inputsLast[NN::dInputAngularVelocity] = inputsCurrent[NN::dInputAngularVelocity];
    inputsLast[NN::dInputPosition]        = inputsCurrent[NN::dInputPosition];
    inputsLast[NN::dInputVelocity]        = inputsCurrent[NN::dInputVelocity];

#if 0
    if (random.random() < epsilon) //choose random action
        action = np.random.randint(0,4);
    else //choose best action from Q(s,a) values
        action = (np.argmax(qval));

    /* Importance factor makes that the results at the end of the epoch are more important */
    errorImportanceFactor = uEpochCurrentIteration/uEpochDuration;
    /* Calculate total error since the beginning of the epoch */
    errorTotalCurrent += errorImportanceFactor*calculateErrorValue();


    /* store the data in a batch fann train file */
    memcpy(oTrainData->input[uEpochCurrentIteration], inputs, NN::dInputNumOf * sizeof(float));
    memcpy(oTrainData->output[uEpochCurrentIteration], &predictedQ, dNUM_OUTPUTS * sizeof(float));

    /* if epoch ended, then update weights and start new epoch */
    if ( uEpochCurrentIteration >= uEpochDuration )
    {
        /* checking if last total error is a number - otherwise it is a first
         * loop and last error needs to be initialized */
        if ( errorTotalLast != errorTotalLast )
        {
            /* initialize last total error and from now this line of code will never be reached */
            errorTotalLast = errorTotalCurrent;

            if ( !isNetworkTrained() )
            {
                /*todo: change OUTPUT randomly(), i.e it should be within a range from -5 to 5.
                 * i need to select the value and the see if it was correct 
                 */
                
            }
        }
        else /* it is not the first loop so all errors data are correct */
        {
            if ( !isNetworkTrained() )
            /* here the learning algorithm is executed */
            {
                errorGradient = errorTotalLast - errorCurrent;
                if ( errorGradient > 0.0f )       /* direction of changing weights is good */;
                else if ( errorGradient <= 0.0f ) /* direction of changing weights is bad */;
                
                for (uint32_t outputIterator = 0; outputIterator < outputIterator; outputIterator++)
                {
                    /* add reward and take into consideration how far from being ideal the error is */
                    oTrainData->output[outputIterator] += getReward()*getAdaptationSpeed();
                }
                if ( errorTotalCurrent < 20 ); //super
                //fann_train_epoch(this->oNn, this->oTrainData); TODO: consider it

                //train the data sets
                fann_train_epoch(oNn, oTrainData);
            }
        }

        /* Epoch has ended, it is time to start counting from the beginning */
        this->uEpochCurrentIteration = 0;
        this->errorTotalCurrent = 0;
    }
    return predictedQ;
#endif
}
#if 0
bool NeuralNetwork::isNetworkTrained()
{
    /* if error is less than treshhold then network is fully trained for current robot model */
    //todo: check if epoch has ended
    return errorTotalCurrent < dERROR_NN_TRAINED;
}
#endif

#include "math.h"
#define dTOTAL_POSSIBLE_PUNISHMENT      200.0f
#define dWIN_LOSE_TO_PUNISHMENT_RATIO   5.0f
void NeuralNetwork::calculateReward()
{
    if (!loosingConditionReached())
    {
        /* TODO: Probalby position punishment should have higher priority than
         *       angle punishement to prevent from situation when angle is
         *       reached but robot is moving and doesn't exceed boundaries
         *       before last epoch iteration.
         *       But maybe reward for reaching position goal will be enough *
         */


        /* get punishment for the current position error.
         * r = -|x|/10 for |x| = [0 to 10]cm
         * r = -1      for (x > 10cm) || (x < -10cm)
         */
        float positionAbs = fabsf(positionDst - inputsCurrent[NN::dInputPosition]);
        float punishmentPosition = positionAbs < 10.0f ? positionAbs / 10.0f : 1.0f;

        /* get punishment for the current angle error. Destination angle is just an output of the net: Q
         * r = -|fi|/10 for |fi| = [0 to 10]deg
         * r = -1       for (fi > 10deg) || (fi < -10deg)
         */
        float angleAbs = fabsf(predictedQLast - inputsCurrent[NN::dInputAngularPosition]);
        float punishmentAngle = angleAbs < 10.0f ? angleAbs / 10.0f : 1.0f;

        /* total punishment is just an mean value of its components divided by ratio parameter.
         * i.e: if there is 200 iterations then max punishment is -200.0 and if ratio is 5,
         *      then total punishment will be 0.2 (to keep ratio 5 for loosing or
         *      winning the game where +-1 point is given)
         */
        float punishmentTotal = ((punishmentPosition + punishmentAngle) / 2.0f)
                                / (dTOTAL_POSSIBLE_PUNISHMENT*dWIN_LOSE_TO_PUNISHMENT_RATIO);
        reward -= punishmentTotal;

        /* check if the net should not be awarded */
        if (winningConditionReached())
        {
            /* give a cookie for a good behaviour
             * TODO: this can be changed to a funtion instead of constant 1 value
             *       to award more better states
             */
            reward += 1.0f;
        }
    }
    else
    {
        /* punish with maximum possible value. It must be assured somewhere that
         * training has failed and new learning proccess should be started. */
        reward -= 0.0f;
    }
}

/*!
 * \brief NeuralNetwork::failConditionReached
 * \return bool - if true then a punishment must be applied as the net doesn't work properly!
 */
bool NeuralNetwork::loosingConditionReached()
{
    bool positionFail = (inputsCurrent[NN::dInputPosition] > POSITION_MAX) || (inputsCurrent[NN::dInputPosition] < -POSITION_MAX);
    bool angleFail = (inputsCurrent[NN::dInputAngularPosition] > ANGLE_MAX) || (inputsCurrent[NN::dInputAngularPosition] < -ANGLE_MAX);

    return positionFail || angleFail;
}

#define dNUMBER_OF_ITERATIONS_TO_WIN    40U
#define dPOSITION_WINNING               1.0f
#define dANGLE_WINNING                  1.0f
bool NeuralNetwork::winningConditionReached()
{
    /* if position and angle were in range for last X iterations then it is a winning state */
    if (inputsCurrent[NN::dInputPosition] > dPOSITION_WINNING) epochWhenPositionEnteredWinningPosition = 0;
    else epochWhenPositionEnteredWinningPosition++;

    if (inputsCurrent[NN::dInputAngularPosition] > dANGLE_WINNING) epochWhenAngleEnteredWinningPosition = 0;
    else epochWhenAngleEnteredWinningPosition++;

    if (  (epochWhenPositionEnteredWinningPosition >= dNUMBER_OF_ITERATIONS_TO_WIN)
       && (epochWhenAngleEnteredWinningPosition >= dNUMBER_OF_ITERATIONS_TO_WIN) )
    {
        return true;
    }
    return false;
}

bool NeuralNetwork::isEpochTimeFinished()
{
    return uEpochCurrentIteration >= dITERAION_NUMBER_MAX;
}

float NeuralNetwork::getReward()
{
    return reward;
}

float NeuralNetwork::getIterator()
{
    return uEpochCurrentIteration;
}


float NeuralNetwork::getAdaptationSpeed()
{
    float adaptationSpeed = 0.25*errorTotalCurrent;

    /* saturate adaptation speed */
    if (adaptationSpeed > 1.0) adaptationSpeed = 1.0;
    else if (adaptationSpeed < -1.0) adaptationSpeed = -1.0;

    return  adaptationSpeed;
}

/*!
 * \brief NeuralNetwork::calculateErrorValue. Example of function that calculates error
 * \return Current error based on given inputs.
 */
float NeuralNetwork::calculateErrorValue()
{
    return 0.125*this->inputsCurrent[NN::dInputPosition]*this->inputsCurrent[NN::dInputPosition];
}

float NeuralNetwork::train(float inputError)
{
#if 0
    /* apply inputs to ann and get the result */
    this->output = inputError + getOutput(inputAngularPosition, inputAngularVelocity,
                                          inputPosition, inputVelocity);

    /* this way, error will be generated only by 'error' - and I can
     * pass there the result of my own cost function! */
    fann_compute_MSE(oNn, &this->output);

    fann_backpropagate_MSE(oNn);
    fann_update_weights(oNn);
    return this->output;
#endif
    return 0.0f;
}

/*!
 * \brief NeuralNetwork::getOutput - used to get output from previously trained network
 * \return - angle shift
 */
float NeuralNetwork::getOutput()
{
    return predictedQ;
}
