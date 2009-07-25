// Description: Guid class
// Detail: Stores a unique 128-bit identifier

#ifndef PASTEL_GUID_H
#define PASTEL_GUID_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! Stores a unique 128-bit identifier.
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
