// Description: Dereferenced predicates
// Documentation: derived_predicates.txt

#ifndef PASTELSYS_DEREFERENCED_PREDICATE_H
#define PASTELSYS_DEREFERENCED_PREDICATE_H

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
	class Dereferenced_Predicate
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return Predicate()(*left, *right);
		}
	};

	typedef Dereferenced_Predicate<LessThan> Dereferenced_LessThan;
	typedef Dereferenced_Predicate<LessEqual> Dereferenced_LessEqual;
	typedef Dereferenced_Predicate<GreaterThan> Dereferenced_GreaterThan;
	typedef Dereferenced_Predicate<GreaterEqual> Dereferenced_GreaterEqual;
	typedef Dereferenced_Predicate<EqualTo> Dereferenced_EqualTo;
	typedef Dereferenced_Predicate<NotEqualTo> Dereferenced_NotEqualTo;

	template <typename Predicate>
	Dereferenced_Predicate<Predicate> dereferencedPredicate(
		const Predicate& predicate)
	{
		return Dereferenced_Predicate<Predicate>();
	}

}

#endif
