// Description: Red-black forest iterator

#ifndef PASTELSYS_REDBLACKFOREST_ITERATOR_H
#define PASTELSYS_REDBLACKFOREST_ITERATOR_H

#include "pastel/sys/redblackforest_fwd.h"

namespace Pastel
{

	namespace RedBlackForest_
	{

		template <typename Iterator>
		class Continuous_Iterator
			: public boost::iterator_adaptor<
			Continuous_Iterator<Iterator>, 
			Iterator>
		{
		public:
			Continuous_Iterator()
				: Continuous_Iterator::iterator_adaptor_()
			{
			}

			Continuous_Iterator(const Iterator& that)
				: Continuous_Iterator::iterator_adaptor_(that)
			{
			}

			template <
				typename That_Iterator,
				typename = PASTEL_ENABLE_IF((boost::is_convertible<That_Iterator, Iterator>), void)>
			Continuous_Iterator(
				const Continuous_Iterator<That_Iterator>& that)
				: Continuous_Iterator::iterator_adaptor_(that.base())
			{
			}

			bool isSentinel() const
			{
				return base().isSentinel();
			}

			bool isGlobalSentinel() const
			{
				return base().right() == base();
			}

		private:
			friend class boost::iterator_core_access;

			void increment()
			{
				Iterator& iter = base_reference();
				++iter;
				if (isSentinel() && !isGlobalSentinel())
				{
					iter = std::next(iter.sentinelData().tree)->begin().base();
				}
			}

			void decrement()
			{
				Iterator& iter = base_reference();
				--iter;
				if (isSentinel())
				{
					iter = std::prev(iter.sentinelData().tree)->last().base();
				}
			}
		};
	
	}

}

#endif
