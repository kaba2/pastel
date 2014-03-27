// Description: Not predicate
// Documentation: predicates.txt

#ifndef PASTELSYS_NOT_PREDICATE_H
#define PASTELSYS_NOT_PREDICATE_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	template <typename Predicate>
	class Not_Predicate
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
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

#endif
