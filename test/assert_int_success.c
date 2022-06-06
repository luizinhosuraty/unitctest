#include "unitctest/unitctest.h"

TEST(assert_eq, "Test ASSERT_EQ behavior for integral type")
{
	int a;
	int b;

	a = 0;
	b = 0;
	
	ASSERT_EQ(a, b, "a is equal to b, should NOT fail");

	a = -1;
	b = -1;
	ASSERT_EQ(a, b, "a is equal to b, should NOT fail");

	a = 1;
	b = 1;
	ASSERT_EQ(a, b, "a is equal to b, should NOT fail");
}

TEST(assert_neq, "Test ASSERT_NEQ behavior for integral type")
{
	int a;
	int b;

	a = 1;
	b = 0;
	
	ASSERT_NEQ(a, b, "a is not equal to b, should NOT fail");

	a = 0;
	b = -1;
	ASSERT_NEQ(a, b, "a is not equal to b, should NOT fail");

	a = -1;
	b = 1;
	ASSERT_NEQ(a, b, "a is not equal to b, should NOT fail");
}

TEST(assert_gt, "Test ASSERT_GT behavior for integral type")
{
	int a;
	int b;

	a = 2;
	b = 1;
	ASSERT_GT(a, b, "a is greater than to b, should NOT fail");

	a = 1;
	b = 0;
	ASSERT_GT(a, b, "a is greater than to b, should NOT fail");
	
	a = 0;
	b = -1;
	ASSERT_GT(a, b, "a is greater than to b, should NOT fail");

	a = -1;
	b = -2;
	ASSERT_GT(a, b, "a is greater than to b, should NOT fail");

}

TEST(assert_ge, "Test ASSERT_GE behavior for integral type")
{
	int a;
	int b;

	a = 1;
	b = 1;
	ASSERT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");

	a = 1;
	b = 0;
	ASSERT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");
	
	a = -1;
	b = -1;
	ASSERT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");

	a = -1;
	b = -2;
	ASSERT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");

}

TEST_MAIN()
