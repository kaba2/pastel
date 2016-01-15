// Description: Operators as functions

#ifndef PASTELSYS_OPERATORS_H
#define PASTELSYS_OPERATORS_H

#define PASTEL_UNARY_OPERATOR(NAME, OP) \
	class NAME \
	{ \
	public: \
		template <typename Type> \
		decltype(auto) operator()( \
			const Type& that) const \
		{ \
			return OP that; \
		} \
	};

#define PASTEL_BINARY_OPERATOR(NAME, OP) \
	class NAME \
	{ \
	public: \
		template < \
			typename Left, \
			typename Right> \
		decltype(auto) operator()( \
			const Left& left,  \
			const Right& right) const \
		{ \
			return left OP right; \
		} \
	};

#ifdef _MSC_VER
	// Visual Studio 2015 Update 1 has a bug which gives
	// a spurious warning here.
	#pragma warning(push)
	#pragma warning(disable: 4552)
	#pragma warning(disable: 4553)
#endif

namespace Pastel
{

	PASTEL_UNARY_OPERATOR(Minus, -);
	PASTEL_UNARY_OPERATOR(Plus, +);
	PASTEL_UNARY_OPERATOR(LogicalNot, !);
	PASTEL_UNARY_OPERATOR(BitNot, ~);
	PASTEL_UNARY_OPERATOR(Dereference, *);

	PASTEL_BINARY_OPERATOR(EqualTo, ==);
	PASTEL_BINARY_OPERATOR(NotEqualTo, !=);
	PASTEL_BINARY_OPERATOR(LessThan, <);
	PASTEL_BINARY_OPERATOR(LessEqual, <=);
	PASTEL_BINARY_OPERATOR(GreaterThan, >);
	PASTEL_BINARY_OPERATOR(GreaterEqual, >=);
	PASTEL_BINARY_OPERATOR(Multiplication, *);
	PASTEL_BINARY_OPERATOR(Division, /);
	PASTEL_BINARY_OPERATOR(Quotient, %);
	PASTEL_BINARY_OPERATOR(Addition, +);
	PASTEL_BINARY_OPERATOR(Subtraction, -);
	PASTEL_BINARY_OPERATOR(LeftShift, <<);
	PASTEL_BINARY_OPERATOR(RightShift, >>);
	PASTEL_BINARY_OPERATOR(LogicalAnd, &&);
	PASTEL_BINARY_OPERATOR(LogicalOr, ||);
	PASTEL_BINARY_OPERATOR(BitXor, ^);
	PASTEL_BINARY_OPERATOR(BitAnd, &);
	PASTEL_BINARY_OPERATOR(BitOr, |);

}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif
