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

	hand->state = HAND_STATE_INITIALIZATION;

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
	if (hand->state == HAND_STATE_ERROR) {
		return;
	}

	Finger_UpdatePosition(&hand->littleFinger);
	Finger_UpdatePosition(&hand->ringFinger);
	Finger_UpdatePosition(&hand->middleFinger);
	Finger_UpdatePosition(&hand->indexFinger);
	Finger_UpdatePosition(&hand->thumbFinger);
	Finger_UpdatePosition(&hand->thumbEjector);
}

void HandController_UpdateState(HandStruct *hand) {
	if (hand->state == HAND_STATE_ERROR) {
		return;
	}

	enum FingerStateType littleFingerState = hand->littleFinger.status;
	enum FingerStateType ringFingerState = hand->ringFinger.status;
	enum FingerStateType middleFingerState = hand->middleFinger.status;
	enum FingerStateType indexFingerState = hand->indexFinger.status;
	enum FingerStateType thumbFingerState = hand->thumbFinger.status;
	enum FingerStateType thumbEjectorState = hand->thumbEjector.status;

	// Если пальцы уже установили позицию, либо им не требуется установка, то состояние HAND_STATE_SLEEP.
	if ((littleFingerState == FINGER_SET
			|| littleFingerState == FINGER_DESYNCHRONIZATION
			|| littleFingerState == FINGER_REQUIRED_EMPTY)
			&& (ringFingerState == FINGER_SET
					|| ringFingerState == FINGER_DESYNCHRONIZATION
					|| ringFingerState == FINGER_REQUIRED_EMPTY)
			&& (middleFingerState == FINGER_SET
					|| middleFingerState == FINGER_DESYNCHRONIZATION
					|| middleFingerState == FINGER_REQUIRED_EMPTY)
			&& (indexFingerState == FINGER_SET
					|| indexFingerState == FINGER_DESYNCHRONIZATION
					|| indexFingerState == FINGER_REQUIRED_EMPTY)
			&& (thumbFingerState == FINGER_SET
					|| thumbFingerState == FINGER_DESYNCHRONIZATION
					|| thumbFingerState == FINGER_REQUIRED_EMPTY)
			&& (thumbEjectorState == FINGER_SET
					|| thumbEjectorState == FINGER_DESYNCHRONIZATION
					|| thumbEjectorState == FINGER_REQUIRED_EMPTY)) {
		hand->state = HAND_STATE_SLEEP;

		if (hand->littleFinger.actuatorInfo.isEnabled) {
			HandController_DisableFingers(hand);
		}

		return;
	}

	// Если какой-либо из пальцев находится в ошибке, либо в запросе установке, то состояние HAND_STATE_ERROR.
	if (littleFingerState == FINGER_ERROR || ringFingerState == FINGER_ERROR
			|| middleFingerState == FINGER_ERROR
			|| indexFingerState == FINGER_ERROR
			|| thumbFingerState == FINGER_ERROR
			|| thumbEjectorState == FINGER_ERROR) {
		HandController_Error(hand);
		return;
	}

	// Если какой-либо из пальцев находится в установке позиции, либо в запросе установке, то состояние HAND_STATE_SETTING_POSITIONS.
	if ((littleFingerState == FINGER_SETTING_POSITION
			|| littleFingerState == FINGER_REQUEST_SET_POSITION)
			|| (ringFingerState == FINGER_SETTING_POSITION
					|| ringFingerState == FINGER_REQUEST_SET_POSITION)
			|| (middleFingerState == FINGER_SETTING_POSITION
					|| middleFingerState == FINGER_REQUEST_SET_POSITION)
			|| (indexFingerState == FINGER_SETTING_POSITION
					|| indexFingerState == FINGER_REQUEST_SET_POSITION)
			|| (thumbFingerState == FINGER_SETTING_POSITION
					|| thumbFingerState == FINGER_REQUEST_SET_POSITION)
			|| (thumbEjectorState == FINGER_SETTING_POSITION
					|| thumbEjectorState == FINGER_REQUEST_SET_POSITION)) {
		hand->state = HAND_STATE_SETTING_POSITIONS;

		if (!hand->littleFinger.actuatorInfo.isEnabled) {
			HandController_EnableFingers(hand);
		}

		return;
	}
}

void HandController_SetAnglePositions(HandStruct *hand,
		HandAnglePositionsStruct newPositions) {
	Finger_SetNewAnglePosition(&hand->littleFinger,
			newPositions.littleFingerAnglePosition);
	Finger_SetNewAnglePosition(&hand->ringFinger,
			newPositions.ringFingerAnglePosition);
	Finger_SetNewAnglePosition(&hand->middleFinger,
			newPositions.middleFingerAnglePosition);
	Finger_SetNewAnglePosition(&hand->indexFinger,
			newPositions.indexFingerAnglePosition);
	Finger_SetNewAnglePosition(&hand->thumbFinger,
			newPositions.thumbFingerAnglePosition);
	Finger_SetNewAnglePosition(&hand->thumbEjector,
			newPositions.thumbEjectorAnglePosition);
}

HandAnglePositionsStruct HandController_GetAnglePositions(HandStruct *hand) {
	HandAnglePositionsStruct result;
	result.littleFingerAnglePosition = Finger_GetAnglePosition(
			&hand->littleFinger);
	result.ringFingerAnglePosition = Finger_GetAnglePosition(
			&hand->ringFinger);
	result.middleFingerAnglePosition = Finger_GetAnglePosition(
			&hand->middleFinger);
	result.indexFingerAnglePosition = Finger_GetAnglePosition(
			&hand->indexFinger);
	result.thumbFingerAnglePosition = Finger_GetAnglePosition(
			&hand->thumbFinger);
	result.thumbEjectorAnglePosition = Finger_GetAnglePosition(
			&hand->thumbEjector);

	return result;
}

void HandController_EnableFingers(HandStruct *hand) {
	// Может включать только один палец, т.к. контакт питания общий.
	Finger_Enable(&hand->littleFinger);
}

void HandController_DisableFingers(HandStruct *hand) {
	// Может отключать только один палец, т.к. контакт питания общий.
	Finger_Disable(&hand->littleFinger);
}

void HandController_Error(HandStruct *hand) {
	HandController_DisableFingers(hand);
	hand->state = HAND_STATE_ERROR;
}
