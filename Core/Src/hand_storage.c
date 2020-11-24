/*
 * hand_storage.c
 *
 *  Created on: Nov 24, 2020
 *      Author: black
 */

#include "hand_storage.h"

void HandStorage_SaveFeedback(HandStorageFingersFeedbackDaoStruct feedback) {

}

HandStorageFingersFeedbackDaoStruct HandStorage_GetFeedback() {
	HandStorageFingersFeedbackDaoStruct result;

	result.littleFingerFeedback.minimum = 0;
	result.littleFingerFeedback.maximum = 4095;

	result.ringFingerFeedback.minimum = 0;
	result.ringFingerFeedback.maximum = 4095;

	result.middleFingerFeedback.minimum = 0;
	result.middleFingerFeedback.maximum = 4095;

	result.indexFingerFeedback.minimum = 0;
	result.indexFingerFeedback.maximum = 4095;

	result.thumbFingerFeedback.minimum = 0;
	result.thumbFingerFeedback.maximum = 4095;

	result.thumbEjectorFeedback.minimum = 0;
	result.thumbEjectorFeedback.maximum = 4095;

	return result;
}
