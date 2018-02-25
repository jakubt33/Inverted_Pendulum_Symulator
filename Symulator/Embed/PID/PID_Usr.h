/*
 * PID_Usr.h
 *
 *  Created on: Nov 20, 2015
 *      Author: Kuba
 */

#ifndef PID_Usr_H_
#define PID_Usr_H_

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------Includes-------------------------------------//

//-----------------------Public defines-------------------------------//
#define AngleOffset        3.95f /*! Because the fact that center of the mass of the robot is not
                                     straight above wheels axis, default angle should not be 0 */

#define MinPwmToReact      50.0f /*! Below this value wheels don't turn */

//-----------------------Public macros--------------------------------//

//-----------------------Public typedefs------------------------------//

//-----------------------Public variables-----------------------------//

//-----------------------Public prototypes----------------------------//
float PID_AngleAdjust( float Angle );

#ifdef __cplusplus
}
#endif

#endif /* PID_Usr_H_ */
