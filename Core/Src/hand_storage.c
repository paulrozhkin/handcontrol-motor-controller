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

	result.littleFingerFeedback.backwardFeedbackLimit = 4095;
	result.littleFingerFeedback.forwardFeedbackLimit = 0;

	result.ringFingerFeedback.backwardFeedbackLimit = 4095;
	result.ringFingerFeedback.forwardFeedbackLimit = 0;

	result.middleFingerFeedback.backwardFeedbackLimit = 4095;
	result.middleFingerFeedback.forwardFeedbackLimit = 0;

	result.indexFingerFeedback.backwardFeedbackLimit = 4095;
	result.indexFingerFeedback.forwardFeedbackLimit = 0;

	result.thumbFingerFeedback.backwardFeedbackLimit = 4095;
	result.thumbFingerFeedback.forwardFeedbackLimit = 0;

	result.thumbEjectorFeedback.backwardFeedbackLimit = 4095;
	result.thumbEjectorFeedback.forwardFeedbackLimit = 0;

	return result;
}
