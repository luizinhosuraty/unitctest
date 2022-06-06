#include "unitctest/unitctest.h"

TEST(assert_true, "Test ASSERT_TRUE")
{
	int a;
	int b;

	a = 0;
	b = 0;
	ASSERT_TRUE((a == b), "a is equal to b, should NOT fail");
	ASSERT_TRUE((a >= b), "a is equal to b, should NOT fail");
}

TEST_MAIN()
