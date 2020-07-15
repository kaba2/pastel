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

	template <typename T>
	concept Point_HasMemberDimension__ = requires(T t) {
		{t.pointDimension()} -> std::convertible_to<integer>;
	};

	template <typename T>
	concept Point_HasMemberDimension_ = 
		Point_HasMemberDimension__<RemoveCvRef<T>>;

	template <typename Type>
	using Point_HasMemberDimension = 
		std::bool_constant<Point_HasMemberDimension_<Type>>;

	template <Point_HasMemberDimension_ Point>
	decltype(auto) dimension(Point&& point)
	{
		return std::forward<Point>(point).pointDimension();
	}

}

namespace Pastel
{

	// Dimension by having member .size().

	template <typename T>
	concept Point_HasMemberSize__ = requires(T t) {
		{addConst(t).size()} -> std::convertible_to<integer>;
	};

	template <typename T>
	concept Point_HasMemberSize_ = 
		Point_HasMemberSize__<RemoveCvRef<T>>;

	template <typename Type>
	using Point_HasMemberSize = 
		std::bool_constant<Point_HasMemberSize_<Type>>;

	template <Point_HasMemberSize_ Point>
	requires (!Point_HasMemberDimension<Point>::value)
	integer dimension(const Point& point)
	{
		return point.size();
	}

}

namespace Pastel
{

	// Dimension by being a number.

	template <Real_Ring_Concept Point>
	requires
		(!Point_HasMemberSize<Point>::value &&
		!Point_HasMemberDimension<Point>::value)
	integer dimension(const Point& point)
	{
		return 1;
	}

}

namespace Pastel
{

	template <Real_Ring_Concept Point>
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
	using Point_N = typename Compute<IntegerConstant<Dynamic>, Point_::Point_N_, Point>::type;

}


namespace Pastel
{

	template <typename Point>
	using Point_Dimension = 
		Point_N<Point>;

}

#endif
