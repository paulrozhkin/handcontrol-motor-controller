/*
 * finger.h
 *
 *  Created on: 21 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#ifndef INC_FINGER_H_
#define INC_FINGER_H_

/* Допустимый пределен смещения позиций в угловом соотношении. */
#define POSITION_OFFSET 2
/* Максимальное кол-во раз перед остановкой пальца, когда текущая и предыдущая позиция были равны,
 * но позиция не была установлена в требуемое значение. */
#define MAX_COUNT_EQUALS_POSITION_EMERGENCY 50
/* Максимальное кол-во раз равенства текущей и предыдущей позиции перед тем,
 *  как мотор перейдет в состояние успешной установки запрошенной позиции. */
#define MAX_COUNT_EQUALS_POSITION 3

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

	/* Предыдущая позиция пальца. */
	FingerPositionUnit previousPosition;

	/* Кол-во раз, когда текущая и предыдущая позиция были одинаковыми */
	int countEqualsCurrentAndPreviousPositions;

	/* Кол-во раз, когда текущая и предыдущая позиция были одинаковыми, но позиция не была запрашиваемой */
	int countEqualsCurrentAndPreviousPositionsEmergency;

	/* Состояние пальца. */
	enum FingerStateType status;

	/* Ожидаемая позиция пальца. */
	FingerPositionUnit requiredPosition;

	ActuatorStruct actuator;
} FingerStruct;

/**
 * @brief Выполняет инициализацию конфигурации отдельного пальца. Выполняет
 *       установку конфигурации пина в соотвесвтующие поля структуры.
 * @param finger Структура, которая должна быть инициализирована.
 * @param actuator Приводов, который является пальцем.
 * @retval None.
 */
void Finger_Init(FingerStruct *finger, ActuatorStruct actuator);

/**
 *	@brief Получить новый feedback и обновить позицию.
 *	@param finger FingerStruct.
 *	@retval None.
 */
void Finger_UpdatePosition(FingerStruct *finger);

/**
 *	@brief Запросить установку новой позиции. Установка новой позиции начнется при вызове UpdatePosition.
 *	@param finger FingerStruct.
 *	@param newPosition
 *	@retval None.
 */
void Finger_SetNewPosition(FingerStruct *finger, FingerPositionUnit newPosition);

/**
 * @brief Запросить установку requiredPosition. Установка позиции начнется при вызове UpdatePosition.
 * Может быть использована, если палец находится в состоянии FINGER_DESYNCHRONIZATION.
 * @param finger FingerStruct.
 * @retval None.
 */
void Finger_SetPosition(FingerStruct *finger);

/*
 * @brief Выполнить принудительную остановку пальца. requiredPosition будет сброшена.
 * @param finger FingerStruct.
 * @retval None.
 */
void Finger_Stop(FingerStruct *finger);

/**
 * @brieft Расчитать позицию пальца в угловом соотношение исходя из обратной связи привода.
 * @param finger FingerStruct.
 * @param feedback текущая обратная связь.
 * @retval Позиция в угловом соотношении.
 */
FingerPositionUnit Finger_GetPositionFromFeedback(FingerStruct *finger,
		FeedbackUnit feedback);

#endif /* INC_FINGER_H_ */
