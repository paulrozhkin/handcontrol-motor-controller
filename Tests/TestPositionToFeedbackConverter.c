#include "unity.h"
#include "unity_fixture.h"
#include "position_to_feedback_converter.h"

TEST_GROUP(PositionToFeedbackConverter);

TEST_SETUP(PositionToFeedbackConverter)
{
}

TEST_TEAR_DOWN(PositionToFeedbackConverter)
{
}

TEST(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_ZeroFeedbackUnitPerAngle_When_ConvertBack_Then_Zero)
{
	// Given
	FeedbackUnit feedback = 1;
	FeedbackUnit backwardFeedbackLimit = 2;
	FeedbackUnit forwardFeedbackLimit = 3;
	FeedbackUnit feedbackUnitPerAngle = 0;
	
	// When
	FingerPositionUnit position = PositionToFeedbackConverter_ConvertBack(feedback, backwardFeedbackLimit,
		forwardFeedbackLimit, feedbackUnitPerAngle);

	// Then
	FingerPositionUnit excpectedPosition = 0;
	TEST_ASSERT_EQUAL_INT(excpectedPosition, position);
}

TEST(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_Limits_When_GetUnitPerAngle_Then_CorrectUnit)
{
	// Given
	double excpected1 = 19.444;
	FeedbackUnit backwardFeedbackLimit1 = 4000;
	FeedbackUnit forwardFeedbackLimit1 = 500;

	double excpected2 = 22.75;
	FeedbackUnit backwardFeedbackLimit2 = 4095;
	FeedbackUnit forwardFeedbackLimit2 = 0;

	double excpected3 = 8.33;
	FeedbackUnit backwardFeedbackLimit3 = 3000;
	FeedbackUnit forwardFeedbackLimit3 = 1500;

	double excpected4 = 19.44;
	FeedbackUnit backwardFeedbackLimit4 = 3800;
	FeedbackUnit forwardFeedbackLimit4 = 300;

	double excpected5 = 20.55;
	FeedbackUnit backwardFeedbackLimit5 = 3945;
	FeedbackUnit forwardFeedbackLimit5 = 245;

	// When
	double unitPerPosition1 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit1, forwardFeedbackLimit1);
	double unitPerPosition2 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit2, forwardFeedbackLimit2);
	double unitPerPosition3 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit3, forwardFeedbackLimit3);
	double unitPerPosition4 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit4, forwardFeedbackLimit4);
	double unitPerPosition5 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit5, forwardFeedbackLimit5);

	// Then
	TEST_ASSERT_EQUAL_DOUBLE(excpected1, unitPerPosition1);
	TEST_ASSERT_EQUAL_DOUBLE(excpected2, unitPerPosition2);
	TEST_ASSERT_EQUAL_DOUBLE(excpected3, unitPerPosition3);
	TEST_ASSERT_EQUAL_DOUBLE(excpected4, unitPerPosition4);
	TEST_ASSERT_EQUAL_DOUBLE(excpected5, unitPerPosition5);
}

TEST(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_FeedbackWithinLimits_When_ConvertBack_Then_CorrectConvert)
{
	// Given
	FingerPositionUnit excpectedPosition1 = 90;
	FeedbackUnit feedback1 = 2047;
	FeedbackUnit backwardFeedbackLimit1 = 4095;
	FeedbackUnit forwardFeedbackLimit1 = 0;
	double feedbackUnitPerAngle1 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit1, forwardFeedbackLimit1);

	FingerPositionUnit excpectedPosition2 = 45;
	FeedbackUnit feedback2 = 1144;
	FeedbackUnit backwardFeedbackLimit2 = 3900;
	FeedbackUnit forwardFeedbackLimit2 = 230;
	double feedbackUnitPerAngle2 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit2, forwardFeedbackLimit2);

	FingerPositionUnit excpectedPosition3 = 36;
	FeedbackUnit feedback3 = 1023;
	FeedbackUnit backwardFeedbackLimit3 = 4024;
	FeedbackUnit forwardFeedbackLimit3 = 266;
	double feedbackUnitPerAngle3 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit3, forwardFeedbackLimit3);

	FingerPositionUnit excpectedPosition4 = 180;
	FeedbackUnit feedback4 = 3000;
	FeedbackUnit backwardFeedbackLimit4 = 3000;
	FeedbackUnit forwardFeedbackLimit4 = 2000;
	double feedbackUnitPerAngle4 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit4, forwardFeedbackLimit4);

	FingerPositionUnit excpectedPosition5 = 0;
	FeedbackUnit feedback5 = 2500;
	FeedbackUnit backwardFeedbackLimit5 = 3500;
	FeedbackUnit forwardFeedbackLimit5 = 2500;
	double feedbackUnitPerAngle5 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit5, forwardFeedbackLimit5);
	
	// When
	FingerPositionUnit position1 = PositionToFeedbackConverter_ConvertBack(feedback1, backwardFeedbackLimit1,
		forwardFeedbackLimit1, feedbackUnitPerAngle1);
	FingerPositionUnit position2 = PositionToFeedbackConverter_ConvertBack(feedback2, backwardFeedbackLimit2,
		forwardFeedbackLimit2, feedbackUnitPerAngle2);
	FingerPositionUnit position3 = PositionToFeedbackConverter_ConvertBack(feedback3, backwardFeedbackLimit3,
		forwardFeedbackLimit3, feedbackUnitPerAngle3);
	FingerPositionUnit position4 = PositionToFeedbackConverter_ConvertBack(feedback4, backwardFeedbackLimit4,
		forwardFeedbackLimit4, feedbackUnitPerAngle4);
	FingerPositionUnit position5 = PositionToFeedbackConverter_ConvertBack(feedback5, backwardFeedbackLimit5,
		forwardFeedbackLimit5, feedbackUnitPerAngle5);

	// Then
	TEST_ASSERT_EQUAL_INT(excpectedPosition1, position1);
	TEST_ASSERT_EQUAL_INT(excpectedPosition2, position2);
	TEST_ASSERT_EQUAL_INT(excpectedPosition3, position3);
	TEST_ASSERT_EQUAL_INT(excpectedPosition4, position4);
	TEST_ASSERT_EQUAL_INT(excpectedPosition5, position5);
}

TEST(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_FeedbackNoWithinLimits_When_ConvertBack_Then_CorrectConvert)
{
	// Given
	FingerPositionUnit excpectedPosition1 = 0;
	FeedbackUnit feedback1 = 150;
	FeedbackUnit backwardFeedbackLimit1 = 3600;
	FeedbackUnit forwardFeedbackLimit1 = 1000;
	double feedbackUnitPerAngle1 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit1, forwardFeedbackLimit1);

	FingerPositionUnit excpectedPosition2 = 180;
	FeedbackUnit feedback2 = 4000;
	FeedbackUnit backwardFeedbackLimit2 = 3800;
	FeedbackUnit forwardFeedbackLimit2 = 1000;
	double feedbackUnitPerAngle2 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit2, forwardFeedbackLimit2);
	
	// When
	FingerPositionUnit position1 = PositionToFeedbackConverter_ConvertBack(feedback1, backwardFeedbackLimit1,
		forwardFeedbackLimit1, feedbackUnitPerAngle1);
	FingerPositionUnit position2 = PositionToFeedbackConverter_ConvertBack(feedback2, backwardFeedbackLimit2,
		forwardFeedbackLimit2, feedbackUnitPerAngle2);

	// Then
	TEST_ASSERT_EQUAL_INT(excpectedPosition1, position1);
	TEST_ASSERT_EQUAL_INT(excpectedPosition2, position2);
}

TEST(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_PositionAndNotInitLimits_When_Convert_Then_ZeroResult)
{
	// Given
	FeedbackUnit excpectedFeedback = 0;
	FingerPositionUnit position = 0;
	FeedbackUnit backwardFeedbackLimit = 0;
	FeedbackUnit forwardFeedbackLimit = 0;
	double feedbackUnitPerAngle = 0;

	// When
	FeedbackUnit feedback = PositionToFeedbackConverter_Convert(position, backwardFeedbackLimit,
		forwardFeedbackLimit, feedbackUnitPerAngle);

	// Then
	TEST_ASSERT_EQUAL_INT(excpectedFeedback, feedback);
}

TEST(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_IncorrectPositions_When_Convert_Then_Limits)
{
	// Given
	FeedbackUnit backwardFeedbackLimit = 3000;
	FeedbackUnit forwardFeedbackLimit = 2000;
	double feedbackUnitPerAngle = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit, forwardFeedbackLimit);

	FeedbackUnit excpectedFeedback1 = backwardFeedbackLimit;
	FingerPositionUnit position1 = 255;
	
	FeedbackUnit excpectedFeedback2 = backwardFeedbackLimit;
	FingerPositionUnit position2 = 195;

	// When
	FeedbackUnit feedback1 = PositionToFeedbackConverter_Convert(position1, backwardFeedbackLimit,
		forwardFeedbackLimit, feedbackUnitPerAngle);

	FeedbackUnit feedback2 = PositionToFeedbackConverter_Convert(position2, backwardFeedbackLimit,
		forwardFeedbackLimit, feedbackUnitPerAngle);

	// Then
	TEST_ASSERT_EQUAL_INT(excpectedFeedback1, feedback1);
	TEST_ASSERT_EQUAL_INT(excpectedFeedback2, feedback2);
}

TEST(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_PositionsAndLimits_When_Convert_Then_CorrectResults)
{
	// Given
	FingerPositionUnit position1 = 0;
	FeedbackUnit backwardFeedbackLimit1 = 3000;
	FeedbackUnit forwardFeedbackLimit1 = 2000;
	FeedbackUnit excpectedFeedback1 = forwardFeedbackLimit1;
	double feedbackUnitPerAngle1 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit1, forwardFeedbackLimit1);

	FingerPositionUnit position2 = 180;
	FeedbackUnit backwardFeedbackLimit2 = 3333;
	FeedbackUnit forwardFeedbackLimit2 = 2111;
	FeedbackUnit excpectedFeedback2 = backwardFeedbackLimit2;
	double feedbackUnitPerAngle2 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit2, forwardFeedbackLimit2);

	FingerPositionUnit position3 = 90;
	FeedbackUnit backwardFeedbackLimit3 = 3800;
	FeedbackUnit forwardFeedbackLimit3 = 200;
	FeedbackUnit excpectedFeedback3 = 2000;
	double feedbackUnitPerAngle3 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit3, forwardFeedbackLimit3);

	FingerPositionUnit position4 = 55;
	FeedbackUnit backwardFeedbackLimit4 = 4000;
	FeedbackUnit forwardFeedbackLimit4 = 300;
	FeedbackUnit excpectedFeedback4 = 1431;
	double feedbackUnitPerAngle4 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit4, forwardFeedbackLimit4);

	FingerPositionUnit position5 = 147;
	FeedbackUnit backwardFeedbackLimit5 = 3500;
	FeedbackUnit forwardFeedbackLimit5 = 500;
	FeedbackUnit excpectedFeedback5 = 2950;
	double feedbackUnitPerAngle5 = PositionToFeedbackConverter_GetUnitPerAngle(backwardFeedbackLimit5, forwardFeedbackLimit5);

	// When
	FeedbackUnit feedback1 = PositionToFeedbackConverter_Convert(position1, backwardFeedbackLimit1,
		forwardFeedbackLimit1, feedbackUnitPerAngle1);

	FeedbackUnit feedback2 = PositionToFeedbackConverter_Convert(position2, backwardFeedbackLimit2,
		forwardFeedbackLimit2, feedbackUnitPerAngle2);

	FeedbackUnit feedback3 = PositionToFeedbackConverter_Convert(position3, backwardFeedbackLimit3,
		forwardFeedbackLimit3, feedbackUnitPerAngle3);

	FeedbackUnit feedback4 = PositionToFeedbackConverter_Convert(position4, backwardFeedbackLimit4,
		forwardFeedbackLimit4, feedbackUnitPerAngle4);

	FeedbackUnit feedback5 = PositionToFeedbackConverter_Convert(position5, backwardFeedbackLimit5,
		forwardFeedbackLimit5, feedbackUnitPerAngle5);

	// Then
	TEST_ASSERT_EQUAL_INT(excpectedFeedback1, feedback1);
	TEST_ASSERT_EQUAL_INT(excpectedFeedback2, feedback2);
	TEST_ASSERT_EQUAL_INT(excpectedFeedback3, feedback3);
	TEST_ASSERT_EQUAL_INT(excpectedFeedback4, feedback4);
	TEST_ASSERT_EQUAL_INT(excpectedFeedback5, feedback5);
}