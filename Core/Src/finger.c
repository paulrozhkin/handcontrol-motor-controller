/*
 * finger.c
 *
 *  Created on: 21 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "finger.h"

void Finger_Init(FingerStruct *finger, ActuatorStruct actuator)
{
	memset(finger, 0, sizeof(FingerStruct));

	finger->actuator = actuator;
	finger->status = FINGER_INITIALIZATION;
}
