/*
 * feedback_reader.h
 *
 *  Created on: 22 нояб. 2020 г.
 *      Author: black
 */

#ifndef INC_FEEDBACK_READER_H_
#define INC_FEEDBACK_READER_H_

#include "stdint.h"

typedef uint16_t FeedbackUnit;

typedef struct {
	/**
	 * Указатель на значения с АЦП.
	 */
	uint16_t *rawAdc;
} FeedbackReaderStruct;

/**
 * @brief Иницилизация ридера.
 * @param reader Reader.
 * @param rawAdc Указатель на значения с АЦП.
 * @retval None.
 */
void FeedbackReader_Init(FeedbackReaderStruct *reader, uint16_t *rawAdc);

/**
 * @brief Получить значение калибровки с предобраткой.
 * @param reader Reader.
 * @retval None.
 */
FeedbackUnit FeedbackReader_GetFeedback(FeedbackReaderStruct *reader);

#endif /* INC_FEEDBACK_READER_H_ */
