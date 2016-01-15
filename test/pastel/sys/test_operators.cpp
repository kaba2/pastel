// Description: Testing for operators
// Documentation: predicate.txt

#include "test/test_init.h"

#include "pastel/sys/operators.h"

#define PASTEL_CHECK_OP(F, OP) \
	for (integer i = -8;i < 8;++i) \
	{ \
		for (integer j = -8;j < 8;++j) \
		{ \
			if (j == 0) {continue;} \
			REQUIRE(F()(i, j) == (i OP j)); \
		} \
	}

TEST_CASE("Operator (operator)")
{
	PASTEL_CHECK_OP(EqualTo, ==);
	PASTEL_CHECK_OP(NotEqualTo, !=);
	PASTEL_CHECK_OP(LessThan, <);
	PASTEL_CHECK_OP(LessEqual, <=);
	PASTEL_CHECK_OP(GreaterThan, <);
	PASTEL_CHECK_OP(GreaterEqual, >=);

	PASTEL_CHECK_OP(Multiplication, *);
	PASTEL_CHECK_OP(Division, /);
	PASTEL_CHECK_OP(Addition, +);
	PASTEL_CHECK_OP(Subtraction, -);
	PASTEL_CHECK_OP(LeftShift, <<);
	PASTEL_CHECK_OP(RightShift, >>);
	PASTEL_CHECK_OP(Quotient, %);
}

