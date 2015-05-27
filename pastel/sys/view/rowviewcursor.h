// Description: RowViewCursor class
// Documentation: concrete_views.txt

#ifndef PASTELSYS_ROWVIEWCURSOR_H
#define PASTELSYS_ROWVIEWCURSOR_H

#include "pastel/sys/view/rowview.h"

namespace Pastel
{

	namespace RowView_
	{

		template <typename ConstCursor>
		class ConstRowViewCursor
		{
		public:
			static constexpr int Dimension = 1;

			using Element = typename ConstCursor::Element;
			using ConstReference = typename ConstCursor::ConstReference;

			ConstRowViewCursor()
				: cursor_()
				, freeIndex_(0)
			{
			}

			explicit ConstRowViewCursor(
				const ConstCursor& cursor, integer freeIndex)
				: cursor_(cursor)
				, freeIndex_(freeIndex)
			{
			}

			void swap(ConstRowViewCursor& that)
			{
				cursor_.swap(that.cursor_);
				std::swap(freeIndex_, that.freeIndex_);
			}

			void increment(integer index)
			{
				PENSURE_OP(index, ==, 0);

				cursor_.increment(freeIndex_);
			}

			void decrement(integer index)
			{
				PENSURE_OP(index, ==, 0);

				cursor_.decrement(freeIndex_);
			}

			void move(integer index, integer amount)
			{
				PENSURE_OP(index, ==, 0);
				cursor_.move(freeIndex_, amount);
			}

			void move(const Vector<integer, 1>& amount)
			{
				cursor_.move(freeIndex_, amount[0]);
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
			integer freeIndex_;
		};

		template <typename Cursor>
		class RowViewCursor
			: public ConstRowViewCursor<Cursor>
		{
		private:
			using Base = ConstRowViewCursor<Cursor>;

			using Base::cursor_;

		public:
			using Element = typename Base::Element;
			using ConstReference = typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			using Reference = typename Cursor::Reference;

			friend class ConstRowViewCursor<Cursor>;

			RowViewCursor()
				: Base()
			{
			}

			explicit RowViewCursor(
				const Cursor& cursor,
				integer freeIndex)
				: Base(cursor, freeIndex)
			{
			}

			void swap(RowViewCursor& that)
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
