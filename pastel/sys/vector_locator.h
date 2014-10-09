// Description: Vector locator

#ifndef PASTELSYS_VECTOR_LOCATOR_H
#define PASTELSYS_VECTOR_LOCATOR_H

#include "pastel/sys/locator_concept.h"
#include "pastel/sys/point_concept.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class Vector_Locator
	{
	public:
		static PASTEL_CONSTEXPR int N = N_;
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

	template <typename Real, integer N>
	class Default_Locator<const Vector<Real, N>&>
	{
	public:
		Vector_Locator<Real, N> operator()(
			const Vector<Real, N>& point) const
		{
			return Vector_Locator<Real, N>(point.n());
		}
	};

	template <typename Real_, integer N>
	auto vectorPoint(const Vector<Real_, N>& point)
		-> decltype(location(point, Vector_Locator<Real_, N>()))
	{
		// FIX: Replace with decltype(auto) when available.
		return location(
			point,
			Vector_Locator<Real_, N>());
	}

}

#endif
