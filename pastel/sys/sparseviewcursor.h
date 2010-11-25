// Description: SparseViewCursor class
// Documentation: view.txt

#ifndef PASTEL_SPARSEVIEWCURSOR_H
#define PASTEL_SPARSEVIEWCURSOR_H

#include "pastel/sys/sparseview.h"

namespace Pastel
{

	namespace Detail_SparseView
	{

		template <int N, typename ConstCursor>
		class ConstSparseViewCursor
		{
		public:
			typedef typename ConstCursor::Element Element;
			typedef typename ConstCursor::ConstReference ConstReference;

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
			typedef ConstSparseViewCursor<N, Cursor> Base;
			using Base::cursor_;

		public:
			typedef typename Base::Element Element;
			typedef typename Base::ConstReference ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			typedef typename Cursor::Reference Reference;

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
