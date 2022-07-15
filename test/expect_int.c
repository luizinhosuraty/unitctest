/* expect_int.c - Test EXPECT_{EQ, NEQ, GT, GE} for int types
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */
#include "unitctest/unitctest.h"

TEST(expect_eq, "Test EXPECT_EQ behavior for integral type")
{
	int a;
	int b;

	a = 0;
	b = 0;
	
	EXPECT_EQ(a, b, "a is equal to b, should NOT fail");

	a = -1;
	b = -1;
	EXPECT_EQ(a, b, "a is equal to b, should NOT fail");

	a = 1;
	b = 1;
	EXPECT_EQ(a, b, "a is equal to b, should NOT fail");

}

TEST(expect_neq, "Test EXPECT_NEQ behavior for integral type")
{
	int a;
	int b;

	a = 1;
	b = 0;
	
	EXPECT_NEQ(a, b, "a is not equal to b, should NOT fail");

	a = 0;
	b = -1;
	EXPECT_NEQ(a, b, "a is not equal to b, should NOT fail");

	a = -1;
	b = 1;
	EXPECT_NEQ(a, b, "a is not equal to b, should NOT fail");
}

TEST(expect_gt, "Test EXPECT_GT behavior for integral type")
{
	int a;
	int b;

	a = 2;
	b = 1;
	EXPECT_GT(a, b, "a is greater than to b, should NOT fail");

	a = 1;
	b = 0;
	EXPECT_GT(a, b, "a is greater than to b, should NOT fail");
	
	a = 0;
	b = -1;
	EXPECT_GT(a, b, "a is greater than to b, should NOT fail");

	a = -1;
	b = -2;
	EXPECT_GT(a, b, "a is greater than to b, should NOT fail");

}

TEST(expect_ge, "Test EXPECT_GE behavior for integral type")
{
	int a;
	int b;

	a = 1;
	b = 1;
	EXPECT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");

	a = 1;
	b = 0;
	EXPECT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");
	
	a = -1;
	b = -1;
	EXPECT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");

	a = -1;
	b = -2;
	EXPECT_GE(a, b, "a is greater (or equal) than to b, should NOT fail");

}

TEST_MAIN()
