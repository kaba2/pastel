// Description: Default locator
// Documentation: locators.txt

#ifndef PASTELSYS_DEFAULT_LOCATOR_H
#define PASTELSYS_DEFAULT_LOCATOR_H

#include "pastel/sys/array_locator.h"
#include "pastel/sys/vector.h"

#include <type_traits>
#include <array>

namespace Pastel
{

	namespace Locator_
	{

		template <typename Point>
		class Default_Locator
		{
		};

		template <typename Real>
		class Default_Locator<Real*>
		{
		public:
			using type = Array_Locator<Real*>;
		};

		template <typename Real, integer N>
		class Default_Locator<std::array<Real, N>>
		{
		public:
			using type = Array_Locator<std::array<Real, N>>;
		};

		template <typename Real, integer N>
		class Default_Locator<Vector<Real, N>>
		{
		public:
			using type = Array_Locator<Vector<Real, N>>;
		};

	}

	template <typename Point>
	using Default_Locator =
		typename Locator_::Default_Locator<Point>::type;

}

#endif
