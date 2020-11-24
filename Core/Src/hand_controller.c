/*
 * hand_controller.c
 *
 *  Created on: 23 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "hand_controller.h"
#include "string.h"

void HandController_Init(HandStruct *handStruct,
		FeedbackReaderStruct littleFingerReader, FeedbackReaderStruct ringFingerReader,
		FeedbackReaderStruct middleFingerReader, FeedbackReaderStruct indexFingerReader,
		FeedbackReaderStruct thumbFingerReader, FeedbackReaderStruct thumbEjectorReader)
{
	memset(handStruct, 0, sizeof(HandStruct));

	handStruct->currentMode = InitializationMode;

	FingerFactory_CreateFinger(&handStruct->littleFinger, FINGER_LITTLE, littleFingerReader);
	FingerFactory_CreateFinger(&handStruct->ringFinger, FINGER_RING, ringFingerReader);
	FingerFactory_CreateFinger(&handStruct->middleFinger, FINGER_MIDDLE, middleFingerReader);
	FingerFactory_CreateFinger(&handStruct->indexFinger, FINGER_INDEX, indexFingerReader);
	FingerFactory_CreateFinger(&handStruct->thumbFinger, FINGER_THUMB, thumbFingerReader);
	FingerFactory_CreateFinger(&handStruct->thumbEjector, FINGER_THUMB_EJECTOR, thumbEjectorReader);
}
