// Description: Predicates for iterator-address's
// Documentation: predicates.txt

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

	typedef IteratorAddress_Predicate<LessThan> IteratorAddress_LessThan;
	typedef IteratorAddress_Predicate<LessEqual> IteratorAddress_LessEqual;
	typedef IteratorAddress_Predicate<GreaterThan> IteratorAddress_GreaterThan;
	typedef IteratorAddress_Predicate<GreaterEqual> IteratorAddress_GreaterEqual;
	typedef IteratorAddress_Predicate<EqualTo> IteratorAddress_EqualTo;
	typedef IteratorAddress_Predicate<NotEqualTo> IteratorAddress_NotEqualTo;

}

#endif
