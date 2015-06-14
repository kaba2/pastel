// Description: Vector locator
// Documentation: locators.txt

#ifndef PASTELSYS_VECTOR_LOCATOR_H
#define PASTELSYS_VECTOR_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	template <typename Real_, integer N_ = Dynamic>
	class Vector_Locator
	{
	public:
		static constexpr integer N = N_;
		using Real = Real_;
		using Point = Vector<Real, N>;

		explicit Vector_Locator(integer n = N)
			: n_(n)
		{
			ENSURE(N == Dynamic || n == N);
			ENSURE_OP(n, >=, 0);
		}

		void swap(Vector_Locator& that)
		{
		}

		integer n() const
		{
			return n_;
		}

		const Real& operator()(const Point& point, integer i) const
		{
			return point[i];
		}

	private:
		integer n_;
	};

	using Vector1_Locator = Vector_Locator<real, 1>;
	using Vector2_Locator = Vector_Locator<real, 2>;
	using Vector3_Locator = Vector_Locator<real, 3>;
	using Vector4_Locator = Vector_Locator<real, 4>;
	using VectorD_Locator = Vector_Locator<real>;

	template <typename Real, integer N>
	class Default_Locator<const Vector<Real, N>&, void>
	{
	public:
		using Point = Vector<Real, N>;
		using Locator = Vector_Locator<Real, N>;

		Locator operator()(const Point& point) const
		{
			return Locator(point.n());
		}
	};

	template <typename Real_, integer N>
	auto vectorPoint(const Vector<Real_, N>& point)
		-> decltype(location(point, Vector_Locator<Real_, N>()))
	{
		// FIX: Replace with decltype(auto) when available.
		return location(
			point,
			Vector_Locator<Real_, N>(point.n()));
	}

}

#endif
