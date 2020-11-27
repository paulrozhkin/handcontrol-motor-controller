/*
 * finger_controller.h
 *
 *  Created on: 22 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#ifndef INC_ACTUATOR_CONTROLLER_H_
#define INC_ACTUATOR_CONTROLLER_H_

#include "feedback_reader.h"
#include "gpio.h"

typedef struct {
	/* Текущее положение привода. */
	FeedbackUnit feedback;

	/* Минимальное положение привода. */
	FeedbackUnit feedbackMinimum;

	/* Максимальное положение привода. */
	FeedbackUnit feedbackMaximum;

	/* Кол-во единиц обратной связи на один градус */
	double feedbackUnitPerAngle;

	/* Ридер для чтения обратной связи. */
	FeedbackReaderStruct feedbackReader;

	/* Структура GPIO к которой подключен вывод привода для движения вперед
	 (Движение вперед - при подаче логической единицы на этот пин привод будет
	 выдвигаться). */
	GPIO_TypeDef *gpioForward;

	/* Номер пина к которому подключен вывод привода для движения вперед. */
	uint16_t pinForward;

	/* Структура GPIO к которой подключен вывод привода для движения назад
	 (Движение назад - при подаче логической единицы на этот пин привода будет
	 сжиматься). */
	GPIO_TypeDef *gpioBackward;

	/* Номер пина к которому подключен вывод привода для движения назад. */
	uint16_t pinBackward;
} ActuatorStruct;

/**
 * @brief Выполняет инициализацию конфигурации отдельного пальца. Выполняет
 *       установку конфигурации пина в соотвесвтующие поля структуры.
 * @param actuator Привод, который должна быть инициализирован.
 * @param feedbackMinimum Минимальное положение привода.
 * @param feedbackMaximum Максимальное положение привода.
 * @param feedbackReader Ридер для чтения обратной связи.
 * @param motorForward Структура GPIO к которой подключен вывод привода для движения вперед.
 *       (Движение вперед - при подаче логической единицы на этот пин мотор будет разжимать палец).
 * @param pinForward Номер пина к которому подключен вывод мотора для движения вперед.
 * @param motorBackward Структура GPIO к которой подключен вывод мотора для движения назад.
 *       (Движение назад - при подаче логической единицы на этот пин мотор будет сжимать палец).
 * @param pinBackward Номер пина к которому подключен вывод мотора для движения вперед.
 * @retval None.
 */
void ActuatorController_Init(ActuatorStruct *actuator,
		FeedbackUnit feedbackMinimum, FeedbackUnit feedbackMaximum,
		FeedbackReaderStruct feedbackReader, GPIO_TypeDef *motorForward,
		uint16_t pinForward, GPIO_TypeDef *motorBackward, uint16_t pinBackward);

/**
 * @brief  Движение привода вперед (привод выдвигается).
 * @param actuator Привод.
 * @retval None.
 */
void ActuatorController_MoveForward(ActuatorStruct *actuator);

/**
 * @brief  Движение привода назад (привод сдвигается).
 * @param actuator Привод.
 * @retval None.
 */
void ActuatorController_MoveBackward(ActuatorStruct *actuator);

/**
 * @brief  Остановка движения мотора.
 * @param actuator Привод.
 * @retval None.
 */
void ActuatorController_Stop(ActuatorStruct *actuator);

/**
 * @brief  Выполняет получение обратной связи с привода и обновляет feedback.
 * @param actuator Привод.
 * @retval Feedback с привода.
 */
FeedbackUnit ActuatorController_UpdateFeedback(ActuatorStruct *actuator);

/**
 * @brief Выполнить обновление ограничений обратной связи привода.
 * @param actuator Привод.
 * @param feedbackMinimum Новое минимальное положение привода.
 * @param feedbackMaximum Новое максимальное положение привода.
 * @retval None.
 */
void ActuatorController_UpdateFeedbackLimits(ActuatorStruct *actuator,
		FeedbackUnit feedbackMinimum, FeedbackUnit feedbackMaximum);

#endif /* INC_ACTUATOR_CONTROLLER_H_ */
