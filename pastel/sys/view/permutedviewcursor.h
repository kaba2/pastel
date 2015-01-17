// Description: PermutedViewCursor class
// Documentation: concrete_views.txt

#ifndef PASTELSYS_PERMUTEDVIEWCURSOR_H
#define PASTELSYS_PERMUTEDVIEWCURSOR_H

#include "pastel/sys/view/permutedview.h"

namespace Pastel
{

	namespace PermutedView_
	{

		template <int N, typename ConstCursor>
		class ConstPermutedViewCursor
		{
		public:
			using Element = typename ConstCursor::Element;
			using ConstReference = typename ConstCursor::ConstReference;

			ConstPermutedViewCursor()
				: cursor_()
				, permutation_()
			{
			}

			explicit ConstPermutedViewCursor(
				const ConstCursor& cursor,
				const Tuple<integer, N>& permutation)
				: cursor_(cursor)
				, permutation_(permutation)
			{
			}

			void swap(ConstPermutedViewCursor& that)
			{
				cursor_.swap(that.cursor_);
				permutation_.swap(that.permutation_);
			}

			void increment(integer index)
			{
				cursor_.increment(permutation_[index]);
			}

			void decrement(integer index)
			{
				cursor_.decrement(permutation_[index]);
			}

			void move(integer index, integer amount)
			{
				cursor_.move(permutation_[index], amount);
			}

			void move(const Vector<integer, N>& amount)
			{
				cursor_.move(permute(amount, permutation_));
			}

			const Element* operator->() const
			{
				return cursor_.operator->();
			}

			ConstReference operator*() const
			{
				return *cursor_;
			}

		protected:
			ConstCursor cursor_;
			Tuple<integer, N> permutation_;
		};

		template <int N, typename Cursor>
		class PermutedViewCursor
			: public ConstPermutedViewCursor<N, Cursor>
		{
		private:
			using Base = ConstPermutedViewCursor<N, Cursor>;

			using Base::cursor_;

		public:
			using Element = typename Base::Element;
			using ConstReference = typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			using Reference = typename Cursor::Reference;

			friend class ConstPermutedViewCursor<N, Cursor>;

			PermutedViewCursor()
				: Base()
			{
			}

			explicit PermutedViewCursor(
				const Cursor& cursor,
				const Tuple<integer, N>& permutation)
				: Base(cursor, permutation)
			{
			}

			void swap(PermutedViewCursor& that)
			{
				Base::swap(that);
			}

			Element* operator->() const
			{
				return cursor_.operator->();
			}

			Reference operator*() const
			{
				return *cursor_;
			}
		};

	}

}

#endif
