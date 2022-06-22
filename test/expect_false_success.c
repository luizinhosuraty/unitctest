#include "unitctest/unitctest.h"

TEST(expect_false, "Test EXPECT_FALSE")
{
	int a;
	int b;

	a = 0;
	b = 0;
	EXPECT_FALSE((a != b), "a is equal to b, should NOT fail");
	EXPECT_FALSE((a < b), "a is equal to b, should NOT fail");
}

TEST_MAIN()
