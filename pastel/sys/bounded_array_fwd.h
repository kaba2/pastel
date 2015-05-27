// Description: Bounded array types

#ifndef PASTELSYS_BOUNDED_ARRAY_FWD_H
#define PASTELSYS_BOUNDED_ARRAY_FWD_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename, template <typename> class>
	class BoundedArray;

	namespace BoundedArray_
	{

		template <typename>
		class Iterator;

	}

	template <typename Settings>
	class BoundedArray_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Element);
		static constexpr integer N = Settings::N;

		using Iterator = BoundedArray_::Iterator<Element>;
		using ConstIterator = BoundedArray_::Iterator<const Element>;
		using Range = boost::iterator_range<Iterator>;
		using ConstRange = boost::iterator_range<ConstIterator>;

		using iterator = Iterator;
		using const_iterator = ConstIterator;
	};

}

#endif
