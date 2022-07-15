/* expect_true_false.c - Test EXPECT_{TRUE,FALSE}
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */
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
