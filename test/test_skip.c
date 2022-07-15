/* test_skip.c - Test TEST_SKIP
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */
#include "unitctest/unitctest.h"

TEST(test_skip, "Test TEST_SKIP()")
{
	TEST_SKIP();
}

TEST_MAIN()
