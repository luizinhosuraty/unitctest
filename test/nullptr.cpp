/* nullptr.c - Test {EXPECT|ASSERT}_NULL for C++
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */
#include "unitctest/unitctest.h"

TEST(expect_null_cpp, "Test EXPECT_NULL for C++")
{
	int *ptr = nullptr;
	EXPECT_NULL(ptr, "Should not fail");
	ASSERT_NULL(ptr, "Should not fail");
}

TEST_MAIN()
