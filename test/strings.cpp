/* strings.cpp - Test C++ using string type
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */

#include <string>
#include "unitctest/unitctest.h"

/* Note that we rely on '==' operator for comparisons */
TEST(test_expect_str_eq_cpp, "Test EXPECT_STR_EQ for C++")
{
	EXPECT_STR_EQ("aaaa", "aaaa", "Should not fail");
	EXPECT_STR_EQ("", "", "Should not fail");
}

TEST(test_expect_str_neq_cpp, "Test EXPECT_STR_NEQ for C++")
{
	EXPECT_STR_NEQ("aaaa", "aaab", "Should not fail");
	EXPECT_STR_NEQ("a", "b", "Should not fail");
}

TEST(test_assert_str_eq_cpp, "Test ASSERT_STR_EQ for C++")
{
	ASSERT_STR_EQ("a", "a", "Should not fail");
	ASSERT_STR_EQ("a12!", "a12!", "Should not fail");
}

TEST(test_assert_str_neq_cpp, "Test ASSERT_STR_NEQ for C++")
{
	ASSERT_STR_NEQ("b", "a", "Should not fail");
	ASSERT_STR_NEQ("a", "a ", "Should not fail");
}

TEST_MAIN()
