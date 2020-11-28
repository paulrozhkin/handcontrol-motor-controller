/*
 * hand_storage.c
 *
 *  Created on: Nov 24, 2020
 *      Author: black
 */

#include "hand_storage.h"

void HandStorage_SaveFeedback(HandStorageFingersFeedbackDaoStruct feedback) {

}

HandStorageFingersFeedbackDaoStruct HandStorage_GetFeedback(void) {
	HandStorageFingersFeedbackDaoStruct result;

	result.littleFingerFeedback.backwardFeedbackLimit = 3700;
	result.littleFingerFeedback.forwardFeedbackLimit = 300;

	result.ringFingerFeedback.backwardFeedbackLimit = 3700;
	result.ringFingerFeedback.forwardFeedbackLimit = 300;

	result.middleFingerFeedback.backwardFeedbackLimit = 3700;
	result.middleFingerFeedback.forwardFeedbackLimit = 300;

	result.indexFingerFeedback.backwardFeedbackLimit = 3700;
	result.indexFingerFeedback.forwardFeedbackLimit = 300;

	result.thumbFingerFeedback.backwardFeedbackLimit = 3700;
	result.thumbFingerFeedback.forwardFeedbackLimit = 300;

	result.thumbEjectorFeedback.backwardFeedbackLimit = 3700;
	result.thumbEjectorFeedback.forwardFeedbackLimit = 300;

	return result;
}
