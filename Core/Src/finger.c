/*
 * finger.c
 *
 *  Created on: 21 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "finger.h"
#include "string.h"
#include <stdlib.h>
#include <stdbool.h>
#include "position_to_feedback_converter.h"

void Finger_Init(FingerStruct *finger, ActuatorStruct actuator) {
	memset(finger, 0, sizeof(FingerStruct));

	finger->actuator = actuator;
	finger->status = FINGER_INITIALIZATION;
}

void Finger_UpdatePosition(FingerStruct *finger) {
	// Обновляем feedback
	FeedbackUnit feedback = ActuatorController_UpdateFeedback(
			&finger->actuator);

	finger->position = PositionToFeedbackConverter_ConvertBack(feedback,
			finger->actuator.feedbackMinimum, finger->actuator.feedbackMaximum,
			finger->actuator.feedbackUnitPerAngle);

	switch (finger->status) {
	case FINGER_INITIALIZATION:
		finger->status = FINGER_REQUIRED_EMPTY;
		break;
	case FINGER_REQUEST_SET_POSITION:
		finger->status = FINGER_SETING_POSITION;
		break;
	case FINGER_SETING_POSITION: {
		bool isPositionsEquals = abs(
				finger->position - finger->previousPosition);
		if (isPositionsEquals) {
			finger->countEqualsCurrentAndPreviousPositionsEmergency++;

			if (MAX_COUNT_EQUALS_POSITION_EMERGENCY
					== finger->countEqualsCurrentAndPreviousPositionsEmergency) {
				ActuatorController_Stop(&finger->actuator);
				finger->status = FINGER_ERROR;
			}
		}

		if (abs(finger->requiredPosition - finger->position) >= POSITION_OFFSET) {
			finger->countEqualsCurrentAndPreviousPositions++;

			if (MAX_COUNT_EQUALS_POSITION
					== finger->countEqualsCurrentAndPreviousPositions) {
				ActuatorController_Stop(&finger->actuator);
				finger->status = FINGER_SET;
			}
		}
		if (finger->requiredPosition > finger->position) {
			ActuatorController_MoveBackward(&finger->actuator);
		} else if (finger->requiredPosition - finger->position) {
			ActuatorController_MoveForward(&finger->actuator);
		} else {
			finger->status = FINGER_ERROR;
		}
		break;
	}
	case FINGER_SET:
		if (abs(finger->requiredPosition < finger->position) >= POSITION_OFFSET) {
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
