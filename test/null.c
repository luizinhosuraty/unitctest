/* null.c - Test {EXPECT|ASSERT}_NULL
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */
#include "unitctest/unitctest.h"

TEST(expect_null, "Test EXPECT_NULL")
{
	int *ptr = NULL;
	EXPECT_NULL(ptr, "Should not fail");
	ASSERT_NULL(ptr, "Should not fail");
}

TEST_MAIN()
