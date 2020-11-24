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

/* Текущее состояние работы драйвера линейных двигателей. */
enum TypeWork {
	/* Режим инициализации драйвера. */
	InitializationMode,

	/* В данном режиме происходит автоматический расчет минимальных и максимальных положений кисти и пальцев протеза. */
	AutoCalibrationMode,

	/* В данном режиме происходит ручная установка минимальных и максимальных положений кисти и пальцев протеза. */
	ManualCalibrationMode,

	/* Режим ожидания. Механические действия не исполняются. */
	SleepMode,

	/* Режим установки новых положений. */
	SettingPositionMode,

	/* Режим ошибки. Работа невозможна. */
	ErrorMode
};

/* Позиции всех движущихся частей протеза.*/
typedef struct {
	enum TypeWork currentMode;
	FingerStruct littleFinger;
	FingerStruct ringFinger;
	FingerStruct middleFinger;
	FingerStruct indexFinger;
	FingerStruct thumbFinger;
	FingerStruct thumbEjector;
} HandStruct;

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
void HandController_Init(HandStruct *handStruct,
		FeedbackReaderStruct littleFingerReader, FeedbackReaderStruct ringFingerReader,
		FeedbackReaderStruct middleFingerReader, FeedbackReaderStruct indexFinger,
		FeedbackReaderStruct thumbFinger, FeedbackReaderStruct thumbEjector);

#endif /* INC_HAND_CONTROLLER_H_ */
