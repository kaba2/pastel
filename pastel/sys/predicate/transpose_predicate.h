// Description: Transpose predicate
// Documentation: derived_predicates.txt

#ifndef PASTELSYS_TRANSPOSE_PREDICATE_H
#define PASTELSYS_TRANSPOSE_PREDICATE_H

#include "pastel/sys/predicate/predicate_concept.h"

namespace Pastel
{

	template <typename Predicate>
	class Transpose_Predicate
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return Predicate()(right, left);
		}
	};

	template <typename Predicate>
	Transpose_Predicate<Predicate> transposePredicate(const Predicate& predicate)
	{
		return Transpose_Predicate<Predicate>();
	}

}

#endif
