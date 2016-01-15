// Description: Not predicate
// Documentation: derived_predicates.txt

#ifndef PASTELSYS_NOT_PREDICATE_H
#define PASTELSYS_NOT_PREDICATE_H

#include "pastel/sys/predicate/predicate_concept.h"

#ifdef _MSC_VER
	// Visual Studio 2015 Update 1 has a bug which gives
	// a spurious warning here.
	#pragma warning(push)
	#pragma warning(disable: 4552)
	#pragma warning(disable: 4553)
#endif

namespace Pastel
{

	template <typename Predicate>
	class Not_Predicate
	{
	public:
		template <
			typename Left, 
			typename Right>
		decltype(auto) operator()(
			const Left& left, 
			const Right& right) const
		{
			return !Predicate()(left, right);
		}
	};

	template <typename Predicate>
	Not_Predicate<Predicate> notPredicate(const Predicate& predicate)
	{
		return Not_Predicate<Predicate>();
	}

}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif
