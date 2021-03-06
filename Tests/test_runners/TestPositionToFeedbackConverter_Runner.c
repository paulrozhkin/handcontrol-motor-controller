#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(PositionToFeedbackConverter)
{
	RUN_TEST_CASE(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_ZeroFeedbackUnitPerAngle_When_ConvertBack_Then_Zero);
	RUN_TEST_CASE(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_Limits_When_GetUnitPerAngle_Then_CorrectUnit);
	RUN_TEST_CASE(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_FeedbackWithinLimits_When_ConvertBack_Then_CorrectConvert);
	RUN_TEST_CASE(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_FeedbackNoWithinLimits_When_ConvertBack_Then_CorrectConvert);
	RUN_TEST_CASE(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_PositionAndNotInitLimits_When_Convert_Then_ZeroResult);
	RUN_TEST_CASE(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_IncorrectPositions_When_Convert_Then_Limits);
	RUN_TEST_CASE(PositionToFeedbackConverter, PositionToFeedbackConverter_Given_PositionsAndLimits_When_Convert_Then_CorrectResults);
}