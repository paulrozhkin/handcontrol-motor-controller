/*
 * finger.h
 *
 *  Created on: 21 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#ifndef INC_FINGER_H_
#define INC_FINGER_H_

#include "actuator_controller.h"

typedef uint8_t FingerPositionUnit;

/* Состояния пальца */
enum FingerStateType {
	/* Инициализация пальца */
	FINGER_INITIALIZATION,
	/* Ошибка при инициализации или при работе с пальцем */
	FINGER_ERROR,
	/* Позиция для установки отсутсвует */
	FINGER_REQUIRED_EMPTY,
	/* Запрошена установка позиции. Установка еще не начата. */
	FINGER_REQUEST_SET_POSITION,
	/* Выполняется установка позиции. */
	FINGER_SETING_POSITION,
	/* Текущая позиция пальца не совпадает с требуемой больше, чем на допустимую погрешность. */
	FINGER_DESYNCHRONIZATION,
	/* Палец установил позицию и остановился. Позиция находится в допустимой погрешности. */
	FINGER_SET
};

/* Структура для работы с движущимися частями протеза (пальцами и кистью).
 * Хранит информацию о расчетах энкордера и физическом подключении протеза.
 */
typedef struct {
	/* Текущая позиция пальца. */
	FingerPositionUnit position;

	/* Состояние пальца. */
	enum FingerStateType status;

	/* Ожидаемая позиция пальца. */
	FingerPositionUnit requiredPosition;

	ActuatorStruct actuator;
} FingerStruct;

/**
 * @brief Выполняет инициализацию конфигурации отдельного пальца. Выполняет
 *       установку конфигурации пина в соотвесвтующие поля структуры.
 * @param  finger Структура, которая должна быть инициализирована.
 * @param  actuator Приводов, который является пальцем.
 * @retval None.
 */
void Finger_Init(FingerStruct *finger, ActuatorStruct actuator);

/**
 *	@brief Получить новый feedback и обновить позицию.
 *	@retval None.
 */
void Finger_UpdatePosition();

/**
 *	@brief Запросить установку новой позиции. Установка новой позиции начнется при вызове UpdatePosition.
 *	@param newPosition
 *	@retval None.
 */
void Finger_SetNewPosition(FingerPositionUnit newPosition);

/**
 * @brief Запросить установку requiredPosition. Установка позиции начнется при вызове UpdatePosition.
 * Может быть использована, если палец находится в состоянии FINGER_DESYNCHRONIZATION.
 * @retval None.
 */
void Finger_SetPosition();

/*
 * @brief Выполнить принудительную остановку пальца. requiredPosition будет сброшена.
 * @retval None.
 */
void Finger_Stop();

#endif /* INC_FINGER_H_ */
