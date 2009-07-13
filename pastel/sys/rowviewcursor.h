#ifndef PASTEL_ROWVIEWCURSOR_H
#define PASTEL_ROWVIEWCURSOR_H

#include "pastel/sys/rowview.h"

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	namespace Detail_RowView
	{

		template <typename ConstCursor>
		class ConstRowViewCursor
		{
		public:
			enum
			{
				Dimension = 1
			};

			typedef typename ConstCursor::Element Element;
			typedef typename ConstCursor::ConstReference ConstReference;

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

			template <int Index>
			typename boost::disable_if_c<Index == 0>::type increment();

			template <int Index>
			typename boost::enable_if_c<Index == 0>::type increment()
			{
				cursor_.increment(freeIndex_);
			}

			void increment(integer index)
			{
				PENSURE_OP(index, ==, 0);

				cursor_.increment(freeIndex_);
			}

			template <int Index>
			typename boost::disable_if_c<Index == 0>::type decrement();

			template <int Index>
			typename boost::enable_if_c<Index == 0>::type decrement()
			{
				cursor_.decrement(freeIndex_);
			}

			void decrement(integer index)
			{
				PENSURE_OP(index, ==, 0);

				cursor_.decrement(freeIndex_);
			}

			template <int Index>
			typename boost::disable_if_c<Index == 0>::type move(integer amount);

			template <int Index>
			typename boost::enable_if_c<Index == 0>::type move(integer amount)
			{
				cursor_.move(freeIndex_, amount);
			}

			void move(integer index, integer amount)
			{
				PENSURE_OP(index, ==, 0);
				cursor_.move(freeIndex_, amount);
			}

			void move(const Vector<1, integer>& amount)
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
			typedef ConstRowViewCursor<Cursor> Base;

			using Base::cursor_;

		public:
			using typename Base::Element;
			using typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			typedef typename Cursor::Reference Reference;

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
