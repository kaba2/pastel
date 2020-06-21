// Description: Dimension of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_DIMENSION_H
#define PASTELSYS_POINT_DIMENSION_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_ring_concept.h"
#include <array>

namespace Pastel
{

	// Dimension by having member .pointDimension().

	template <
		typename Point,
		typename = decltype(Concept::convertsTo<integer>(std::declval<Point>().pointDimension()))>
	struct Point_HasMemberDimension_Test {};

	template <typename Type>
	using Point_HasMemberDimension = 
		Compiles<Point_HasMemberDimension_Test, Type>;

	template <typename T>
	concept Point_HasMemberDimension_ = requires(T t) {
		{t.pointDimension()} -> std::convertible_to<integer>;
	};

	template <Point_HasMemberDimension_ Point>
	decltype(auto) dimension(Point&& point)
	{
		return std::forward<Point>(point).pointDimension();
	}

}

namespace Pastel
{

	// Dimension by having member .size().

	template <
		typename Point,
		typename = decltype(Concept::convertsTo<integer>(std::declval<Point>().size()))>
	struct Point_HasMemberSize_Test {};
	
	template <typename Type>
	using Point_HasMemberSize = 
		Compiles<Point_HasMemberSize_Test, Type>;

	template <typename T>
	concept Point_HasMemberSize_ = requires(T t) {
		{addConst(t).size()} -> std::convertible_to<integer>;
	};

	template <
		typename Point,
		Requires<
			Point_HasMemberSize<Point>,
			Not<Point_HasMemberDimension<Point>>
		> = 0
	>
	decltype(auto) dimension(Point&& point)
	{
		return std::forward<Point>(point).size();
	}

}

namespace Pastel
{

	// Dimension by being a number.

	template <
		typename Point,
		Requires<
			Models<Point, Real_Ring_Concept>,
			Not<Point_HasMemberSize<Point>>,
			Not<Point_HasMemberDimension<Point>>
		> = 0
	>
	integer dimension(Point&& point)
	{
		return 1;
	}

}

namespace Pastel
{

	template <Real_Ring_Concept_ Point>
	IntegerConstant<1> pointN(const Point* point);

	template <typename Real, std::size_t N>
	IntegerConstant<N> pointN(const std::array<Real, N>* point);

	namespace Point_
	{

		template <
			typename Type,
			typename Result = decltype(pointN((const Type*)nullptr))>
		struct Point_N_ 
		{
			using type = Result;
		};

	}

	template <typename Point>
	using Point_N_F = 
		Compute<IntegerConstant<Dynamic>, Point_::Point_N_, Point>;

	template <typename Point>
	using Point_N = typename Point_N_F<Point>::type;

}


namespace Pastel
{

	template <typename Point>
	using Point_Dimension = 
		Point_N<Point>;

	template <integer M, integer N>
	using EqualDimension_C = 
		BoolConstant<(M == N || M < 0 || N < 0)>;

	template <typename M, typename N>
	using EqualDimension = 
		EqualDimension_C<M::value, N::value>;

}

#endif
