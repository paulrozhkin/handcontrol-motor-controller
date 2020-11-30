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

	finger->actuatorInfo = actuator;
	finger->status = FINGER_INITIALIZATION;
}

void Finger_UpdatePosition(FingerStruct *finger) {
	// Обновляем feedback
	finger->actuatorPosition = ActuatorController_GetFeedback(
			&finger->actuatorInfo);

	switch (finger->status) {
	case FINGER_INITIALIZATION: {
		finger->status = FINGER_REQUIRED_EMPTY;
		break;
	}
	case FINGER_REQUEST_SET_POSITION: {
		// Если запрашиваемое положение корректное.
		if (finger->requiredActuatorPosition
				>= finger->actuatorInfo.forwardFeedbackLimit
				&& finger->requiredActuatorPosition
						<= finger->actuatorInfo.backwardFeedbackLimit) {

			// Если мы находимся в пределах половины смещения дессинхронизации (~2.5 градуса), то двигаться не надо,
			// т.к. мы не может установить подобную точность.
			if (abs(
					finger->requiredActuatorPosition
							- finger->actuatorPosition) <= (ACTUATOR_POSITION_DESYNCHRONIZATION_OFFSET / 2)) {
				finger->status = FINGER_SET;
				break;
			}

			// Устанвливаем направление движения
			if (finger->requiredActuatorPosition >= finger->actuatorPosition) {
				finger->requiredDirectionMotion = DIRECTION_BACKWARD;
			} else {
				finger->requiredDirectionMotion = DIRECTION_FORWARD;
			}

			finger->countEqualsCurrentAndRequiredActuatorPosition = 0;
			// Начинаем установку положения.
			finger->status = FINGER_SETTING_POSITION;
		} else {
			/* Если запрашиваемое положение не входит в лимиты привода, то устанавливаем ошибку. */
			finger->status = FINGER_ERROR;
		}
		break;
	}
	case FINGER_SETTING_POSITION: {
		bool beyondRequiredPosition = false;

		// Если мы вышли за запрашиваемую позицию при сжатии
		if (finger->requiredDirectionMotion == DIRECTION_BACKWARD
				&& finger->requiredActuatorPosition
						< finger->actuatorPosition) {
			beyondRequiredPosition = true;
		}

		// Если мы вышли за запрашиваемую позицию при разжатии
		if (finger->requiredDirectionMotion == DIRECTION_FORWARD
				&& finger->requiredActuatorPosition
						> finger->actuatorPosition) {
			beyondRequiredPosition = true;
		}

		// Если запрашиваемое положение и текущее положение равны,
		// 	либо мы находимся уже за пределами запрашиваемого положения.
		if (beyondRequiredPosition
				|| abs(
						finger->requiredActuatorPosition
								- finger->actuatorPosition)
						<= ACTUATOR_POSITION_OFFSET) {
			// Увеличиваем счетчик повторений позиции
			finger->countEqualsCurrentAndRequiredActuatorPosition++;

			// Если счетчик повторов превысил максимальне значение, то считаем, что позиция установлена.
			if (MAX_COUNT_EQUALS_POSITION
					== finger->countEqualsCurrentAndRequiredActuatorPosition) {
				ActuatorController_Stop(&finger->actuatorInfo);
				finger->status = FINGER_SET;
				break;
			}
		} else {
			// Сбрасываем счетчик повторов
			finger->countEqualsCurrentAndRequiredActuatorPosition = 0;
		}

		// Начинаем движение мотора в требуемую сторону.
		if (finger->requiredDirectionMotion == DIRECTION_BACKWARD) {
			// Если привод еще не выполняет движение в нужном направление, то начинаем движение.
			if (finger->actuatorInfo.currentDirection != DIRECTION_BACKWARD) {
				ActuatorController_MoveBackward(&finger->actuatorInfo);
			}
		} else if (finger->requiredDirectionMotion == DIRECTION_FORWARD) {
			// Если привод еще не выполняет движение в нужном направление, то начинаем движение.
			if (finger->actuatorInfo.currentDirection != DIRECTION_FORWARD) {
				ActuatorController_MoveForward(&finger->actuatorInfo);
			}
		} else {
			// Ошибка, недопустимое состояние.
			ActuatorController_Stop(&finger->actuatorInfo);
			finger->status = FINGER_ERROR;
		}

		break;
	}
	case FINGER_SET: {
		if (abs(
				finger->requiredActuatorPosition
						- finger->actuatorPosition) >= ACTUATOR_POSITION_DESYNCHRONIZATION_OFFSET) {
			finger->status = FINGER_DESYNCHRONIZATION;
		}
		break;
	}
	default:
		break;
	}
}

void Finger_SetNewAnglePosition(FingerStruct *finger,
		FingerPositionUnit newAnglePosition) {
	FeedbackUnit newPosition = PositionToFeedbackConverter_Convert(
			newAnglePosition, finger->actuatorInfo.backwardFeedbackLimit,
			finger->actuatorInfo.forwardFeedbackLimit,
			finger->actuatorInfo.feedbackUnitPerAngle);

	Finger_SetNewPosition(finger, newPosition);
}

void Finger_SetNewPosition(FingerStruct *finger,
		FeedbackUnit newActuatorPosition) {
	if (finger->status == FINGER_ERROR) {
		return;
	}

	if (finger->status == FINGER_SETTING_POSITION) {
		finger->requiredDirectionMotion = DIRECTION_NONE;
		ActuatorController_Stop(&finger->actuatorInfo);
	}

	finger->requiredActuatorPosition = newActuatorPosition;
	finger->status = FINGER_REQUEST_SET_POSITION;
}

void Finger_SetPosition(FingerStruct *finger) {
	Finger_SetNewPosition(finger, finger->requiredActuatorPosition);
}

FingerPositionUnit Finger_GetAnglePosition(FingerStruct *finger) {
	if (finger->status == FINGER_ERROR) {
		return 0;
	}

	return PositionToFeedbackConverter_ConvertBack(finger->actuatorPosition,
			finger->actuatorInfo.backwardFeedbackLimit,
			finger->actuatorInfo.forwardFeedbackLimit,
			finger->actuatorInfo.feedbackUnitPerAngle);
}

void Finger_Stop(FingerStruct *finger) {
	ActuatorController_Stop(&finger->actuatorInfo);

	if (finger->status != FINGER_ERROR) {
		finger->status = FINGER_REQUIRED_EMPTY;
	}
}

void Finger_Enable(FingerStruct *finger) {
	ActuatorController_Enable(&finger->actuatorInfo);
}

void Finger_Disable(FingerStruct *finger) {
	ActuatorController_Disable(&finger->actuatorInfo);
}
