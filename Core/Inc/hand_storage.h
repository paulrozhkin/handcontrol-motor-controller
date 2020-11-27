/*
 * hand_storage.h
 *
 *  Created on: 22 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#ifndef INC_HAND_STORAGE_H_
#define INC_HAND_STORAGE_H_

#include "feedback_reader.h"

typedef struct {
	/* Минимальное положение привода. */
	uint16_t minimum;

	/* Максимальное положение привода. */
	uint16_t maximum;
} HandStorageActuatorFeedbackDaoStruct;

typedef struct {
	HandStorageActuatorFeedbackDaoStruct littleFingerFeedback;
	HandStorageActuatorFeedbackDaoStruct ringFingerFeedback;
	HandStorageActuatorFeedbackDaoStruct middleFingerFeedback;
	HandStorageActuatorFeedbackDaoStruct indexFingerFeedback;
	HandStorageActuatorFeedbackDaoStruct thumbFingerFeedback;
	HandStorageActuatorFeedbackDaoStruct thumbEjectorFeedback;
} HandStorageFingersFeedbackDaoStruct;

/**
 * @brief Сохранить информацию калибровки.
 * @param feedback минимальные и максимальные положения при калбировки для каждого пальца.
 * @retval None.
 */
void HandStorage_SaveFeedback(HandStorageFingersFeedbackDaoStruct feedback);

/**
 * @brief Получить информацию калибровки.
 * @retval инимальные и максимальные положения при калбировки для каждого пальца.
 */
HandStorageFingersFeedbackDaoStruct HandStorage_GetFeedback(void);

#endif /* INC_HAND_STORAGE_H_ */
