// Description: Skip list iterator

#ifndef PASTEL_SKIPLIST_ITERATOR_H
#define PASTEL_SKIPLIST_ITERATOR_H

#include "pastel/sys/skiplist.h"

#include <boost/iterator/iterator_adaptor.hpp>

namespace Pastel
{

	namespace SkipList_
	{

		template <
			typename NodePtr, 
			typename Type_Class>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<NodePtr, Type_Class>, NodePtr,
			Type_Class,
			boost::bidirectional_traversal_tag>
		{
		private:
			struct enabler {};

		public:
			Iterator()
				: Iterator::iterator_adaptor_(0) 
			{
			}

			Iterator(NodePtr that)
				: Iterator::iterator_adaptor_(that) 
			{
			}

			template <typename That>
			Iterator(
				const Iterator<That, Type_Class>& that,
				typename boost::enable_if<
				boost::is_convertible<That, NodePtr>, 
				enabler>::type = enabler())
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

		private:
			friend class boost::iterator_core_access;

			Type_Class& dereference() const
			{
				return *(SkipList_::Data_Node<Type_Class>*)(
					this->base());
			}

			void increment() 
			{ 
				this->base_reference() = 
					this->base()->link_[0].next[1];
			}

			void decrement() 
			{ 
				// For some reason
				// this->base()->link<false>(0)
				// does not work. Perhaps a bug
				// in Clang?

				this->base_reference() = 
					this->base()->link_[0].next[0];
			}
		};

	}

}

#endif
