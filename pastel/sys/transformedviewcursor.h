#ifndef PASTELSYS_TRANSFORMEDVIEWCURSOR_H
#define PASTELSYS_TRANSFORMEDVIEWCURSOR_H

#include "pastel/sys/transformedview.h"

namespace Pastel
{

	namespace Detail_TransformedView
	{

		template <int N, typename TransformFunctor, typename ConstCursor>
		class ConstTransformedViewCursor
		{
		public:
			typedef typename TransformFunctor::Output_ Element;
			typedef Element const& ConstReference;

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

			template <int Index>
			void increment()
			{
				cursor_.increment<Index>();
			}

			template <int Index>
			void decrement()
			{
				cursor_.decrement<Index>();
			}

			template <int Index>
			void move(integer amount)
			{
				cursor_.move<Index>(amount);
			}

			void move(Vector<N, integer> amount)
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
