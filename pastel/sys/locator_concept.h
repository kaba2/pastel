// Description: Locator concept

#ifndef PASTELSYS_LOCATOR_CONCEPT_H
#define PASTELSYS_LOCATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Locator_Concept
	{

		class Locator
		{
		public:
			//! Compile-time dimension.
			/*!
			If the dimension is dynamic,
			specify N = Dynamic.
			*/
			static PASTEL_CONSTEXPR integer N = UserDefinedInteger;

			//! The type of the point coordinates.
			using Real = UserDefinedType;
				
			//! The type of the points.
			using Point = UserDefinedType;

			//! Run-time dimension.
			/*!
			Returns:
			N, if N != Dynamic, and
			non-negative integer, otherwise.
			*/
			integer n() const;

			//! Swaps two locators.
			void swap(Locator& that);

			//! Returns the i:th coordinate of the given point.
			/*!
			Preconditions:
			0 <= i < n()
			*/
			Real operator()(const Point& point, integer i) const;
		};

	}

}

#include <type_traits>

namespace Pastel
{

	namespace Locator_
	{

		template <typename... LocatorSet>
		class Locator_Real;

		template <typename Locator, typename... LocatorSet>
		class Locator_Real<Locator, LocatorSet...>
		{
		public:
			using type = std::common_type_t<
				typename Locator_Real<Locator>::type, 
				typename Locator_Real<LocatorSet...>::type>;
		};

		template <typename Locator>
		class Locator_Real<Locator>
		{
		public:
			using type = typename Locator::Real;
		};

	}

	template <typename... LocatorSet>
	using Locator_Real = typename Locator_::Locator_Real<LocatorSet...>::type;

}

#endif
