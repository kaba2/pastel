// Description: Skip list iterator

#ifndef PASTEL_SKIPLIST_ITERATOR_H
#define PASTEL_SKIPLIST_ITERATOR_H

#include "pastel/sys/skiplist.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>

namespace Pastel
{

	namespace SkipList_
	{

		template <typename Key, typename Value_Class>
		struct Iterator_Value
		{
			typedef Value_Class type;
			static const bool UseValue = true;
		};

		template <typename Key>
		struct Iterator_Value<Key, Class<void>>
		{
			typedef const Key type;
			static const bool UseValue = false;
		};

		template <
			typename NodePtr,
			typename Key,
			typename Value_Class>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<NodePtr, Key, Value_Class>, NodePtr,
			typename Iterator_Value<Key, Value_Class>::type,
			boost::bidirectional_traversal_tag>
		{
		private:
			struct enabler {};

			typedef SkipList_::Data_Node<Key, Value_Class>
				Data_Node;

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
				const Iterator<That, Key, Value_Class>& that,
				typename boost::enable_if<
				boost::is_convertible<That, NodePtr>, 
				enabler>::type = enabler())
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			const Key& key() const
			{
				return ((Data_Node*)this->base())->key();
			}

			Value_Class& value()
			{
				return ((Data_Node*)this->base())->value();
			}

			const Value_Class& value() const
			{
				return ((Data_Node*)this->base())->value();
			}

		private:
			friend class boost::iterator_core_access;

			typedef Iterator_Value<Key, Value_Class>
				Dereference;

			typedef typename Dereference::type DereferenceType;

			struct KeyTag {};
			struct ValueTag {};

			typedef typename std::conditional<
				Dereference::UseValue, 
				ValueTag,
				KeyTag>::type DereferenceTag;

			const Key& dereference(KeyTag) const
			{
				return key();
			}

			Value_Class& dereference(ValueTag) const
			{
				return (Value_Class&)value();
			}

			DereferenceType& dereference() const
			{
				return dereference(DereferenceTag());
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