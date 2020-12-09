/*
 * position_to_feedback_converter.c
 *
 *  Created on: Nov 28, 2020
 *      Author: Pavel Rozhkin
 */

#include "position_to_feedback_converter.h"
#include <stdlib.h>
#include <math.h>

FingerPositionUnit PositionToFeedbackConverter_ConvertBack(
		FeedbackUnit feedback, FeedbackUnit backwardFeedbackLimit,
		FeedbackUnit forwardFeedbackLimit, double feedbackUnitPerAngle) {

	if (feedbackUnitPerAngle == 0) {
		return 0;
	}

	if (feedback >= forwardFeedbackLimit && feedback <= backwardFeedbackLimit) {
		return (FingerPositionUnit) round(
				(feedback - forwardFeedbackLimit) / feedbackUnitPerAngle);
	} else if (feedback < forwardFeedbackLimit) {
		return 0;
	} else {
		return 180;
	}
}

FeedbackUnit PositionToFeedbackConverter_Convert(FingerPositionUnit position,
		FeedbackUnit backwardFeedbackLimit, FeedbackUnit forwardFeedbackLimit,
		double feedbackUnitPerAngle)
{
	if (feedbackUnitPerAngle == 0) {
		return 0;
	}

	if (position <= 0) {
		return forwardFeedbackLimit;
	}

	if (position >= 180) {
		return backwardFeedbackLimit;
	}

	return (FeedbackUnit) round((position * feedbackUnitPerAngle)) + forwardFeedbackLimit;
}

double PositionToFeedbackConverter_GetUnitPerAngle(FeedbackUnit backwardFeedbackLimit, FeedbackUnit forwardFeedbackLimit)
{
	return abs(forwardFeedbackLimit - backwardFeedbackLimit) / 180.0;
}
