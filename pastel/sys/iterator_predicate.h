// Description: Comparison predicates for iterators
// Documentation: predicates.txt

#ifndef PASTEL_ITERATOR_PREDICATE_H
#define PASTEL_ITERATOR_PREDICATE_H

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
	class Iterator_Predicate
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

	typedef Iterator_Predicate<LessThan> Iterator_Less;
	typedef Iterator_Predicate<LessEqual> Iterator_LessEqual;
	typedef Iterator_Predicate<GreaterThan> Iterator_Greater;
	typedef Iterator_Predicate<GreaterEqual> Iterator_GreaterEqual;
	typedef Iterator_Predicate<EqualTo> Iterator_EqualTo;
	typedef Iterator_Predicate<NotEqualTo> Iterator_NotEqualTo;

}

#endif
