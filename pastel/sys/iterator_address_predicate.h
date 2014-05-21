// Description: Predicates for iterator-address's
// Documentation: derived_predicates.txt

#ifndef PASTELSYS_ITERATORADDRESS_PREDICATE_H
#define PASTELSYS_ITERATORADDRESS_PREDICATE_H

#include "pastel/sys/predicate_concept.h"

#include "pastel/sys/equalto.h"
#include "pastel/sys/notequalto.h"
#include "pastel/sys/lessequal.h"
#include "pastel/sys/greaterequal.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/greaterthan.h"

namespace Pastel
{

	template <typename Predicate>
	class IteratorAddress_Predicate
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return Predicate()(&*left, &*right);
		}
	};

	using IteratorAddress_LessThan = IteratorAddress_Predicate<LessThan>;
	using IteratorAddress_LessEqual = IteratorAddress_Predicate<LessEqual>;
	using IteratorAddress_GreaterThan = IteratorAddress_Predicate<GreaterThan>;
	using IteratorAddress_GreaterEqual = IteratorAddress_Predicate<GreaterEqual>;
	using IteratorAddress_EqualTo = IteratorAddress_Predicate<EqualTo>;
	using IteratorAddress_NotEqualTo = IteratorAddress_Predicate<NotEqualTo>;

}

#endif
