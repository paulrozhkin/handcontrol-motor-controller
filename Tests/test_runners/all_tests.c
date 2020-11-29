#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(PositionToFeedbackConverter);
  RUN_TEST_GROUP(Finger);
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
