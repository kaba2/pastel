#ifndef PASTELSYS_REFERENCECOUNTED_HPP
#define PASTELSYS_REFERENCECOUNTED_HPP

#include "pastel/sys/countedptr/referencecounted.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	inline ReferenceCounted::ReferenceCounted()
		: count_(0)
	{
	}

	inline ReferenceCounted::ReferenceCounted(
		const ReferenceCounted& that)
		: count_(0)
	{
		// See weakptr.h for rationale.
		unused(that);
	}

	inline ReferenceCounted& ReferenceCounted::operator=(
		const ReferenceCounted& that)
	{
		// See weakptr.h for rationale.
		unused(that);
		return *this;
	}

	inline ReferenceCounted::~ReferenceCounted()
	{
		ASSERT1(count_ == 0, count_);
	}

}

#endif
