#include "unity.h"
#include "unity_fixture.h"
#include "hand_storage.h"

TEST_GROUP(ProductionCode);

TEST_SETUP(ProductionCode)
{
}

TEST_TEAR_DOWN(ProductionCode)
{
}

TEST(ProductionCode, FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode)
{
	HandStorageFingersFeedbackDaoStruct test = HandStorage_GetFeedback();
	TEST_ASSERT_EQUAL(3700, test.littleFingerFeedback.minimum);
}