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
 * @feedbackMinimum Минимальный feedback привода.
 * @feedbackMaximum Максимальный feedback привода.
 * @feedbackUnitPerAngle Кол-во feedback в 1 единице позиции.
 * @retval Позиция.
 */
FingerPositionUnit PositionToFeedbackConverter_ConvertBack(
		FeedbackUnit feedback, FeedbackUnit feedbackMinimum,
		FeedbackUnit feedbackMaximum, FeedbackUnit feedbackUnitPerAngle);

/**
 * @brief Конвертирует позицию в feedback.
 * @position Позиция.
 * @feedback feedback.
 * @feedbackMinimum Минимальный feedback привода.
 * @feedbackMaximum Максимальный feedback привода.
 * @feedbackUnitPerAngle Кол-во feedback в 1 единице позиции.
 * @retval feedback.
 */
FeedbackUnit PositionToFeedbackConverter_Convert(FingerPositionUnit position,
		FeedbackUnit feedbackMinimum, FeedbackUnit feedbackMaximum,
		FeedbackUnit feedbackUnitPerAngle);

#endif /* INC_POSITION_TO_FEEDBACK_CONVERTER_H_ */
