// Description: Tree iterator

#ifndef PASTELSYS_TREE_ITERATOR_H
#define PASTELSYS_TREE_ITERATOR_H

#include "pastel/sys/tree.h"
#include "pastel/sys/tree_node.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>

namespace Pastel
{

	namespace Tree_
	{

		template <
			typename NodePtr,
			typename Data_Class>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<NodePtr, Data_Class>, 
			NodePtr,
			Data_Class,
			boost::bidirectional_traversal_tag>
		{
		private:
			struct enabler {};

			typedef Tree_::Data_Node<Data_Class> Data_Node;

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
				const Iterator<That, Data_Class>& that,
				typename boost::enable_if<
				boost::is_convertible<That, NodePtr>, 
				enabler>::type = enabler())
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			bool empty() const
			{
				ASSERT(node());
				return node()->empty();
			}

			Iterator parent() const
			{
				return Iterator(node()->parent);
			}

			Iterator root() const
			{
				Iterator iter = *this;

				if (!iter.empty())
				{
					while(!iter.parent().empty())
					{
						iter = iter.parent();
					};
				}

				return iter;
			}

			Iterator left() const
			{
				return child(0);
			}

			Iterator leftMost() const
			{
				Iterator iter = *this;

				if (!iter.empty())
				{
					while(!iter.left().empty())
					{
						iter = iter.left();
					};
				}

				return iter;
			}

			Iterator right() const
			{
				return child(1);
			}

			Iterator rightMost() const
			{
				Iterator iter = *this;

				if (!iter.empty())
				{
					while(!iter.right().empty())
					{
						iter = iter.right();
					};
				}

				return iter;
			}

			Iterator child(integer index) const
			{
				ASSERT_OP(index, >=, 0);
				ASSERT_OP(index, <, 2);

				return Iterator(node()->child(index));
			}

			integer children() const
			{
				integer result = 0;
				for (integer i = 0;i < 2;++i)
				{
					if (!child(i).empty())
					{
						++result;
					}
				}
				
				return result;
			}

		private:
			friend class boost::iterator_core_access;

			NodePtr node() const
			{
				return this->base();
			}

			NodePtr next(integer forward)
			{
				ASSERT_OP(forward, >=, 0);
				ASSERT_OP(forward, <=, 1);

				const integer backward = !forward;

				NodePtr result;
				NodePtr child = node()->child(forward);
			
				if (child->empty())
				{
					result = node();

					NodePtr previous = 0;
					do
					{
						previous = result;
						result = previous->parent;
					}
					while(!result->empty() && 
						result->child(backward) != previous);
				}
				else
				{
					result = child;

					while(!result->child(backward)->empty())
					{
						result = result->child(backward);
					}
				}

				return result;
			}

			Data_Class& dereference() const
			{
				return (Data_Class&)*((Data_Node*)this->base());
			}

			void increment()
			{
				// If the cursor is in the sentinel node,
				// we should remain in that node.
				if (!node()->empty())
				{
					this->base_reference() = next(1);
				}
			}

			void decrement()
			{
				// If the cursor is in the sentinel node,
				// we should back out from it to the
				// 'maximum' node (the parent of the
				// sentinel node). Thus no special 
				// handling for the sentinel node here.

				this->base_reference() = next(0);
			}
		};

	}

}

#endif
