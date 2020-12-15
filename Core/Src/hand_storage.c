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

	result.littleFingerFeedback.forwardFeedbackLimit = 3800;
	result.littleFingerFeedback.backwardFeedbackLimit = 300;

	result.ringFingerFeedback.forwardFeedbackLimit = 3800;
	result.ringFingerFeedback.backwardFeedbackLimit = 300;

	result.middleFingerFeedback.forwardFeedbackLimit = 3800;
	result.middleFingerFeedback.backwardFeedbackLimit = 300;

	result.indexFingerFeedback.forwardFeedbackLimit = 3800;
	result.indexFingerFeedback.backwardFeedbackLimit = 300;

	result.thumbFingerFeedback.forwardFeedbackLimit = 3800;
	result.thumbFingerFeedback.backwardFeedbackLimit = 300;

	result.thumbEjectorFeedback.forwardFeedbackLimit = 3800;
	result.thumbEjectorFeedback.backwardFeedbackLimit = 300;

	return result;
}
