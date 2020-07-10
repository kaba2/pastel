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

		template <typename Key, typename Value>
		using DereferenceType_ = std::conditional_t<
			std::is_same_v<Value, Empty>, const Key, Value>;

		template <
			typename NodePtr,
			typename Key,
			typename Value>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<NodePtr, Key, Value>, 
			NodePtr,
			DereferenceType_<Key, Value>,
			boost::bidirectional_traversal_tag>
		{
		private:
			typedef SkipList_::Data_Node<Key, Value>
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
			requires std::is_convertible_v<That, NodePtr>
			Iterator(const Iterator<That, Key, Value>& that)
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			const Key& key() const
			{
				return ((Data_Node*)this->base())->key();
			}

			Value& value()
			{
				return ((Data_Node*)this->base())->value();
			}

			const Value& value() const
			{
				return ((const Data_Node*)this->base())->value();
			}

			integer equivalents() const
			{
				return this->base()->keys();
			}

			integer height() const
			{
				return this->base()->height();
			}

			Iterator next(integer level, bool direction) const
			{
				NodePtr node = this->base();
				return Iterator(node->link(level)[direction]);
			}

			Iterator next(integer level) const
			{
				Node* node = (Node*)this->base();
				return Iterator(node->link(level)[1]);
			}

			Iterator prev(integer level) const
			{
				Node* node = (Node*)this->base();
				return Iterator(node->link(level)[0]);
			}

		private:
			friend class boost::iterator_core_access;

			using DereferenceType = DereferenceType_<Key, Value>;

			struct KeyTag {};
			struct ValueTag {};

			using DereferenceTag = std::conditional_t<
				!std::is_same_v<Value, Empty>,
				ValueTag,
				KeyTag>;

			const Key& dereference(KeyTag) const
			{
				return key();
			}

			Value& dereference(ValueTag) const
			{
				return (Value&)value();
			}

			DereferenceType& dereference() const
			{
				return dereference(DereferenceTag());
			}

			void increment() 
			{ 
				this->base_reference() = 
					this->base()->link(0)[1];
			}

			void decrement() 
			{ 
				this->base_reference() = 
					this->base()->link(0)[0];
			}
		};

	}

}

#endif
