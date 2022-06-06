#include "unitctest/unitctest.h"

TEST(expect_true, "Test EXPECT_TRUE")
{
	int a;
	int b;

	a = 0;
	b = 0;
	EXPECT_TRUE((a == b), "a is equal to b, should NOT fail");
	EXPECT_TRUE((a >= b), "a is equal to b, should NOT fail");
}

TEST_MAIN()
