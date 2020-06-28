// Description: TransformedViewCursor class
// Documentation: concrete_views.txt

#ifndef PASTELSYS_TRANSFORMEDVIEWCURSOR_H
#define PASTELSYS_TRANSFORMEDVIEWCURSOR_H

#include "pastel/sys/view/transformedview.h"

namespace Pastel
{

	namespace TransformedView_
	{

		template <int N, typename TransformFunctor, typename ConstCursor>
		class ConstTransformedViewCursor
		{
		public:
			using Element = typename TransformFunctor::Output_;
			using ConstReference = Element const&;

			explicit ConstTransformedViewCursor(
				const ConstCursor& cursor,
				const TransformFunctor& transform)
				: cursor_(cursor)
				, transform_(transform)
			{
			}

			void swap(ConstTransformedViewCursor& that)
			{
				cursor_.swap(that.cursor_);
				std::swap(transform_, that.transform_);
			}

			void increment(integer index)
			{
				cursor_.increment(index);
			}

			void decrement(integer index)
			{
				cursor_.decrement(index);
			}

			void move(Vector<integer, N> amount)
			{
				cursor_.move(amount);
			}

			/*
			const Element* operator->() const
			{
				return cursor_.operator->();
			}
			*/

			Element operator*() const
			{
				return transform_(*cursor_);
			}

		private:
			ConstCursor cursor_;
			TransformFunctor transform_;
		};

	}

}

#endif
