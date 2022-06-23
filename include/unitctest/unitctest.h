/* unitctest.h
 *
 * A Tiny, Header-only Unit Test Framework for C/C++
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* -----------------------------------------------------------------------------
 *  Internal private structs and variables
 * -----------------------------------------------------------------------------
 */

typedef void (*_unitctest_func_t)(void);

enum _unitctest_test_result { TEST_SUCCEED, TEST_FAILED, TEST_SKIPPED };

struct _unitctest_test {
	unsigned int id;
	enum _unitctest_test_result result;
	const char *name;
	const char *desc;
	_unitctest_func_t func;
};

struct _unitctest_ctx {
	unsigned int nbr;
	unsigned int verbosity;
	struct _unitctest_test *tests;
	struct _unitctest_test *current_test;
};

extern struct _unitctest_ctx _ctx;

/* -----------------------------------------------------------------------------
 *  Logs & co
 * -----------------------------------------------------------------------------
 */
#if (__STDC_VERSION__ >= 201112L)
#define _UNITCTEST_LOG_FMT(x)                                                  \
	_Generic((x),                                                          \
	char: "%c",                                                            \
	signed char: "%hhd",                                                   \
	unsigned char: "%hhu",                                                 \
	signed short: "%hd",                                                   \
	unsigned short: "%hu",                                                 \
	signed int: "%d",                                                      \
	unsigned int: "%u",                                                    \
	long int: "%ld",                                                       \
	unsigned long int: "%lu",                                              \
	long long int: "%lld",                                                 \
	unsigned long long int: "%llu",                                        \
	float: "%f",                                                           \
	double: "%f",                                                          \
	long double: "%Lf",                                                    \
	char *: "%s",                                                          \
	void *: "%p",                                                          \
	default: "%p")
#else
#define _UNITCTEST_LOG_FMT(x) "%lld"
#endif /* __STDC_VERSION__ >= 201112L */

#define _UNITCTEST_LOG(...) fprintf(stdout, __VA_ARGS__);

#define _UNITCTEST_LOG_ERR(...) fprintf(stderr, __VA_ARGS__);

#define _UNITCTEST_LOG_TAP(...) _UNITCTEST_LOG(__VA_ARGS__);

#define _UNITCTEST_LOG_TAP_SUCCEED()                                           \
	_UNITCTEST_LOG_TAP("ok %d - %s | %s \n", _ctx.current_test->id,        \
			   _ctx.current_test->name, _ctx.current_test->desc);
#define _UNITCTEST_LOG_TAP_FAILED()                                            \
	_UNITCTEST_LOG_TAP("not ok %d - %s | %s\n", _ctx.current_test->id,     \
			   _ctx.current_test->name, _ctx.current_test->desc);
#define _UNITCTEST_LOG_TAP_SKIPPED()                                           \
	_UNITCTEST_LOG_TAP("ok %d - %s | %s # SKIP\n", _ctx.current_test->id,  \
			   _ctx.current_test->name, _ctx.current_test->desc);

/* -----------------------------------------------------------------------------
 *  Asserts and Expects
 * -----------------------------------------------------------------------------
 */
#define _UNITCTEST_TEST_FAILED()                                               \
	if (_ctx.current_test->result == TEST_SUCCEED) {                       \
		_UNITCTEST_LOG_TAP_FAILED();                                   \
		_ctx.current_test->result = TEST_FAILED;                       \
	}

#define _UNITCTEST_TEST_FAILED_INT(type, op, lhs, rhs, strerr)                 \
	_UNITCTEST_TEST_FAILED()                                               \
	if (_ctx.verbosity) {                                                  \
		_UNITCTEST_LOG_ERR(" %s:%d -> ", __FILE__, __LINE__);          \
		_UNITCTEST_LOG_ERR("%s(%s) failed (", type, #lhs #op #rhs);    \
		_UNITCTEST_LOG_ERR(_UNITCTEST_LOG_FMT(lhs), lhs);              \
		_UNITCTEST_LOG_ERR(" %s ", #op);                               \
		_UNITCTEST_LOG_ERR(_UNITCTEST_LOG_FMT(rhs), rhs);              \
		_UNITCTEST_LOG_ERR(") -> %s", strerr);                         \
		_UNITCTEST_LOG_ERR("\n");                                      \
	}

#define _UNITCTEST_TEST_FAILED_COND(type, cond, strerr)                        \
	_UNITCTEST_TEST_FAILED()                                               \
	if (_ctx.verbosity) {                                                  \
		_UNITCTEST_LOG_ERR(" %s:%d -> %s(%s) failed -> %s\n",          \
				   __FILE__, __LINE__, type, cond, strerr);    \
	}

#define _UNITCTEST_ASSERT_HELPER_INT(strop, op, lhs, rhs, strerr)              \
	{                                                                      \
		typeof(lhs) _lhs = lhs;                                        \
		typeof(rhs) _rhs = rhs;                                        \
		if (!(_lhs op _rhs)) {                                         \
			_UNITCTEST_TEST_FAILED_INT("ASSERT_" strop, op, lhs,   \
						   rhs, strerr);               \
			return;                                                \
		}                                                              \
	}

#define _UNITCTEST_EXPECT_HELPER_INT(strop, op, lhs, rhs, strerr)              \
	{                                                                      \
		typeof(lhs) _lhs = lhs;                                        \
		typeof(rhs) _rhs = rhs;                                        \
		if (!(_lhs op _rhs)) {                                         \
			_UNITCTEST_TEST_FAILED_INT("EXPECT_" strop, op, lhs,   \
						   rhs, strerr);               \
		}                                                              \
	}

/*
 * @name EXPECT_TRUE
 * @brief Checks whether cond is true. Test continues even on fail.
 * @param cond - condition to be tested
 */
#define EXPECT_TRUE(cond, strerr)                                              \
	{                                                                      \
		if (!(cond)) {                                                 \
			_UNITCTEST_TEST_FAILED_COND("EXPECT_TRUE", #cond,      \
						    strerr);                   \
		}                                                              \
	}

/*
 * @name EXPECT_FALSE
 * @brief Checks whether cond is false. Test continues even on fail.
 * @param cond - condition to be tested
 */
#define EXPECT_FALSE(cond, strerr)                                             \
	{                                                                      \
		if (cond) {                                                    \
			_UNITCTEST_TEST_FAILED_COND("EXPECT_FALSE", #cond,     \
						    strerr);                   \
		}                                                              \
	}

/*
 * @name EXPECT_EQ
 * @brief Checks whether lhs == rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_EQ(lhs, rhs, strerr)                                            \
	_UNITCTEST_EXPECT_HELPER_INT("EQ", ==, lhs, rhs, strerr)

/*
 * @name EXPECT_NEQ
 * @brief Checks whether lhs != rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_NEQ(lhs, rhs, strerr)                                           \
	_UNITCTEST_EXPECT_HELPER_INT("NEQ", !=, lhs, rhs, strerr)

/*
 * @name EXPECT_NEQ
 * @brief Checks whether lhs > rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_GT(lhs, rhs, strerr)                                            \
	_UNITCTEST_EXPECT_HELPER_INT("GT", >, lhs, rhs, strerr)
/*
 * @name EXPECT_GE
 * @brief Checks whether lhs >= rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_GE(lhs, rhs, strerr)                                            \
	_UNITCTEST_EXPECT_HELPER_INT("GE", >=, lhs, rhs, strerr)
/*
 * @name EXPECT_LT
 * @brief Checks whether lhs < rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_LT(lhs, rhs, strerr)                                            \
	_UNITCTEST_EXPECT_HELPER_INT("LT", <, lhs, rhs, strerr)

/*
 * @name EXPECT_LE
 * @brief Checks whether lhs <= rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_LE(lhs, rhs, strerr)                                            \
	_UNITCTEST_EXPECT_HELPER_INT("LE", <=, lhs, rhs, strerr)

/*
 * @name ASSERT_TRUE
 * @brief Checks whether cond is true. Stop current test if condition is not true
 * @param cond - condition to be tested
 */
#define ASSERT_TRUE(cond, strerr)                                              \
	{                                                                      \
		if (!(cond)) {                                                 \
			_UNITCTEST_TEST_FAILED_COND("ASSERT_TRUE", #cond,      \
						    strerr);                   \
		}                                                              \
	}

/*
 * @name ASSERT_FALSE
 * @brief Checks whether cond is false. Stop current test if condition is not true
 * @param cond - condition to be tested
 */
#define ASSERT_FALSE(cond, strerr)                                             \
	{                                                                      \
		if (cond) {                                                    \
			_UNITCTEST_TEST_FAILED_COND("ASSERT_FALSE", #cond,     \
						    strerr);                   \
		}                                                              \
	}

/*
 * @name ASSERT_EQ
 * @brief Checks whether lhs == rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_EQ(lhs, rhs, strerr)                                            \
	_UNITCTEST_ASSERT_HELPER_INT("EQ", ==, lhs, rhs, strerr)

/*
 * @name ASSERT_NEQ
 * @brief Checks whether lhs != rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_NEQ(lhs, rhs, strerr)                                           \
	_UNITCTEST_ASSERT_HELPER_INT("NEQ", !=, lhs, rhs, strerr)
/*
 * @name ASSERT_GT
 * @brief Checks whether lhs > rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_GT(lhs, rhs, strerr)                                            \
	_UNITCTEST_ASSERT_HELPER_INT("GT", >, lhs, rhs, strerr)

/*
 * @name ASSERT_GE
 * @brief Checks whether lhs >= rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_GE(lhs, rhs, strerr)                                            \
	_UNITCTEST_ASSERT_HELPER_INT("GE", >=, lhs, rhs, strerr)

/*
 * @name ASSERT_LT
 * @brief Checks whether lhs < rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_LT(lhs, rhs, strerr)                                            \
	_UNITCTEST_ASSERT_HELPER_INT("LT", <, lhs, rhs, strerr)

/*
 * @name ASSERT_LE
 * @brief Checks whether lhs <= rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_LE(lhs, rhs, strerr)                                            \
	_UNITCTEST_ASSERT_HELPER_INT("LE", <=, lhs, rhs, strerr)

/* -----------------------------------------------------------------------------
 *  Test Definition
 * -----------------------------------------------------------------------------
 */

/*
 * __attribute__((constructor)) causes this function to be called before main
 */
#define _UNITCTEST_TEST_REGISTER(f)                                            \
	static void f(void) __attribute__((constructor));                      \
	static void f(void)

#define _UNITCTEST_TEST_REALLOC()                                              \
	_ctx.tests = (struct _unitctest_test *)realloc(                        \
		_ctx.tests, sizeof(struct _unitctest_test) * _ctx.nbr);        \
	if (_ctx.tests == NULL) {                                              \
		_UNITCTEST_LOG_ERR("Failed to realloc context\n");             \
		abort();                                                       \
	}
/*
 * @name TEST
 * @brief Defines a test
 * @param tname - single string containing the name of the test
 * @param tdesc - string containing the description of the test (printed on TAP)
 */
#define TEST(tname, tdesc)                                                     \
	static void _unitctest_##tname(void);                                  \
	_UNITCTEST_TEST_REGISTER(_unitctest_register_##tname)                  \
	{                                                                      \
		const unsigned int i = _ctx.nbr++;                             \
		_UNITCTEST_TEST_REALLOC();                                     \
		_ctx.tests[i].id = i + 1;                                      \
		_ctx.tests[i].result = TEST_SUCCEED;                           \
		_ctx.tests[i].name = #tname;                                   \
		_ctx.tests[i].desc = tdesc;                                    \
		_ctx.tests[i].func = &_unitctest_##tname;                      \
	}                                                                      \
	void _unitctest_##tname(void)

/* TODO: Group/Fixture of tests*/
/* #define TEST_G(tgroup, tname, tdesc) */

/*
 * @name TEST_SKIP
 * @brief Set current test as skipped
 */
#define TEST_SKIP() _ctx.current_test->result = TEST_SKIPPED;

/*
 * @name TEST_HAS_FAILED
 * @brief Verify whether test has failed or not. To be used inside a test.
 */
#define TEST_HAS_FAILED() (_ctx.current_test->result == TEST_FAILED);

/*
 * @name TEST_VERBOSE_MODE
 * @brief Verify whether program is running in verbose mode
 */
#define TEST_VERBOSE_MODE() (_ctx.verbosity)

/* -----------------------------------------------------------------------------
 *  Functions to be perform tests
 * -----------------------------------------------------------------------------
 */
static inline void _unitctest_log_result(void)
{
	switch (_ctx.current_test->result) {
	case TEST_SUCCEED:
		_UNITCTEST_LOG_TAP_SUCCEED();
		break;
	case TEST_FAILED:
		/* TAP log occurs during failed expect/assert */
		break;
	case TEST_SKIPPED:
		_UNITCTEST_LOG_TAP_SKIPPED();
		break;
	default:
		fprintf(stderr, "Unknown error\n");
		break;
	}
}

static int _unitctest_init(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int op;

	while ((op = getopt(argc, argv, "v")) != -1) {
		switch (op) {
		case 'v':
			_ctx.verbosity = 1;
			break;
		default:
			abort();
		}
	}
	_UNITCTEST_LOG_TAP("1..%d\n", _ctx.nbr);
	return 0;
}

static int _unitctest_run(void)
{
	for (unsigned int i = 0; i < _ctx.nbr; i++) {
		_ctx.current_test = &_ctx.tests[i];
		_ctx.current_test->func();
		_unitctest_log_result();
	}
	return 0;
}

static int _unitctest_fini(void)
{
	free(_ctx.tests);
	return 0;
}

#define TEST_MAIN()                                                            \
	struct _unitctest_ctx _ctx = { 0, 0, NULL, NULL };                     \
	int main(int argc, char **argv)                                        \
	{                                                                      \
		_unitctest_init(argc, argv);                                   \
		_unitctest_run();                                              \
		_unitctest_fini();                                             \
	}
