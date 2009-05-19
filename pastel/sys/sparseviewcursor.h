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
				const Vector<N, integer>& step)
				: cursor_(cursor)
				, step_(step)
			{
			}

			void swap(ConstSparseViewCursor& that)
			{
				cursor_.swap(that.cursor_);
				step_.swap(that.step_);
			}

			template <int Index>
			void increment()
			{
				cursor_.move<Index>(step_[Index]);
			}

			void increment(integer index)
			{
				cursor_.move(index, step_[Index]);
			}

			template <int Index>
			void decrement()
			{
				cursor_.move<Index>(-step_[Index]);
			}

			void decrement(integer index)
			{
				cursor_.move(index, -step_[Index]);
			}

			template <int Index>
			void move(integer amount)
			{
				cursor_.move<Index>(amount * step_[Index]);
			}

			void move(integer index, integer amount)
			{
				cursor_.move(index, amount * step_[Index]);
			}

			void move(const Vector<N, integer>& amount)
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
			Vector<N, integer> step_;
		};

		template <int N, typename Cursor>
		class SparseViewCursor
			: public ConstSparseViewCursor<N, Cursor>
		{
		private:
			typedef ConstSparseViewCursor<N, Cursor> Base;
			using Base::cursor_;

		public:
			using typename Base::Element;
			using typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			typedef typename Cursor::Reference Reference;

			explicit SparseViewCursor(
				const Cursor& cursor,
				const Vector<N, integer>& step)
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
