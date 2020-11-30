/*
 * hand_controller.h
 *
 *  Created on: 23 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#ifndef INC_HAND_CONTROLLER_H_
#define INC_HAND_CONTROLLER_H_

#include "feedback_reader.h"
#include "finger_factory.h"
#include "finger.h"

/* Текущее состояние работы драйвера линейных двигателей. */
enum HandStateType {
	/* Режим инициализации драйвера. */
	HAND_STATE_INITIALIZATION,

	/* В данном режиме происходит автоматический расчет минимальных и максимальных положений кисти и пальцев протеза. */
	HAND_STATE_AUTO_CALIBRATION,

	/* В данном режиме происходит ручная установка минимальных и максимальных положений кисти и пальцев протеза. */
	HAND_STATE_MANUAL_CALIBRATION,

	/* Режим ожидания. Механические действия не исполняются. */
	HAND_STATE_SLEEP,

	/* Режим установки новых положений. */
	HAND_STATE_SETTING_POSITIONS,

	/* Режим ошибки. Работа невозможна. */
	HAND_STATE_ERROR
};

/* Позиции всех движущихся частей протеза.*/
typedef struct {
	enum HandStateType state;
	FingerStruct littleFinger;
	FingerStruct ringFinger;
	FingerStruct middleFinger;
	FingerStruct indexFinger;
	FingerStruct thumbFinger;
	FingerStruct thumbEjector;
} HandStruct;

typedef struct {
	FingerPositionUnit littleFingerAnglePosition;
	FingerPositionUnit ringFingerAnglePosition;
	FingerPositionUnit middleFingerAnglePosition;
	FingerPositionUnit indexFingerAnglePosition;
	FingerPositionUnit thumbFingerAnglePosition;
	FingerPositionUnit thumbEjectorAnglePosition;
} HandAnglePositionsStruct;

/**
 * @brief Выполнить инициализацию конфигурации протеза.
 * @param handStruct Структура, которая должна быть инициализирована.
 * @param littleFingerReader ридер обратной связи мезинца пальца.
 * @param ringFingerReader ридер обратной связи безымянного пальца.
 * @param middleFingerReader ридер обратной связи среднего пальца.
 * @param indexFinger ридер обратной связи указательного пальца.
 * @param thumbFinger ридер обратной связи большого пальца.
 * @param thumbEjector ридер обратной связи мезинца раздвигателя большого пальца.
 * @retval None.
 */
void HandController_Init(HandStruct *hand,
		FeedbackReaderStruct littleFingerReader,
		FeedbackReaderStruct ringFingerReader,
		FeedbackReaderStruct middleFingerReader,
		FeedbackReaderStruct indexFinger, FeedbackReaderStruct thumbFinger,
		FeedbackReaderStruct thumbEjector);

/**
 *	@brief Выполняет обновление текущих позиций для каждого пальца.
 *	При необходимости выполняет установку позиций.
 *	@param handStruct HandStruct.
 *	@retval None.
 */
void HandController_UpdateFingers(HandStruct *hand);

/**
 *	@brief Выполняет обновление состояний протеза.
 *	@param hand HandStruct.
 *	@retval None.
 */
void HandController_UpdateState(HandStruct *hand);

/**
 * @brief Установить позиции протеза.
 * @param hand HandStruct.
 * @param newPositions позиции для установки.
 * @retval None.
 */
void HandController_SetAnglePositions(HandStruct *hand,
		HandAnglePositionsStruct newPositions);

/**
 * @brief Получить позиции протеза.
 * @param hand HandStruct.
 * @retval Текущии позиции протеза.
 */
HandAnglePositionsStruct HandController_GetAnglePositions(HandStruct *hand);

/**
 * @brief Включает питание всех пальцев (приводов).
 * @param hand HandStruct.
 * @retval None.
 */
void HandController_EnableFingers(HandStruct *hand);

/**
 * @brief Отключает питание всех пальцев (приводов).
 * @param hand HandStruct.
 * @retval None.
 */
void HandController_DisableFingers(HandStruct *hand);

/**
 * @brief установить ошибку протеза.
 * @hand HandStruct.
 * @retval None.
 */
void HandController_Error(HandStruct *hand);

#endif /* INC_HAND_CONTROLLER_H_ */
