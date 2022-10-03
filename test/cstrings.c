/* cstrings.c - Test {EXPECT|ASSERT}_STR_{EQ|NEQ}
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */
#include "unitctest/unitctest.h"

TEST(test_expect_str_eq, "Test EXPECT_STR_EQ")
{
	EXPECT_STR_EQ("aaaa", "aaaa", "Should not fail");
	EXPECT_STR_EQ("", "", "Should not fail");
}

TEST(test_expect_str_neq, "Test EXPECT_STR_NEQ")
{
	EXPECT_STR_NEQ("aaaa", "aaab", "Should not fail");
	EXPECT_STR_NEQ("a", "b", "Should not fail");
}

TEST(test_assert_str_eq, "Test ASSERT_STR_EQ")
{
	ASSERT_STR_EQ("a", "a", "Should not fail");
	ASSERT_STR_EQ("a12!", "a12!", "Should not fail");
}

TEST(test_assert_str_neq, "Test ASSERT_STR_NEQ")
{
	ASSERT_STR_NEQ("b", "a", "Should not fail");
	ASSERT_STR_NEQ("a", "a ", "Should not fail");
}

TEST_MAIN()
