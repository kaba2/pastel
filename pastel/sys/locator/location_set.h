// Description: A set with a locator; a point-set.
// Documentation: pointset.txt

#ifndef PASTELSYS_LOCATION_SET_H
#define PASTELSYS_LOCATION_SET_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/set/transformed_set.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/locator/location.h"

#include <range/v3/all.hpp>

namespace Pastel
{

	//! A set with a locator; a point-set.
	template <
		Set_Concept Set,
		Locator_Concept Locator,
		typename Base>
	class LocationSet
	: public Base
	{
	public:
		using Index = Set_Index<std::add_const_t<Set>>;

		LocationSet(
			const Set& set,
			const Locator& locator,
			const Base& base)
			: Base(base)
			, set_(set)
			, locator_(locator)
		{
		}

		const Set& pointSetSet() const
		{
			return set_;
		}

		const Locator& pointSetLocator() const
		{
			return locator_;
		}

	private:
		const Set& set_;
		Locator locator_;
	};

	template <typename Type>
	struct IsLocationSet
		: std::false_type
	{};

	template <
		typename Set, 
		typename Locator,
		typename Base>
	struct IsLocationSet<LocationSet<Set, Locator, Base>>
		: std::true_type
	{};


	//! Constructs a location-set from a point-set and a locator.
	template <
		Set_Concept Set, 
		Locator_Concept Locator
	>
	decltype(auto) locationSet(
		const Set& set,
		const Locator& locator)
	{
		auto transformedSet_ = transformedSet(
			set,
			[=](auto&& element){return location(element, locator);});

		using Base = decltype(transformedSet_);

		return LocationSet<Set, Locator, Base>(set, locator, transformedSet_);
	}

}

#endif
