/*
 * hand_controller.c
 *
 *  Created on: 23 нояб. 2020 г.
 *      Author: Pavel Rozhkin
 */

#include "hand_controller.h"
#include "string.h"

void HandController_Init(HandStruct *hand,
		FeedbackReaderStruct littleFingerReader,
		FeedbackReaderStruct ringFingerReader,
		FeedbackReaderStruct middleFingerReader,
		FeedbackReaderStruct indexFingerReader,
		FeedbackReaderStruct thumbFingerReader,
		FeedbackReaderStruct thumbEjectorReader) {
	memset(hand, 0, sizeof(HandStruct));

	hand->currentMode = InitializationMode;

	FingerFactory_CreateFinger(&hand->littleFinger, FINGER_NAME_LITTLE,
			littleFingerReader);
	FingerFactory_CreateFinger(&hand->ringFinger, FINGER_NAME_RING,
			ringFingerReader);
	FingerFactory_CreateFinger(&hand->middleFinger, FINGER_NAME_MIDDLE,
			middleFingerReader);
	FingerFactory_CreateFinger(&hand->indexFinger, FINGER_NAME_INDEX,
			indexFingerReader);
	FingerFactory_CreateFinger(&hand->thumbFinger, FINGER_NAME_THUMB,
			thumbFingerReader);
	FingerFactory_CreateFinger(&hand->thumbEjector, FINGER_NAME_THUMB_EJECTOR,
			thumbEjectorReader);
}

void HandController_UpdateFingers(HandStruct *hand) {
	Finger_UpdatePosition(&hand->littleFinger);
	Finger_UpdatePosition(&hand->ringFinger);
	Finger_UpdatePosition(&hand->middleFinger);
	Finger_UpdatePosition(&hand->indexFinger);
	Finger_UpdatePosition(&hand->thumbFinger);
	Finger_UpdatePosition(&hand->thumbEjector);
}
