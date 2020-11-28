/*
 * actuator_controller.c
 *
 *  Created on: 23 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "actuator_controller.h"
#include "string.h"
#include "stdlib.h"
#include "position_to_feedback_converter.h"

void ActuatorController_Init(ActuatorStruct *actuator,
		FeedbackUnit feedbackMinimum, FeedbackUnit feedbackMaximum,
		FeedbackReaderStruct feedbackReader, GPIO_TypeDef *motorForward,
		uint16_t pinForward, GPIO_TypeDef *motorBackward, uint16_t pinBackward) {
	memset(actuator, 0, sizeof(ActuatorStruct));

	actuator->backwardFeedbackLimit = feedbackMinimum;
	actuator->forwardFeedbackLimit = feedbackMaximum;

	actuator->feedbackUnitPerAngle = PositionToFeedbackConverter_GetUnitPerAngle(feedbackMinimum, feedbackMaximum);

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

void ActuatorController_Stop(ActuatorStruct *actuator)
{
	HAL_GPIO_WritePin(actuator->gpioBackward, actuator->pinBackward, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(actuator->gpioForward, actuator->pinForward, GPIO_PIN_RESET);
}

void ActuatorController_MoveBackward(ActuatorStruct *actuator)
{
	HAL_GPIO_WritePin(actuator->gpioBackward, actuator->pinBackward, GPIO_PIN_SET);
	HAL_GPIO_WritePin(actuator->gpioForward, actuator->pinForward, GPIO_PIN_RESET);
}

void ActuatorController_MoveForward(ActuatorStruct *actuator)
{
	HAL_GPIO_WritePin(actuator->gpioBackward, actuator->pinBackward, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(actuator->gpioForward, actuator->pinForward, GPIO_PIN_SET);
}
