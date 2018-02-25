/*
 * Motors.c
 *
 *  Created on: Oct 25, 2015
 *      Author: Kuba
 */

//-----------------------Includes-------------------------------------//
#include "Motors.h"

//-----------------------Private defines-------------------------------//
/*!
 * For max speed 500RPM and DT=0.128s and encoder resolution 1200 ticks per shaft revolution,
 * maximum possible ticks per iteration is 1280 so any number above that value is safe to use
 * (counter won't over/underflow). Resolution is 1tick*AnglePerTick/DT = 0.0384 deg/sec.
 */
#define CounterDef         1500u /*! Must be less than (tim->period)/2 so counter wont overflow */
#define TicksPerRevolution 1200u
#define AnglePerTick       0.3f
#define RpmPerTIck         0.05f
#define WheelDiameter      8.0f /*! [cm] */
#define Pi                 3.14159256359
#define CmPerTick          ( WheelDiameter * Pi / TicksPerRevolution )

#define SerHorOffset       10.0f
#define SerVerOffset       30.0f

#define AngleMax           ( 90.0f )
#define AngleStep          ( 0.75f )

//-----------------------Private macros--------------------------------//
#define ANGLE_TO_PWM_VALUE(ANGLE)   ( 2 * (ANGLE) + 540 )

//-----------------------Private typedefs------------------------------//

//-----------------------Private variables-----------------------------//
/*! DT_slow defines interval between encoder readings */
extern float DT_slow;

#ifdef ENABLE_HARDWARE
Encoders_C oEncoders = {
   .DistanceLeft  = 0.0f,
   .DistanceRight = 0.0f,
   .OmegaLeft     = 0.0f,
   .OmegaRight    = 0.0f,
   .TIMxLeft      = TIM_ENC1,
   .TIMxRight     = TIM_ENC2,
};
#endif

Motors_C oMotors = {
   .TableMean_RotationDst = {0},
   .TableMean_SpeedDst    = {0}
};


//-----------------------Private prototypes----------------------------//
static float pub_MotorGetNewSpeedDst( float NewValue );
static float pub_MotorGetNewRotationDst( float NewValue );
static void  pub_MotorSetSpeedLeft( float Value );
static void  pub_MotorSetSpeedRight( float Value );
static void  pub_EncoderPerform( void );
static float pub_GetOmegaLeft( void );
static float pub_GetOmegaRight( void );

//-----------------------Private functions-----------------------------//
static float pub_MotorGetNewSpeedDst( float NewValue )
{
   uint8_t Counter;
   /*! Shifting buffer one position right (>>) */
   for( Counter = SpeedMeanLength - 2u; Counter != 0u; Counter-- )
   {
      oMotors.TableMean_SpeedDst[Counter + 1u] = oMotors.TableMean_SpeedDst[Counter];
   }
   oMotors.TableMean_SpeedDst[1] = oMotors.TableMean_SpeedDst[0];
   oMotors.TableMean_SpeedDst[0] = NewValue;

   /*! Add last measurements */
   float Sum = 0.0f;
   for( Counter = SpeedMeanLength - 1u; Counter != 0u; Counter-- )
   {
      Sum += oMotors.TableMean_SpeedDst[Counter];
   }
   Sum += oMotors.TableMean_SpeedDst[0];

   /*! return mean */
   return ( Sum / SpeedMeanLength );
}

static float pub_MotorGetNewRotationDst( float NewValue )
{
   uint8_t Counter;
   /*! Shifting buffer one position right (>>) */
   for( Counter = RotationMeanLength - 2u; Counter != 0u; Counter-- )
   {
      oMotors.TableMean_RotationDst[Counter + 1u] = oMotors.TableMean_RotationDst[Counter];
   }
   oMotors.TableMean_RotationDst[1] = oMotors.TableMean_RotationDst[0];
   oMotors.TableMean_RotationDst[0] = NewValue;

   /*! Add last measurements */
   float Sum = 0.0f;
   for( Counter = RotationMeanLength - 1u; Counter != 0u; Counter-- )
   {
      Sum += oMotors.TableMean_RotationDst[Counter];
   }
   Sum += oMotors.TableMean_RotationDst[0];

   /*! return mean */
   return ( Sum / RotationMeanLength );
}

/*!
 * Value may vary between -1000 and 1000.
 */
static void pub_MotorSetSpeedLeft( float Value )
{
   if( Value <= 1000.0f && Value >= -1000.0f )
   {
      uint8_t Sign = ( Value > 0.0f );
      if( !Sign )
      {
#ifdef ENABLE_HARDWARE
         GPIO_SetBits( MOT1_DIRA_GPIO, MOT1_DIRA_PIN );
         GPIO_ResetBits( MOT1_DIRB_GPIO, MOT1_DIRB_PIN );
#else
          //todo: fill structure
#endif
      }
      else
      {
#ifdef ENABLE_HARDWARE
         GPIO_ResetBits( MOT1_DIRA_GPIO, MOT1_DIRA_PIN );
         GPIO_SetBits( MOT1_DIRB_GPIO, MOT1_DIRB_PIN );
#else
          //todo: fill structure
#endif
      }
#ifdef ENABLE_HARDWARE
      TIM_MOTORS->MOT1_PWM_CHANNEL = Sign ? (uint16_t) Value : (uint16_t) -Value;
#endif
   }
}

/*!
 * Value may vary between -1000 and 1000.
 */
static void pub_MotorSetSpeedRight( float Value )
{
   if( Value <= 1000.0f && Value >= -1000.0f )
   {
      uint8_t Sign = ( Value > 0.0f );
      if( !Sign )
      {
#ifdef ENABLE_HARDWARE
         GPIO_ResetBits( MOT2_DIRA_GPIO, MOT2_DIRA_PIN );
         GPIO_SetBits( MOT2_DIRB_GPIO, MOT2_DIRB_PIN );
#else
          //todo
#endif
      }
      else
      {
 #ifdef ENABLE_HARDWARE
         GPIO_SetBits( MOT2_DIRA_GPIO, MOT2_DIRA_PIN );
         GPIO_ResetBits( MOT2_DIRB_GPIO, MOT2_DIRB_PIN );
#else
          //todo
#endif
      }
 #ifdef ENABLE_HARDWARE
      TIM_MOTORS->MOT2_PWM_CHANNEL = Sign ? (uint16_t) Value : (uint16_t) -Value;
#endif
   }
}

static void pub_EncoderPerform( void )
{
#ifdef ENABLE_HARDWARE
   /*! ------------------------LEFT ENCODER-------------------------*/
   /*! How many ticks since last iteration */
   int16_t DeltaTicks = GetCounter( oEncoders.TIMxLeft ) - CounterDef;

   /*! Calculate angular speed of shaft */
   oEncoders.OmegaLeft = ( DeltaTicks * RpmPerTIck ) / oEncoders.Dt; /*!< Shaft Omega [RPM] */
   oEncoders.DistanceLeft += ( DeltaTicks * CmPerTick );

   /*! Reset counter so it cannot get out of range */
   priv_SetCounter( oEncoders.TIMxLeft, CounterDef );

   /*! ------------------------RIGHT ENCODER------------------------*/
   /*! How many ticks since last iteration */
   DeltaTicks = GetCounter( oEncoders.TIMxRight ) - CounterDef;

   /*! Calculate angular speed of shaft */
   oEncoders.OmegaRight = ( DeltaTicks * RpmPerTIck ) / oEncoders.Dt; /*!< Shaft Omega [RPM] */
   oEncoders.DistanceRight += ( DeltaTicks * CmPerTick );

   /*! Reset counter so it cannot get out of range */
   priv_SetCounter( oEncoders.TIMxRight, CounterDef );
#endif
   //todo:
}

static float pub_GetOmegaLeft( void )
{
#ifdef ENABLE_HARDWARE
   return ( oEncoders.OmegaLeft );
#endif
}

static float pub_GetOmegaRight( void )
{
#ifdef ENABLE_HARDWARE
   return ( oEncoders.OmegaRight );
#endif
}

//-----------------------Public functions------------------------------//
void InitializeEncoders()
{
#ifdef ENABLE_HARDWARE
   /*! Physical initialization */
   InitializeRCC( DriverSelectEncoders );
   InitializeTimers( TIM_ENC1 );
   InitializeTimers( TIM_ENC2 );
   InitializeGPIO( DriverSelectEncoders );

   /*! Software */
   oEncoders.Dt = DT_slow;

   oEncoders.Perform       = pub_EncoderPerform;
   oEncoders.GetOmegaLeft  = pub_GetOmegaLeft;
   oEncoders.GetOmegaRight = pub_GetOmegaRight;
   oEncoders.SetCounter    = priv_SetCounter; /*! Timer function */
#endif
}

void InitializeMotors()
{
#ifdef ENABLE_HARDWARE
   /*! Physical initialization */
   InitializeRCC( DriverSelectMotors );
   InitializeTimers( TIM_MOTORS );
   InitializeGPIO( DriverSelectMotors );
#endif

   /*! Software */
   InitializePIDs();
   oMotors.GetNewRotationDst = pub_MotorGetNewRotationDst;
   oMotors.GetNewSpeedDst    = pub_MotorGetNewSpeedDst;
   oMotors.SetSpeedLeft      = pub_MotorSetSpeedLeft;
   oMotors.SetSpeedRight     = pub_MotorSetSpeedRight;
}


void InitializePIDs()
{
   PID_Initialize( &oPID_Angle );
   oPID_Angle.SetKp( &oPID_Angle.Parameters, 38.0f );
   oPID_Angle.SetKi( &oPID_Angle.Parameters, 4.5f );
   oPID_Angle.SetKd( &oPID_Angle.Parameters, 250.0f );
   oPID_Angle.Parameters.MaxOutSignal = 800.0f; /*!< Max output PWM = 800 + VoltageAdjust */
   oPID_Angle.Parameters.iWindUp = 25.0f;
   oPID_Angle.Parameters.dWindUp = 300.0f;

   PID_Initialize( &oPID_AngleMoving );
   oPID_AngleMoving.SetKp( &oPID_AngleMoving.Parameters, 38.0f );
   oPID_AngleMoving.SetKi( &oPID_AngleMoving.Parameters, 2.5f );
   oPID_AngleMoving.SetKd( &oPID_AngleMoving.Parameters, 250.0f );
   oPID_AngleMoving.Parameters.MaxOutSignal = 8000.0f; /*!< Max output PWM = 800 + VoltageAdjust */
   oPID_AngleMoving.Parameters.iWindUp = 25.0f;
   oPID_AngleMoving.Parameters.dWindUp = 300.0f;

   PID_Initialize( &oPID_Omega );
   oPID_Omega.SetKp( &oPID_Omega.Parameters, 0.038f );
   oPID_Omega.SetKi( &oPID_Omega.Parameters, 0.015f );
   oPID_Omega.SetKd( &oPID_Omega.Parameters, 0.018f );
   oPID_Omega.Parameters.MaxOutSignal = 15.0f; /*!< Max output angle = 15deg+-offset. */
   oPID_Omega.Parameters.iWindUp = 250.0f;
   oPID_Omega.Parameters.dWindUp = 150.0f;

   PID_Initialize( &oPID_Rotation );
   oPID_Rotation.SetKp( &oPID_Rotation.Parameters, 0.2f );
   oPID_Rotation.SetKi( &oPID_Rotation.Parameters, 0.2f );
   oPID_Rotation.SetKd( &oPID_Rotation.Parameters, 0.0f );
   oPID_Rotation.Parameters.MaxOutSignal = 400.0f;
   oPID_Rotation.Parameters.iWindUp = 500.0f;
   oPID_Rotation.Parameters.dWindUp = 150.0f;
}
