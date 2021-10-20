/*
 * motors.c
 *
 *  Created on: Sep 10, 2020
 *      Author: Tyler Price
 */

#include "motors.h"

/*
 * This function should return PWM_MAX if pwm > PWM_MAX, -PWM_MAX if pwm < -PWM_MAX, and pwm otherwise.
 */
float limitPWM(float pwm)
{
	
}

/*
 * This function controls the left motor speed and direction. We do this by setting the timer pulse value for the appropriate channel.
 * All our H-bridge inputs use timer 4. TIM4->CCRX = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS) sets the pulse value for channel x.
 * For the left motor, setting channel 2 makes the wheel spin forward and setting channel 1 makes the wheel spin backwards.
 *
 * The parameter pwm is a value between 0 and 1, representing the percentage of maximum speed to turn the motor. e.g. 0.5 is half speed.
 *
 * IMPORTANT: NEVER SET BOTH THE FORWARD AND BACKWARD CHANNELS TO NON-ZERO VALUES AT THE SAME TIME. As mentioned in lecture, this can
 * destroy your h-bridge. When setting a channel, always set the other channel to zero first.
 *
 * Implement this function to make the left wheel spin forwards when pwm is >= 0, and spin backwards when pwm < 0.
 */
void setMotorLPWM(float pwm)
{
	
}

/*
 * This function controls the right motor speed and direction. We do this by setting the timer pulse value for the appropriate channel.
 * All our H-bridge inputs use timer 4. TIM4->CCRX = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS) sets the pulse value for channel x.
 * For the right motor, setting channel 4 makes the wheel spin forward and setting channel 3 makes the wheel spin backwards.
 *
 * The parameter pwm is a value between 0 and 1, representing the percentage of maximum speed to turn the motor. e.g. 0.5 is half speed.
 *
 * IMPORTANT: NEVER SET BOTH THE FORWARD AND BACKWARD CHANNELS TO NON-ZERO VALUES AT THE SAME TIME. As mentioned in lecture, this can
 * destroy your h-bridge. When setting a channel, always set the other channel to zero first.
 *
 * Implement this function to make the right wheel spin forwards when pwm is >= 0, and spin backwards when pwm < 0.
 */
void setMotorRPWM(float pwm)
{
	
}

/*
 * Using the two previous setMotorPWM functions, implement resetMotors to set the PWM for both wheels to zero, effectively stopping them.
 */
void resetMotors()
{
	
}

