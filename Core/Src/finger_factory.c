/*
 * finger_factory.c
 *
 *  Created on: 23 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "finger_factory.h"

void FingerFactory_CreateFinger(FingerStruct *finger,
		enum FingerNameType fingerName, FeedbackReaderStruct feedbackReader) {

	HandStorageFingersFeedbackDaoStruct feedbackLimits =
			HandStorage_GetFeedback();

	GPIO_TypeDef *commonGpioEnable = GPIOC;
	uint16_t commonPinEnable = GPIO_PIN_13;

	switch (fingerName) {
	// MOTOR 1
	case FINGER_NAME_LITTLE: {
		ActuatorStruct littleActuator;
		ActuatorController_Init(&littleActuator,
				feedbackLimits.littleFingerFeedback.backwardFeedbackLimit,
				feedbackLimits.littleFingerFeedback.forwardFeedbackLimit,
				feedbackReader,
				GPIOB, GPIO_PIN_10,
				GPIOB, GPIO_PIN_11,
				commonGpioEnable, commonPinEnable);

		Finger_Init(finger, littleActuator);
		break;
	}
		// MOTOR 2
	case FINGER_NAME_RING: {
		ActuatorStruct ringActuator;
		ActuatorController_Init(&ringActuator,
				feedbackLimits.ringFingerFeedback.backwardFeedbackLimit,
				feedbackLimits.ringFingerFeedback.forwardFeedbackLimit,
				feedbackReader,
				GPIOB, GPIO_PIN_0,
				GPIOB, GPIO_PIN_1, commonGpioEnable, commonPinEnable);

		Finger_Init(finger, ringActuator);
		break;
	}
		// MOTOR 3
	case FINGER_NAME_MIDDLE: {
		ActuatorStruct middleActuator;
		ActuatorController_Init(&middleActuator,
				feedbackLimits.ringFingerFeedback.backwardFeedbackLimit,
				feedbackLimits.ringFingerFeedback.forwardFeedbackLimit,
				feedbackReader,
				GPIOB, GPIO_PIN_3,
				GPIOB, GPIO_PIN_12, commonGpioEnable, commonPinEnable);

		Finger_Init(finger, middleActuator);
		break;
	}
		// MOTOR 4
	case FINGER_NAME_INDEX: {
		ActuatorStruct indexActuator;
		ActuatorController_Init(&indexActuator,
				feedbackLimits.indexFingerFeedback.backwardFeedbackLimit,
				feedbackLimits.indexFingerFeedback.forwardFeedbackLimit,
				feedbackReader,
				GPIOB, GPIO_PIN_5,
				GPIOB, GPIO_PIN_4, commonGpioEnable, commonPinEnable);

		Finger_Init(finger, indexActuator);
		break;
	}
		// MOTOR 5
	case FINGER_NAME_THUMB: {
		ActuatorStruct thumbActuator;
		ActuatorController_Init(&thumbActuator,
				feedbackLimits.thumbFingerFeedback.backwardFeedbackLimit,
				feedbackLimits.thumbFingerFeedback.forwardFeedbackLimit,
				feedbackReader,
				GPIOB, GPIO_PIN_6,
				GPIOB, GPIO_PIN_7, commonGpioEnable, commonPinEnable);

		Finger_Init(finger, thumbActuator);
		break;
	}
		// MOTOR 6
	case FINGER_NAME_THUMB_EJECTOR: {
		ActuatorStruct thumbEjectorActuator;
		ActuatorController_Init(&thumbEjectorActuator,
				feedbackLimits.thumbEjectorFeedback.backwardFeedbackLimit,
				feedbackLimits.thumbEjectorFeedback.forwardFeedbackLimit,
				feedbackReader,
				GPIOB, GPIO_PIN_8,
				GPIOB, GPIO_PIN_9, commonGpioEnable, commonPinEnable);

		Finger_Init(finger, thumbEjectorActuator);
		break;
	}
	default:
		break;
	}
}
