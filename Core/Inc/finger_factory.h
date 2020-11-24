/*
 * finger_factory.h
 *
 *  Created on: 23 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#ifndef INC_FINGER_FACTORY_H_
#define INC_FINGER_FACTORY_H_

#include "finger.h"
#include "actuator_controller.h"
#include "hand_storage.h"

/* Тип пальца */
enum FingerNameType {
	FINGER_LITTLE,
	FINGER_RING,
	FINGER_MIDDLE,
	FINGER_INDEX,
	FINGER_THUMB,
	FINGER_THUMB_EJECTOR
};

/**
 * @brief Выполняет иницилизацию пальца
 * @param finger структура, которая должна быть иницилизирована.
 * @param fingerName название пальца для его идентификации.
 * @feedbackReader сервис, который выполняет чтение обратной связи для этого пальца.
 * @retval None.
 */
void FingerFactory_CreateFinger(FingerStruct *finger,
		enum FingerNameType fingerName, FeedbackReaderStruct feedbackReader);

#endif /* INC_FINGER_FACTORY_H_ */
