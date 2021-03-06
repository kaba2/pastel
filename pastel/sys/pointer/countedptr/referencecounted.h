// Description: A base class for CountedPtr reference counted objects
// Documentation: countedptr.txt

#ifndef PASTELSYS_REFERENCECOUNTED_H
#define PASTELSYS_REFERENCECOUNTED_H

namespace Pastel
{

	//! A base class for CountedPtr reference counted objects.
	/*!
	Any class wishing to use CountedPtr for reference counting
	must derive from ReferenceCounted. This base class
	simply includes a counter that the CountedPtr implementation
	increases and decreases.
	*/
	class ReferenceCounted
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

#include "pastel/sys/pointer/countedptr/referencecounted.hpp"

#endif
