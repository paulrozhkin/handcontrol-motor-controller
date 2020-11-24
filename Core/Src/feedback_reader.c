/*
 * feedback_reader.c
 *
 *  Created on: 24 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "feedback_reader.h"
#include "string.h"

void FeedbackReader_Init(FeedbackReaderStruct *reader, uint16_t *rawAdc)
{
	memset(reader, 0, sizeof(FeedbackReaderStruct));

	reader->rawAdc = rawAdc;
}


