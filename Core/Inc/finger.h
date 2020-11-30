/*
 * finger.h
 *
 *  Created on: 21 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#ifndef INC_FINGER_H_
#define INC_FINGER_H_

/* Допустимый пределен смещения положения привода при установке. */
#define ACTUATOR_POSITION_OFFSET 40
/* Допустимый пределен смещения положения привода при установленном положении. */
#define ACTUATOR_POSITION_DESYNCHRONIZATION_OFFSET 100
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
	FINGER_SETTING_POSITION,
	/* Текущая позиция пальца не совпадает с требуемой больше, чем на допустимую погрешность. */
	FINGER_DESYNCHRONIZATION,
	/* Палец установил позицию и остановился. Позиция находится в допустимой погрешности. */
	FINGER_SET
};

/* Структура для работы с движущимися частями протеза (пальцами и кистью).
 * Хранит информацию о расчетах энкордера и физическом подключении протеза.
 */
typedef struct {
	/* Текущее положение привода. */
	FeedbackUnit actuatorPosition;

	/* Ожидаемая позиция привода. */
	FeedbackUnit requiredActuatorPosition;

	/* Кол-во раз, когда текущее и требуемое положение привода были одинаковыми */
	int countEqualsCurrentAndRequiredActuatorPosition;

	/* Запрашиваемое направление движения при установке новой позиции. */
	enum DirectionType requiredDirectionMotion;

	/* Состояние пальца. */
	enum FingerStateType status;

	ActuatorStruct actuatorInfo;
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
 *	@brief Запросить установку новой позиции в угловом соотношении. Установка новой позиции начнется при вызове UpdatePosition.
 *	@param finger FingerStruct.
 *	@param newAnglePosition позиция в угловом соотношении.
 *	@retval None.
 */
void Finger_SetNewAnglePosition(FingerStruct *finger, FingerPositionUnit newAnglePosition);

/**
 * @brief Запросить установку requiredPosition. Установка позиции начнется при вызове UpdatePosition.
 * Может быть использована, если палец находится в состоянии FINGER_DESYNCHRONIZATION.
 * @param finger FingerStruct.
 * @retval None.
 */
void Finger_SetPosition(FingerStruct *finger);

/**
 *	@brief Запросить установку новой позиции привода. Установка новой позиции начнется при вызове UpdatePosition.
 *	@param finger FingerStruct.
 *	@param newAnglePosition позиция привода.
 *	@retval None.
 */
void Finger_SetNewPosition(FingerStruct *finger, FeedbackUnit newActuatorPosition);

/*
 * @brief Выполнить принудительную остановку пальца. requiredPosition будет сброшена.
 * @param finger FingerStruct.
 * @retval None.
 */
void Finger_Stop(FingerStruct *finger);

/**
 * @brieft Расчитать позицию пальца в угловом соотношение исходя из обратной связи привода.
 * @param finger FingerStruct.
 * @retval Позиция в угловом соотношении.
 */
FingerPositionUnit Finger_GetAnglePosition(FingerStruct *finger);

/**
 * @brieft Включить привод, который отвечает за палец.
 * @param finger FingerStruct.
 * @retval None.
 */
void Finger_Enable(FingerStruct *finger);

/**
 * @brief Отключить привод, который отвечает за палец.
 * @param finger FingerStruct.
 * @retval None.
 */
void Finger_Disable(FingerStruct *finger);

#endif /* INC_FINGER_H_ */
