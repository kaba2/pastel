// Description: Array locator
// Documentation: locators.txt

#ifndef PASTELSYS_ARRAY_LOCATOR_H
#define PASTELSYS_ARRAY_LOCATOR_H

#include "pastel/sys/locator_concept.h"
#include "pastel/sys/vector.h"

#include <type_traits>
#include <array>

namespace Pastel
{

	//! The generic case.
	template <typename Point_>
	class Array_Locator
	{
	public:
		using Point = Point_;
		using Return = decltype(std::declval<const Point>()[0]);
		using Real = 
			std::remove_cv_t<std::remove_reference_t<Return>>;
		static PASTEL_CONSTEXPR integer N = Dynamic;

		explicit Array_Locator(integer n = N)
			: n_(n)
		{
			ENSURE(N == Dynamic || n == N);
			ENSURE_OP(n, >=, 0);
		}

		void swap(Array_Locator& that)
		{
			std::swap(n_, that.n_);
		}

		integer n() const
		{
			return n_;
		}

		Return operator()(const Point& point, integer i) const
		{
			return point[i];
		}

	private:
		integer n_;
	};

	template <typename Real_>
	class Array_Locator<Real_*>
	{
	public:
		using Real = Real_;
		using Point = const Real*;
		using Return = decltype(std::declval<Point>()[0]);
		static PASTEL_CONSTEXPR integer N = Dynamic;

		explicit Array_Locator(integer n = N)
			: n_(n)
		{
			ENSURE(N == Dynamic || n == N);
			ENSURE_OP(n, >=, 0);
		}

		void swap(Array_Locator& that)
		{
			std::swap(n_, that.n_);
		}

		integer n() const
		{
			return n_;
		}

		Return operator()(const Point& point, integer i) const
		{
			return point[i];
		}

	private:
		integer n_;
	};

	template <typename Real_, integer N_>
	class Array_Locator<std::array<Real_, N_>>
	{
	public:
		using Real = Real_;
		static PASTEL_CONSTEXPR integer N = N_;

		using Point = std::array<Real, N>;
		using Return = decltype(std::declval<const Point>()[0]);

		explicit Array_Locator(integer n = N)
		{
			ENSURE_OP(n, ==, N);
			ENSURE_OP(n, >=, 0);
		}

		void swap(Array_Locator& that)
		{
		}

		integer n() const
		{
			return N;
		}

		Return operator()(const Point& point, integer i) const
		{
			return point[i];
		}
	};

	template <typename Real_, integer N_>
	class Array_Locator<Vector<Real_, N_>>
	{
	public:
		using Real = Real_;
		static PASTEL_CONSTEXPR integer N = N_;

		using Point = Vector<Real, N>;
		using Return = decltype(std::declval<const Point>()[0]);

		explicit Array_Locator(integer n = N)
		: n_(n)
		{
			ENSURE(N == Dynamic || n == N);
			ENSURE_OP(n, >=, 0);
		}

		void swap(Array_Locator& that)
		{
		}

		integer n() const
		{
			return n_;
		}

		Return operator()(const Point& point, integer i) const
		{
			return point[i];
		}

	private:
		integer n_;
	};

}

#endif
