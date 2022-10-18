/* unitctest.h
 *
 * A Tiny, Header-only Unit Test Framework for C (and C++)
 *
 * Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
 * 
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef _UNITCTEST_H
#define _UNITCTEST_H

#ifdef __cplusplus
#include <iostream>
#include <cstdio>
#else /* __cplusplus */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /* __cplusplus */

#include <getopt.h>

/* -----------------------------------------------------------------------------
 *  Internal private structs and variables
 * -----------------------------------------------------------------------------
 */

typedef void (*__unitctest_func_t)(void);

enum __unitctest_test_result { TEST_SUCCEED, TEST_FAILED, TEST_SKIPPED };

struct __unitctest_test {
	unsigned int id;
	enum __unitctest_test_result result;
	const char *name;
	const char *desc;
	__unitctest_func_t func;
};

struct __unitctest_ctx {
	unsigned int nbr;
	unsigned int verbosity;
	enum __unitctest_test_result result;
	struct __unitctest_test *tests;
	struct __unitctest_test *current_test;
};

extern struct __unitctest_ctx __ctx;

/* -----------------------------------------------------------------------------
 *  Logs & co
 * -----------------------------------------------------------------------------
 */
#ifdef __cplusplus

#define typeof decltype

#define __UNITCTEST_NULL (void *)nullptr

#define __UNITCTEST_STR_COMPARE(op, lhs, rhs) (lhs op rhs)

#define __UNITCTEST_LOG_OPERAND(operand) std::cerr << operand;

#define __UNITCTEST_LOG_NAMESPACE std::

#else /* __cplusplus */

#if (__STDC_VERSION__ >= 201112L)

#define __UNITCTEST_LOG_FMT(x)                                                 \
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
#else /* __STDC_VERSION__ >= 201112L */

#define typeof __typeof__

#define __UNITCTEST_LOG_FMT(x) "%d"

#endif /* __STDC_VERSION__ >= 201112L */

#define __UNITCTEST_NULL NULL

#define __UNITCTEST_STR_COMPARE(op, lhs, rhs) (strcmp(lhs, rhs) op 0)

#define __UNITCTEST_LOG_OPERAND(operand)                                       \
	__UNITCTEST_LOG_ERR(__UNITCTEST_LOG_FMT(operand), operand);

#define __UNITCTEST_LOG_NAMESPACE

#endif /* __cplusplus */

#define __UNITCTEST_LOG(...)                                                   \
	__UNITCTEST_LOG_NAMESPACE fprintf(stdout, __VA_ARGS__);

#define __UNITCTEST_LOG_ERR(...)                                               \
	__UNITCTEST_LOG_NAMESPACE fprintf(stderr, __VA_ARGS__);

#define __UNITCTEST_LOG_TAP(...) __UNITCTEST_LOG(__VA_ARGS__);

#define __UNITCTEST_LOG_TAP_SUCCEED()                                          \
	__UNITCTEST_LOG_TAP("ok %d - %s | %s \n", __ctx.current_test->id,      \
			    __ctx.current_test->name,                          \
			    __ctx.current_test->desc);
#define __UNITCTEST_LOG_TAP_FAILED()                                           \
	__UNITCTEST_LOG_TAP("not ok %d - %s | %s\n", __ctx.current_test->id,   \
			    __ctx.current_test->name,                          \
			    __ctx.current_test->desc);
#define __UNITCTEST_LOG_TAP_SKIPPED()                                          \
	__UNITCTEST_LOG_TAP("ok %d - %s | %s # SKIP\n",                        \
			    __ctx.current_test->id, __ctx.current_test->name,  \
			    __ctx.current_test->desc);
/* -----------------------------------------------------------------------------
 *  Asserts and Expects
 * -----------------------------------------------------------------------------
 */
#define __UNITCTEST_TEST_FAILED()                                              \
	if (__ctx.current_test->result == TEST_SUCCEED) {                      \
		__UNITCTEST_LOG_TAP_FAILED();                                  \
		__ctx.current_test->result = TEST_FAILED;                      \
	}

#define __UNITCTEST_TEST_FAILED_INT(type, op, lhs, rhs, strerr)                \
	__UNITCTEST_TEST_FAILED()                                              \
	if (__ctx.verbosity) {                                                 \
		__UNITCTEST_LOG_ERR(" %s:%d -> ", __FILE__, __LINE__);         \
		__UNITCTEST_LOG_ERR("%s(%s) failed (", type, #lhs #op #rhs);   \
		__UNITCTEST_LOG_OPERAND(lhs);                                  \
		__UNITCTEST_LOG_ERR(" %s ", #op);                              \
		__UNITCTEST_LOG_OPERAND(rhs);                                  \
		__UNITCTEST_LOG_ERR(") -> %s", strerr);                        \
		__UNITCTEST_LOG_ERR("\n");                                     \
	}

#define __UNITCTEST_TEST_FAILED_COND(type, cond, strerr)                       \
	__UNITCTEST_TEST_FAILED()                                              \
	if (__ctx.verbosity) {                                                 \
		__UNITCTEST_LOG_ERR(" %s:%d -> %s(%s) failed -> %s\n",         \
				    __FILE__, __LINE__, type, cond, strerr);   \
	}

#define __UNITCTEST_ASSERT_HELPER_INT(strop, op, lhs, rhs, strerr)             \
	{                                                                      \
		typeof(lhs) _lhs = lhs;                                        \
		typeof(rhs) _rhs = rhs;                                        \
		if (!(_lhs op _rhs)) {                                         \
			__UNITCTEST_TEST_FAILED_INT("ASSERT_" strop, op, lhs,  \
						    rhs, strerr);              \
			return;                                                \
		}                                                              \
	}

#define __UNITCTEST_EXPECT_HELPER_INT(strop, op, lhs, rhs, strerr)             \
	{                                                                      \
		typeof(lhs) _lhs = lhs;                                        \
		typeof(rhs) _rhs = rhs;                                        \
		if (!(_lhs op _rhs)) {                                         \
			__UNITCTEST_TEST_FAILED_INT("EXPECT_" strop, op, lhs,  \
						    rhs, strerr);              \
		}                                                              \
	}

#define __UNITCTEST_ASSERT_HELPER_STR(strop, op, lhs, rhs, strerr)             \
	{                                                                      \
		typeof(lhs) _lhs = lhs;                                        \
		typeof(rhs) _rhs = rhs;                                        \
		if (!(__UNITCTEST_STR_COMPARE(op, _lhs, _rhs))) {              \
			__UNITCTEST_TEST_FAILED_INT("ASSERT_STR" strop, op,    \
						    lhs, rhs, strerr);         \
			return;                                                \
		}                                                              \
	}

#define __UNITCTEST_EXPECT_HELPER_STR(strop, op, lhs, rhs, strerr)             \
	{                                                                      \
		typeof(lhs) _lhs = lhs;                                        \
		typeof(rhs) _rhs = rhs;                                        \
		if (!(__UNITCTEST_STR_COMPARE(op, _lhs, _rhs))) {              \
			__UNITCTEST_TEST_FAILED_INT("EXPECT_STR" strop, op,    \
						    lhs, rhs, strerr);         \
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
			__UNITCTEST_TEST_FAILED_COND("EXPECT_TRUE", #cond,     \
						     strerr);                  \
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
			__UNITCTEST_TEST_FAILED_COND("EXPECT_FALSE", #cond,    \
						     strerr);                  \
		}                                                              \
	}

/*
 * @name EXPECT_EQ
 * @brief Checks whether lhs == rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_EQ(lhs, rhs, strerr)                                            \
	__UNITCTEST_EXPECT_HELPER_INT("EQ", ==, lhs, rhs, strerr)

/*
 * @name EXPECT_NEQ
 * @brief Checks whether lhs != rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_NEQ(lhs, rhs, strerr)                                           \
	__UNITCTEST_EXPECT_HELPER_INT("NEQ", !=, lhs, rhs, strerr)

/*
 * @name EXPECT_NEQ
 * @brief Checks whether lhs > rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_GT(lhs, rhs, strerr)                                            \
	__UNITCTEST_EXPECT_HELPER_INT("GT", >, lhs, rhs, strerr)
/*
 * @name EXPECT_GE
 * @brief Checks whether lhs >= rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_GE(lhs, rhs, strerr)                                            \
	__UNITCTEST_EXPECT_HELPER_INT("GE", >=, lhs, rhs, strerr)
/*
 * @name EXPECT_LT
 * @brief Checks whether lhs < rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_LT(lhs, rhs, strerr)                                            \
	__UNITCTEST_EXPECT_HELPER_INT("LT", <, lhs, rhs, strerr)

/*
 * @name EXPECT_LE
 * @brief Checks whether lhs <= rhs. Test continues even on fail.
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define EXPECT_LE(lhs, rhs, strerr)                                            \
	__UNITCTEST_EXPECT_HELPER_INT("LE", <=, lhs, rhs, strerr)

/*
 * @name EXPECT_STR_EQ
 * @brief Checks whether strings lhs == rhs. Test continues even on fail.
 * @param lhs - left hand-side operand (must be string)
 * @param rhs - right hand-side operand (must be string)
 */
#define EXPECT_STR_EQ(lhs, rhs, strerr)                                        \
	__UNITCTEST_EXPECT_HELPER_STR("EQ", ==, lhs, rhs, strerr)

/*
 * @name EXPECT_STR_NEQ
 * @brief Checks whether strings lhs != rhs. Test continues even on fail.
 * @param lhs - left hand-side operand (must be string)
 * @param rhs - right hand-side operand (must be string)
 */
#define EXPECT_STR_NEQ(lhs, rhs, strerr)                                       \
	__UNITCTEST_EXPECT_HELPER_STR("NEQ", !=, lhs, rhs, strerr)

/*
 * @name EXPECT_NULL
 * @brief Checks whether lhs == NULL. Test continues even on fail.
 * @param lhs - left hand-side operand (must be pointer)
 */
#define EXPECT_NULL(lhs, strerr)                                               \
	__UNITCTEST_EXPECT_HELPER_INT("NULL", ==, lhs, __UNITCTEST_NULL, strerr)

/*
 * @name ASSERT_TRUE
 * @brief Checks whether cond is true. Stop current test if condition is not true
 * @param cond - condition to be tested
 */
#define ASSERT_TRUE(cond, strerr)                                              \
	{                                                                      \
		if (!(cond)) {                                                 \
			__UNITCTEST_TEST_FAILED_COND("ASSERT_TRUE", #cond,     \
						     strerr);                  \
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
			__UNITCTEST_TEST_FAILED_COND("ASSERT_FALSE", #cond,    \
						     strerr);                  \
		}                                                              \
	}

/*
 * @name ASSERT_EQ
 * @brief Checks whether lhs == rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_EQ(lhs, rhs, strerr)                                            \
	__UNITCTEST_ASSERT_HELPER_INT("EQ", ==, lhs, rhs, strerr)

/*
 * @name ASSERT_NEQ
 * @brief Checks whether lhs != rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_NEQ(lhs, rhs, strerr)                                           \
	__UNITCTEST_ASSERT_HELPER_INT("NEQ", !=, lhs, rhs, strerr)
/*
 * @name ASSERT_GT
 * @brief Checks whether lhs > rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_GT(lhs, rhs, strerr)                                            \
	__UNITCTEST_ASSERT_HELPER_INT("GT", >, lhs, rhs, strerr)

/*
 * @name ASSERT_GE
 * @brief Checks whether lhs >= rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_GE(lhs, rhs, strerr)                                            \
	__UNITCTEST_ASSERT_HELPER_INT("GE", >=, lhs, rhs, strerr)

/*
 * @name ASSERT_LT
 * @brief Checks whether lhs < rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_LT(lhs, rhs, strerr)                                            \
	__UNITCTEST_ASSERT_HELPER_INT("LT", <, lhs, rhs, strerr)

/*
 * @name ASSERT_LE
 * @brief Checks whether lhs <= rhs. Stop current test if condition is not true
 * @param lhs - left hand-side operand
 * @param rhs - right hand-side operand
 */
#define ASSERT_LE(lhs, rhs, strerr)                                            \
	__UNITCTEST_ASSERT_HELPER_INT("LE", <=, lhs, rhs, strerr)

/*
 * @name ASSERT_STR_EQ
 * @brief Checks whether strings lhs == rhs. Stop current test if not true
 * @param lhs - left hand-side operand (must be string)
 * @param rhs - right hand-side operand (must be string)
 */
#define ASSERT_STR_EQ(lhs, rhs, strerr)                                        \
	__UNITCTEST_ASSERT_HELPER_STR("EQ", ==, lhs, rhs, strerr)

/*
 * @name ASSERT_STR_NEQ
 * @brief Checks whether strings lhs != rhs. Stop current test if not true
 * @param lhs - left hand-side operand (must be string)
 * @param rhs - right hand-side operand (must be string)
 */
#define ASSERT_STR_NEQ(lhs, rhs, strerr)                                       \
	__UNITCTEST_ASSERT_HELPER_STR("NEQ", !=, lhs, rhs, strerr)

/*
 * @name ASSERT_NULL
 * @brief Checks whether lhs == NULL. Stop current test if condition is not true
 * @param lhs - left hand-side operand (must be pointer)
 */
#define ASSERT_NULL(lhs, strerr)                                               \
	__UNITCTEST_ASSERT_HELPER_INT("NULL", ==, lhs, __UNITCTEST_NULL, strerr)

/* -----------------------------------------------------------------------------
 *  Test Definition
 * -----------------------------------------------------------------------------
 */

/*
 * __attribute__((constructor)) causes this function to be called before main
 */
#define __UNITCTEST_TEST_REGISTER(f)                                           \
	static void f(void) __attribute__((constructor));                      \
	static void f(void)

#define __UNITCTEST_TEST_REALLOC()                                             \
	__ctx.tests = (struct __unitctest_test *)realloc(                      \
		__ctx.tests, sizeof(struct __unitctest_test) * __ctx.nbr);     \
	if (__ctx.tests == NULL) {                                             \
		__UNITCTEST_LOG_ERR("Failed to realloc context\n");            \
		abort();                                                       \
	}
/*
 * @name TEST
 * @brief Defines a test
 * @param tname - single string containing the name of the test
 * @param tdesc - string containing the description of the test (printed on TAP)
 */
#define TEST(tname, tdesc)                                                     \
	static void __unitctest_##tname(void);                                 \
	__UNITCTEST_TEST_REGISTER(__unitctest_register_##tname)                \
	{                                                                      \
		const unsigned int i = __ctx.nbr++;                            \
		__UNITCTEST_TEST_REALLOC();                                    \
		__ctx.tests[i].id = i + 1;                                     \
		__ctx.tests[i].result = TEST_SUCCEED;                          \
		__ctx.tests[i].name = #tname;                                  \
		__ctx.tests[i].desc = tdesc;                                   \
		__ctx.tests[i].func = &__unitctest_##tname;                    \
	}                                                                      \
	void __unitctest_##tname(void)

/*
 * @name TEST_FIXTURE
 * @brief Defines a fixture and the struct that will be shared/available
 * @param tfixture - single string containing the fixture name
 */
#define TEST_FIXTURE(tfixture, tfixturestruct)                                 \
	typedef struct tfixturestruct _type_##tfixture;
/*
 * @name TEST_F_SETUP
 * @brief Defines a fixture setup that will be called before each test
 * @param tfixture - single string containing the fixture name
 */
#define TEST_F_SETUP(tfixture)                                                 \
	static void __unitctest_##tfixture##_setup(_type_##tfixture *);        \
	static void __unitctest_##tfixture##_setup(_type_##tfixture *tfixture)
/*
 * @name TEST_F_TEARDOWN
 * @brief Defines a fixture teardown that will be called after each test
 * @param tfixture - single string containing the fixture name
 */
#define TEST_F_TEARDOWN(tfixture)                                              \
	static void __unitctest_##tfixture##_teardown(_type_##tfixture *);     \
	static void __unitctest_##tfixture##_teardown(                         \
		_type_##tfixture *tfixture)
/*
 * @name TEST_F
 * @brief Defines a test with a fixture
 * @param tfixture - single string containing the fixture name
 * @param tname - single string containing the name of the test
 * @param tdesc - string containing the description of the test (printed on TAP)
 */
#define TEST_F(tfixture, tname, tdesc)                                         \
	static void __unitctest_##tname##_##tfixture(_type_##tfixture *);      \
	static void __unitctest_##tfixture##_##tname(void)                     \
	{                                                                      \
		_type_##tfixture _var_##tfixture;                              \
		__unitctest_##tfixture##_setup(&_var_##tfixture);              \
		if (__ctx.current_test->result != TEST_SUCCEED)                \
			return;                                                \
		__unitctest_##tname##_##tfixture(&_var_##tfixture);            \
		__unitctest_##tfixture##_teardown(&_var_##tfixture);           \
	}                                                                      \
	__UNITCTEST_TEST_REGISTER(__unitctest_register_##tfixture##_##tname)   \
	{                                                                      \
		const unsigned int i = __ctx.nbr++;                            \
		__UNITCTEST_TEST_REALLOC();                                    \
		__ctx.tests[i].id = i + 1;                                     \
		__ctx.tests[i].result = TEST_SUCCEED;                          \
		__ctx.tests[i].name = #tfixture "_" #tname;                    \
		__ctx.tests[i].desc = tdesc;                                   \
		__ctx.tests[i].func = &__unitctest_##tfixture##_##tname;       \
	}                                                                      \
	void __unitctest_##tname##_##tfixture(_type_##tfixture *tfixture)

/*
 * @name TEST_SKIP
 * @brief Set current test as skipped
 */
#define TEST_SKIP() __ctx.current_test->result = TEST_SKIPPED;

/*
 * @name TEST_HAS_FAILED
 * @brief Verify whether test has failed or not. To be used inside a test.
 */
#define TEST_HAS_FAILED() (__ctx.current_test->result == TEST_FAILED);

/*
 * @name TEST_VERBOSE_MODE
 * @brief Verify whether program is running in verbose mode
 */
#define TEST_VERBOSE_MODE() (__ctx.verbosity)

/* -----------------------------------------------------------------------------
 *  Functions to control/run the test suite
 * -----------------------------------------------------------------------------
 */
static inline void __unitctest_log_usage(FILE *fd, int argc, char **argv)
{
	(void)argc;
	fprintf(fd, "Usage : %s [-h] [-v]\n", argv[0]);
	fprintf(fd, " \t -h display this help message\n");
	fprintf(fd, " \t -v run tests on verbose mode\n");
}

static inline void __unitctest_log_result(void)
{
	switch (__ctx.current_test->result) {
	case TEST_SUCCEED:
		__UNITCTEST_LOG_TAP_SUCCEED();
		break;
	case TEST_FAILED:
		__ctx.result = TEST_FAILED;
		/* TAP log occurs during failed expect/assert */
		break;
	case TEST_SKIPPED:
		__UNITCTEST_LOG_TAP_SKIPPED();
		break;
	default:
		fprintf(stderr, "Unknown error\n");
		break;
	}
}

static int __unitctest_init(int argc, char **argv)
{
	int op;

	while ((op = getopt(argc, argv, "hv")) != -1) {
		switch (op) {
		case 'h':
			__unitctest_log_usage(stdout, argc, argv);
			exit(0);
		case 'v':
			__ctx.verbosity = 1;
			break;
		default:
			__unitctest_log_usage(stderr, argc, argv);
			exit(-1);
		}
	}
	__UNITCTEST_LOG_TAP("1..%d\n", __ctx.nbr);
	return 0;
}

static int __unitctest_run(void)
{
	for (unsigned int i = 0; i < __ctx.nbr; i++) {
		__ctx.current_test = &__ctx.tests[i];
		__ctx.current_test->func();
		__unitctest_log_result();
	}
	return 0;
}

static int __unitctest_fini(void)
{
	free(__ctx.tests);
	return 0;
}

#define TEST_MAIN()                                                            \
	struct __unitctest_ctx __ctx = { 0, 0, TEST_SUCCEED, NULL, NULL };     \
	int main(int argc, char **argv)                                        \
	{                                                                      \
		__unitctest_init(argc, argv);                                  \
		__unitctest_run();                                             \
		__unitctest_fini();                                            \
		return (__ctx.result);                                         \
	}

#endif /* _UNITCTEST_H */
