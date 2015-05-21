// Description: Bounded array iterator

#ifndef PASTELSYS_BOUNDED_ARRAY_ITERATOR_H
#define PASTELSYS_BOUNDED_ARRAY_ITERATOR_H

#include "pastel/sys/bounded_array_fwd.h"
#include "pastel/sys/hashing.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>

namespace Pastel
{

	namespace BoundedArray_
	{

		template <typename Element>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<Element>, 
			Element*,
			boost::use_default,
			boost::random_access_traversal_tag>
		{
		public:
			Iterator()
				: Iterator::iterator_adaptor_(0) 
			{
			}

			template <
				typename That_Element,
				Requires<std::is_convertible<That_Element, Element>> = 0>
			Iterator(const Iterator<That_Element>& that)
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			Iterator& operator=(const Iterator& that)
			{
				this->base_reference() = that.base();
				return *this;
			}

		private:
			template <typename, template <typename> class>
			friend class Pastel::BoundedArray;

			friend class boost::iterator_core_access;

			explicit Iterator(Element* that)
				: Iterator::iterator_adaptor_(that) 
			{
			}
		};
	
	}

}

namespace std
{

	template <typename Element>
	struct hash<Pastel::BoundedArray_::Iterator<Element>>
	{
		Pastel::hash_integer operator()(
			const Pastel::BoundedArray_::Iterator<Element>& that) const
		{
			return Pastel::computeHash<Element*>(that.base());
		}
	};

}

#endif
