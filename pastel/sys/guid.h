/*!
\file
\brief A class for a unique 128-bit identifier.
*/

#ifndef PASTEL_GUID_H
#define PASTEL_GUID_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! Manages a unique 128-bit identifier.

	/*!
	This class stores a 128-bit identifier along with some
	modifying and comparison operations . It is often the case
	that some things may need to be given truly global identifiers,
	from which they can be safely identified later. In this case
	a need for an identifier allocation policy also rises.
	If this identifier allocation process is distributed,
	it is impossible without coordination to avoid duplicate
	identifiers. This is
	solved by using a statistical approach where a long
	identifier is gathered together from time, place and
	environment dependent variables. For example, a network
	mac address should itself be a universally unique identifier.
	If we further pack with this information the current date
	and time and some random number we are pretty sure to never
	have any colliding identifiers.
	*/

	class PASTELSYS Guid
		: boost::less_than_comparable<
		Guid
		, boost::equality_comparable<
		Guid
		> >
	{
	public:
		//! Constructs a zero guid.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Guid();

		//! Constructs the guid with the given values.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Guid(uint32 a, uint32 b, uint32 c, uint32 d);

		//! Constructs with another guid.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Guid(const Guid& that);

		//! Swaps two guids.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(Guid& that);

		//! Compares two guids.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		bool operator==(const Guid& that) const;

		//! Compares two guids.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		bool operator<(const Guid& that) const;

		//! Sets the identifier to zero.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void set(uint32 a, uint32 b, uint32 c, uint32 d);

	private:
		uint32 a_;
		uint32 b_;
		uint32 c_;
		uint32 d_;
	};

	void swap(Guid& left, Guid& right);

}

#include "pastel/sys/guid.hpp"

#endif
