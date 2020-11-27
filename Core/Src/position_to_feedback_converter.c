/*
 * position_to_feedback_converter.c
 *
 *  Created on: Nov 28, 2020
 *      Author: Pavel Rozhkin
 */

#include "position_to_feedback_converter.h"

FingerPositionUnit PositionToFeedbackConverter_ConvertBack(
		FeedbackUnit feedback, FeedbackUnit feedbackMinimum,
		FeedbackUnit feedbackMaximum, FeedbackUnit feedbackUnitPerAngle) {
	if (feedback >= feedbackMinimum && feedback <= feedbackMaximum) {
		return (FingerPositionUnit) (feedback
				/ feedbackUnitPerAngle);
	}
	if (feedback > feedbackMinimum) {
		return 0 - (FingerPositionUnit) (feedback / feedbackUnitPerAngle);
	} else {
		return 180 - (FingerPositionUnit) (feedback / feedbackUnitPerAngle);
	}
}
