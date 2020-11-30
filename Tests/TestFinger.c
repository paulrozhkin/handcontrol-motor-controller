#include "unity.h"
#include "unity_fixture.h"
#include "fff.h"
#include "actuator_controller.h"
#include "finger.h"

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(FeedbackUnit, ActuatorController_GetFeedback, ActuatorStruct *);
FAKE_VOID_FUNC(ActuatorController_Stop, ActuatorStruct *);
FAKE_VOID_FUNC(ActuatorController_MoveBackward, ActuatorStruct *);
FAKE_VOID_FUNC(ActuatorController_MoveForward, ActuatorStruct *);
FAKE_VOID_FUNC(ActuatorController_Enable, ActuatorStruct *);
FAKE_VOID_FUNC(ActuatorController_Disable, ActuatorStruct *);

FingerStruct create_finger_mock(void);

TEST_GROUP(Finger);

TEST_SETUP(Finger)
{
}

TEST_TEAR_DOWN(Finger)
{
	RESET_FAKE(ActuatorController_GetFeedback);
	RESET_FAKE(ActuatorController_Stop);
	RESET_FAKE(ActuatorController_MoveBackward);
	RESET_FAKE(ActuatorController_MoveForward);
}

TEST(Finger, Finger_Given_RequestSetPositionState_When_UpdatePosition_Then_SettingPositionState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 700;
	finger.status = FINGER_REQUEST_SET_POSITION;

	// When
	Finger_UpdatePosition(&finger);

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SETTING_POSITION, finger.status);
}

TEST(Finger, Finger_Given_RequestSetPositionStateAndIncorrectLimits_When_UpdatePosition_Then_ErrorState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 700;
	finger.actuatorInfo.backwardFeedbackLimit = 500;
	finger.actuatorInfo.forwardFeedbackLimit = 300;
	finger.status = FINGER_REQUEST_SET_POSITION;

	// When
	Finger_UpdatePosition(&finger);

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_ERROR, finger.status);
}

TEST(Finger, Finger_Given_RequestSetPositionStateWithBackwardDirection_When_UpdatePosition_Then_RequiredDirectionMotionCorrect) 
{
	// Given
	FeedbackUnit defaultPosition = 400;
	ActuatorController_GetFeedback_fake.return_val = defaultPosition;

	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 700;
	finger.status = FINGER_REQUEST_SET_POSITION;

	// When
	Finger_UpdatePosition(&finger);

	// Then
	TEST_ASSERT_EQUAL_INT(defaultPosition, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(FINGER_SETTING_POSITION, finger.status);
	TEST_ASSERT_EQUAL_INT(DIRECTION_BACKWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_RequestSetPositionStateWithForwardDirection_When_UpdatePosition_Then_RequiredDirectionMotionCorrect) 
{
	// Given
	FeedbackUnit defaultPosition = 3000;
	ActuatorController_GetFeedback_fake.return_val = defaultPosition;

	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 500;
	finger.status = FINGER_REQUEST_SET_POSITION;

	// When
	Finger_UpdatePosition(&finger);

	// Then
	TEST_ASSERT_EQUAL_INT(defaultPosition, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(FINGER_SETTING_POSITION, finger.status);
	TEST_ASSERT_EQUAL_INT(DIRECTION_FORWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_SettingsPositionBackward_When_UpdatesPosition_Then_PositionSet) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 1000;
	finger.status = FINGER_REQUEST_SET_POSITION;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;

	// When

	// First call change state from FINGER_REQUEST_SET_POSITION to FINGER_SETTING_POSITION
	ActuatorController_GetFeedback_fake.return_val = 900;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 920;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_MoveBackward_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_BACKWARD;
	}

	ActuatorController_GetFeedback_fake.return_val = 940;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 960;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 970;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 980;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 990;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 1000;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_Stop_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_NONE;
	}

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SET, finger.status);
	TEST_ASSERT_EQUAL_INT(8, ActuatorController_GetFeedback_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_Stop_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_MoveBackward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveForward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1000, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(DIRECTION_BACKWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_SettingsPositionForward_When_UpdatesPosition_Then_PositionSet) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 700;
	finger.status = FINGER_REQUEST_SET_POSITION;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;

	// When
	
	// First call change state from FINGER_REQUEST_SET_POSITION to FINGER_SETTING_POSITION
	ActuatorController_GetFeedback_fake.return_val = 900;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 850;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_MoveForward_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_FORWARD;
	}

	ActuatorController_GetFeedback_fake.return_val = 800;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 740;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 730;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 720;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 710;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 700;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_Stop_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_NONE;
	}

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SET, finger.status);
	TEST_ASSERT_EQUAL_INT(8, ActuatorController_GetFeedback_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_Stop_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveBackward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_MoveForward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(700, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(DIRECTION_FORWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_SettingsPositionForwardWithinDesyncronizationLimit_When_UpdatesPosition_Then_PositionSetWithoutMovement) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 900;
	finger.status = FINGER_REQUEST_SET_POSITION;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;

	// When
	
	// First call change state from FINGER_REQUEST_SET_POSITION to FINGER_SETTING_POSITION
	ActuatorController_GetFeedback_fake.return_val = 860;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 870;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_MoveForward_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_FORWARD;
	}

	ActuatorController_GetFeedback_fake.return_val = 870;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 879;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 880;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 876;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 860;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 870;
	Finger_UpdatePosition(&finger);

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SET, finger.status);
	TEST_ASSERT_EQUAL_INT(8, ActuatorController_GetFeedback_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_Stop_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveBackward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveForward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(870, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(DIRECTION_NONE, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_SettingsPositionForwardInProgress_When_UpdatesPosition_Then_SettingsPositionState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 700;
	finger.status = FINGER_REQUEST_SET_POSITION;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;

	// When
	
	// First call change state from FINGER_REQUEST_SET_POSITION to FINGER_SETTING_POSITION
	ActuatorController_GetFeedback_fake.return_val = 900;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 850;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_MoveForward_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_FORWARD;
	}

	ActuatorController_GetFeedback_fake.return_val = 800;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 790;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 780;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 777;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 760;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 760;
	Finger_UpdatePosition(&finger);

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SETTING_POSITION, finger.status);
	TEST_ASSERT_EQUAL_INT(8, ActuatorController_GetFeedback_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_Stop_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveBackward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_MoveForward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(760, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(DIRECTION_FORWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_SettingsPositionBackwardInProgress_When_UpdatesPosition_Then_SettingsPositionState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 1000;
	finger.status = FINGER_REQUEST_SET_POSITION;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;

	// When

	// First call change state from FINGER_REQUEST_SET_POSITION to FINGER_SETTING_POSITION
	ActuatorController_GetFeedback_fake.return_val = 800;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 850;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_MoveBackward_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_BACKWARD;
	}

	ActuatorController_GetFeedback_fake.return_val = 890;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 900;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 910;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 920;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 930;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 940;
	Finger_UpdatePosition(&finger);

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SETTING_POSITION, finger.status);
	TEST_ASSERT_EQUAL_INT(8, ActuatorController_GetFeedback_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_Stop_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_MoveBackward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveForward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(940, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(DIRECTION_BACKWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_SettingsPositionForward_When_UpdatePositionAndCurrentPositionBeyondRequired_Then_PositionSetState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 700;
	finger.status = FINGER_REQUEST_SET_POSITION;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;

	// When
	
	// First call change state from FINGER_REQUEST_SET_POSITION to FINGER_SETTING_POSITION
	ActuatorController_GetFeedback_fake.return_val = 900;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 850;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_MoveForward_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_FORWARD;
	}

	ActuatorController_GetFeedback_fake.return_val = 800;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 750;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 650;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 670;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 680;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 690;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_Stop_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_NONE;
	}

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SET, finger.status);
	TEST_ASSERT_EQUAL_INT(8, ActuatorController_GetFeedback_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_Stop_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveBackward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_MoveForward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(690, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(DIRECTION_FORWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_SettingsPositionBackward_When_UpdatePositionAndCurrentPositionBeyondRequired_Then_PositionSetState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.requiredActuatorPosition = 1000;
	finger.status = FINGER_REQUEST_SET_POSITION;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;

	// When

	// First call change state from FINGER_REQUEST_SET_POSITION to FINGER_SETTING_POSITION
	ActuatorController_GetFeedback_fake.return_val = 800;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 850;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_MoveBackward_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_BACKWARD;
	}

	ActuatorController_GetFeedback_fake.return_val = 900;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 950;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 1050;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 1070;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 1080;
	Finger_UpdatePosition(&finger);

	ActuatorController_GetFeedback_fake.return_val = 1090;
	Finger_UpdatePosition(&finger);

	if (ActuatorController_Stop_fake.call_count == 1) {
		finger.actuatorInfo.currentDirection = DIRECTION_NONE;
	}

	// Then
	TEST_ASSERT_EQUAL_INT(FINGER_SET, finger.status);
	TEST_ASSERT_EQUAL_INT(8, ActuatorController_GetFeedback_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_Stop_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1, ActuatorController_MoveBackward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(0, ActuatorController_MoveForward_fake.call_count);
	TEST_ASSERT_EQUAL_INT(1090, finger.actuatorPosition);
	TEST_ASSERT_EQUAL_INT(DIRECTION_BACKWARD, finger.requiredDirectionMotion);
}

TEST(Finger, Finger_Given_NewActuatorPosition_When_SetNewPosition_Then_SettingPositionState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.status = FINGER_REQUIRED_EMPTY;
	FeedbackUnit expectedPosition = 1500;

	// When
	Finger_SetNewPosition(&finger, expectedPosition);

	// Then
	TEST_ASSERT_EQUAL_INT(expectedPosition, finger.requiredActuatorPosition);
	TEST_ASSERT_EQUAL_INT(FINGER_REQUEST_SET_POSITION, finger.status);
}

TEST(Finger, Finger_Given_NewAnglePosition_When_SetNewAnglePosition_Then_SettingPositionState) 
{
	// Given
	FingerStruct finger = create_finger_mock();
	finger.status = FINGER_REQUIRED_EMPTY;
	FingerPositionUnit anglePosition = 135;
	FeedbackUnit expectedPosition = 3100;

	FingerStruct finger2 = create_finger_mock();
	finger2.status = FINGER_REQUIRED_EMPTY;
	FingerPositionUnit anglePosition2 = 222;
	FeedbackUnit expectedPosition2 = finger2.actuatorInfo.backwardFeedbackLimit;

	// When
	Finger_SetNewAnglePosition(&finger, anglePosition);
	Finger_SetNewAnglePosition(&finger2, anglePosition2);

	// Then
	TEST_ASSERT_EQUAL_INT(expectedPosition, finger.requiredActuatorPosition);
	TEST_ASSERT_EQUAL_INT(FINGER_REQUEST_SET_POSITION, finger.status);

	TEST_ASSERT_EQUAL_INT(expectedPosition2, finger2.requiredActuatorPosition);
	TEST_ASSERT_EQUAL_INT(FINGER_REQUEST_SET_POSITION, finger2.status);
}


FingerStruct create_finger_mock(void) 
{
	FingerStruct finger;
	finger.requiredDirectionMotion = DIRECTION_NONE;
	finger.status = FINGER_REQUIRED_EMPTY;
	finger.actuatorInfo.backwardFeedbackLimit = 4000;
	finger.actuatorInfo.forwardFeedbackLimit = 400;
	finger.actuatorInfo.feedbackUnitPerAngle = 20;
	finger.actuatorInfo.currentDirection = DIRECTION_NONE;
	return finger;
}