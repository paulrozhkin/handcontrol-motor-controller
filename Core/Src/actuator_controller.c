/*
 * actuator_controller.c
 *
 *  Created on: 23 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "actuator_controller.h"
#include "string.h"

void ActuatorController_Init(ActuatorStruct *actuator,
		FeedbackUnit feedbackMinimum, FeedbackUnit feedbackMaximum,
		FeedbackReaderStruct feedbackReader, GPIO_TypeDef *motorForward,
		uint16_t pinForward, GPIO_TypeDef *motorBackward, uint16_t pinBackward) {
	memset(actuator, 0, sizeof(ActuatorStruct));

	actuator->feedbackMinimum = feedbackMinimum;
	actuator->feedbackMaximum = feedbackMaximum;
	actuator->feedbackReader = feedbackReader;
	actuator->gpioForward = motorForward;
	actuator->pinForward = pinForward;
	actuator->gpioBackward = motorBackward;
	actuator->pinBackward = pinBackward;
}

FeedbackUnit ActuatorController_UpdateFeedback(ActuatorStruct *actuator) {
	actuator->feedback = FeedbackReader_GetFeedback(&actuator->feedbackReader);
	return actuator->feedback;
}
