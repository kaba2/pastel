// Description: SparseViewCursor class
// Documentation: views.txt

#ifndef PASTELSYS_SPARSEVIEWCURSOR_H
#define PASTELSYS_SPARSEVIEWCURSOR_H

#include "pastel/sys/sparseview.h"

namespace Pastel
{

	namespace SparseView_
	{

		template <int N, typename ConstCursor>
		class ConstSparseViewCursor
		{
		public:
			using Element = typename ConstCursor::Element;
			using ConstReference = typename ConstCursor::ConstReference;

			explicit ConstSparseViewCursor(
				const ConstCursor& cursor,
				const Vector<integer, N>& step)
				: cursor_(cursor)
				, step_(step)
			{
			}

			void swap(ConstSparseViewCursor& that)
			{
				cursor_.swap(that.cursor_);
				step_.swap(that.step_);
			}

			void increment(integer index)
			{
				cursor_.move(index, step_[index]);
			}

			void decrement(integer index)
			{
				cursor_.move(index, -step_[index]);
			}

			void move(integer index, integer amount)
			{
				cursor_.move(index, amount * step_[index]);
			}

			void move(const Vector<integer, N>& amount)
			{
				cursor_.move(amount * step_);
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
			Vector<integer, N> step_;
		};

		template <int N, typename Cursor>
		class SparseViewCursor
			: public ConstSparseViewCursor<N, Cursor>
		{
		private:
			using Base = ConstSparseViewCursor<N, Cursor>;
			using Base::cursor_;

		public:
			using Element = typename Base::Element;
			using ConstReference = typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			using Reference = typename Cursor::Reference;

			explicit SparseViewCursor(
				const Cursor& cursor,
				const Vector<integer, N>& step)
				: Base(cursor, step)
			{
			}

			void swap(SparseViewCursor& that)
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
