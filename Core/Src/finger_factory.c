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

	switch (fingerName) {
	// MOTOR 1
	case FINGER_LITTLE: {
		ActuatorStruct littleActuator;
		ActuatorController_Init(&littleActuator,
				feedbackLimits.littleFingerFeedback.minimum,
				feedbackLimits.littleFingerFeedback.maximum, feedbackReader,
				GPIOB, GPIO_PIN_10,
				GPIOB, GPIO_PIN_11);

		Finger_Init(finger, littleActuator);
		break;
	}
		// MOTOR 2
	case FINGER_RING: {
		ActuatorStruct ringActuator;
		ActuatorController_Init(&ringActuator,
				feedbackLimits.ringFingerFeedback.minimum,
				feedbackLimits.ringFingerFeedback.maximum, feedbackReader,
				GPIOB, GPIO_PIN_0,
				GPIOB, GPIO_PIN_1);

		Finger_Init(finger, ringActuator);
		break;
	}
		// MOTOR 3
	case FINGER_MIDDLE: {
		ActuatorStruct middleActuator;
		ActuatorController_Init(&middleActuator,
				feedbackLimits.ringFingerFeedback.minimum,
				feedbackLimits.ringFingerFeedback.maximum, feedbackReader,
				GPIOB, GPIO_PIN_3,
				GPIOB, GPIO_PIN_12);

		Finger_Init(finger, middleActuator);
		break;
	}
		// MOTOR 4
	case FINGER_INDEX: {
		ActuatorStruct indexActuator;
		ActuatorController_Init(&indexActuator,
				feedbackLimits.indexFingerFeedback.minimum,
				feedbackLimits.indexFingerFeedback.maximum, feedbackReader,
				GPIOB, GPIO_PIN_5,
				GPIOB, GPIO_PIN_4);

		Finger_Init(finger, indexActuator);
		break;
	}
		// MOTOR 5
	case FINGER_THUMB: {
		ActuatorStruct thumbActuator;
		ActuatorController_Init(&thumbActuator,
				feedbackLimits.thumbFingerFeedback.minimum,
				feedbackLimits.thumbFingerFeedback.maximum, feedbackReader,
				GPIOB, GPIO_PIN_6,
				GPIOB, GPIO_PIN_7);

		Finger_Init(finger, thumbActuator);
		break;
	}
		// MOTOR 6
	case FINGER_THUMB_EJECTOR: {
		ActuatorStruct thumbEjectorActuator;
		ActuatorController_Init(&thumbEjectorActuator,
				feedbackLimits.thumbEjectorFeedback.minimum,
				feedbackLimits.thumbEjectorFeedback.maximum, feedbackReader,
				GPIOB, GPIO_PIN_8,
				GPIOB, GPIO_PIN_9);

		Finger_Init(finger, thumbEjectorActuator);
		break;
	}
	default:
		break;
	}
}
