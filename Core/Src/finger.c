/*
 * finger.c
 *
 *  Created on: 21 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "finger.h"
#include "string.h"

void Finger_Init(FingerStruct *finger, ActuatorStruct actuator) {
	memset(finger, 0, sizeof(FingerStruct));

	finger->actuator = actuator;
	finger->status = FINGER_INITIALIZATION;
}

void Finger_UpdatePosition(FingerStruct *finger) {
	// Обновляем feedback
	FeedbackUnit feedback = ActuatorController_UpdateFeedback(
			&finger->actuator);
	finger->position = Finger_GetPositionFromFeedback(finger, feedback);

	switch (finger->status) {
	case FINGER_INITIALIZATION:
		finger->status = FINGER_REQUIRED_EMPTY;
		break;
	case FINGER_REQUEST_SET_POSITION:
		finger->status = FINGER_SETING_POSITION;
		break;
	case FINGER_SETING_POSITION:
		// TODO: логика установки позиции
		break;
	case FINGER_SET:
		if (finger->position != finger->requiredPosition) {
			finger->status = FINGER_DESYNCHRONIZATION;
		}
		break;
	case FINGER_DESYNCHRONIZATION:
		finger->status = FINGER_SETING_POSITION;
		break;
	default:
		break;
	}
}

FingerPositionUnit Finger_GetPositionFromFeedback(FingerStruct *finger,
		FeedbackUnit feedback) {
	return (FingerPositionUnit) ((double) feedback
			/ (finger->actuator.feedbackMaximum
					- finger->actuator.feedbackMinimum)) * 180;
}
