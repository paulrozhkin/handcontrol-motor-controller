/*
 * position_converter.h
 *
 *  Created on: Nov 28, 2020
 *      Author: Pavel Rozhkin
 */

#ifndef INC_POSITION_TO_FEEDBACK_CONVERTER_H_
#define INC_POSITION_TO_FEEDBACK_CONVERTER_H_

#include "feedback_reader.h"
#include "finger.h"

/**
 * @brief Конвертирует feedback в позицию.
 * @feedback feedback.
 * @param backwardFeedbackLimit Максимальный feedback привода при сжатии.
 * @param forwardFeedbackLimit Максимальный feedback привода при разжатии.
 * @param feedbackUnitPerAngle Кол-во feedback в 1 единице позиции.
 * @retval Позиция.
 */
FingerPositionUnit PositionToFeedbackConverter_ConvertBack(
		FeedbackUnit feedback, FeedbackUnit backwardFeedbackLimit,
		FeedbackUnit forwardFeedbackLimit, double feedbackUnitPerAngle);

/**
 * @brief Конвертирует позицию в feedback.
 * @param position Позиция.
 * @param feedback feedback.
 * @param backwardFeedbackLimit Максимальный feedback привода при сжатии.
 * @param forwardFeedbackLimit Максимальный feedback привода при разжатии.
 * @param feedbackUnitPerAngle Кол-во feedback в 1 единице позиции.
 * @retval feedback.
 */
FeedbackUnit PositionToFeedbackConverter_Convert(FingerPositionUnit position,
		FeedbackUnit backwardFeedbackLimit, FeedbackUnit forwardFeedbackLimit,
		double feedbackUnitPerAngle);

/**
 * @brief Получить кол-во единиц обратной связи в одной позиции.
 * @param backwardFeedbackLimit Максимальный feedback привода при сжатии.
 * @param forwardFeedbackLimit Максимальный feedback привода при разжатии.
 * @retval Кол-во единиц обратной связи в одной позиции.
 */
double PositionToFeedbackConverter_GetUnitPerAngle(FeedbackUnit backwardFeedbackLimit, FeedbackUnit forwardFeedbackLimit);

#endif /* INC_POSITION_TO_FEEDBACK_CONVERTER_H_ */
