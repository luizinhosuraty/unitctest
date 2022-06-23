#include "unitctest/unitctest.h"

struct mysharedstruct {
	int a;
	int b;
};

struct othersharedstruct {
	int c;
	int d;
};

TEST_FIXTURE(myfixture, mysharedstruct);

TEST_F_SETUP(myfixture)
{
	myfixture->a = 0;
	myfixture->b = 0;
}

TEST_F_TEARDOWN(myfixture)
{
}

TEST_F(myfixture, assert_true, "Test ASSERT_TRUE")
{
	ASSERT_TRUE((myfixture->a == myfixture->b),
		    "a is equal to b, should NOT fail");
	ASSERT_TRUE((myfixture->a >= myfixture->b),
		    "a is equal to b, should NOT fail");
}

TEST_FIXTURE(myotherfixture, mysharedstruct);

TEST_F_SETUP(myotherfixture)
{
	myotherfixture->a = 2;
	myotherfixture->b = 1;
}

TEST_F_TEARDOWN(myotherfixture)
{
}

TEST_F(myotherfixture, assert_true, "Test ASSERT_TRUE")
{
	ASSERT_TRUE((myotherfixture->a == myotherfixture->b + 1),
		    "a is equal to b+1, should NOT fail");
	ASSERT_TRUE((myotherfixture->a >= myotherfixture->b),
		    "a is equal to b, should NOT fail");
}

TEST_FIXTURE(mynextfixture, othersharedstruct);

TEST_F_SETUP(mynextfixture)
{
	mynextfixture->c = 3;
	mynextfixture->d = 2;
}

TEST_F_TEARDOWN(mynextfixture)
{
}

TEST_F(mynextfixture, assert_true, "Test ASSERT_TRUE")
{
	ASSERT_TRUE((mynextfixture->c == mynextfixture->d + 1),
		    "a is equal to b+1, should NOT fail");
	ASSERT_TRUE((mynextfixture->c >= mynextfixture->d),
		    "a is equal to b, should NOT fail");
}
TEST_MAIN()
