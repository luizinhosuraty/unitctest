/* assert_true_false.c - Test ASSERT_{TRUE,FALSE}
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */
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

TEST(assert_false, "Test ASSERT_FALSE")
{
	int a;
	int b;

	a = 0;
	b = 0;
	ASSERT_FALSE((a != b), "a is equal to b, should NOT fail");
	ASSERT_FALSE((a < b), "a is equal to b, should NOT fail");
}

TEST_MAIN()
