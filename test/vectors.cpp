/* vector.cpp - Test C++ using vector type
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */

#include <vector>
#include "unitctest/unitctest.h"

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v)
{
	out << "[";
	for (std::vector<int>::size_type i = 0; i < v.size(); ++i) {
		out << v[i];
		if (i != v.size() - 1)
			out << ", ";
	}
	out << "]";
	return out;
}

/* Note that we rely on '==' operator for comparisons */
TEST(vector, "Check if two vectors are equal (a == b)")
{
	std::vector<int> a = { 0, 0, 1, 0 };
	std::vector<int> b = { 0, 0, 1, 0 };
	EXPECT_EQ(a, b, "Both vectors should be '=='");

	a.push_back(0);
	EXPECT_NEQ(a, b, "a added 0, vectors should NOT be '=='");

	b.push_back(0);
	EXPECT_EQ(a, b, "Both vectors should be '==' again");
}

TEST_MAIN()
