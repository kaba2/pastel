#ifndef PASTEL_REFERENCECOUNTED_H
#define PASTEL_REFERENCECOUNTED_H

#include "pastel/sys/syslibrary.h"

namespace Pastel
{

	class PASTELSYS ReferenceCounted
	{
	private:
		template <typename> friend class CountedPtr;
		template <typename> friend class WeakPtr;
	
	public:
		ReferenceCounted();
		virtual ~ReferenceCounted();

		// Note that a reference counter counts
		// the references to the _memory address_.
		// Thus assignment, copy construction and
		// swap must not influence the reference count.
		// In a way we should then prohibit them,
		// but in this special case it is easier
		// to define these functions but not
		// copy the reference counts. This
		// way the derived classes can use default
		// compiler implementations of copy constructors etc.

		// "Prohibited"
		ReferenceCounted(const ReferenceCounted&);
		// "Prohibited"
		ReferenceCounted& operator=(const ReferenceCounted&);

	private:
		mutable integer count_;
	};

}

#include "pastel/sys/referencecounted.hpp"

#endif
