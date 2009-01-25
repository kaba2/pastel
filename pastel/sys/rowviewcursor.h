#ifndef PASTELSYS_ROWVIEWCURSOR_H
#define PASTELSYS_ROWVIEWCURSOR_H

#include "pastel/sys/rowview.h"

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	namespace Detail_RowView
	{

		template <int FreeIndex, typename ConstCursor>
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
			{
			}

			explicit ConstRowViewCursor(
				const ConstCursor& cursor)
				: cursor_(cursor)
			{
			}

			void swap(ConstRowViewCursor& that)
			{
				cursor_.swap(that.cursor_);
			}

			template <int Index>
			typename boost::disable_if_c<Index == 0>::type increment();

			template <int Index>
			typename boost::enable_if_c<Index == 0>::type increment()
			{
				cursor_.increment<FreeIndex>();
			}

			void increment(integer index)
			{
				PENSURE1(index == 0, index);

				cursor_.increment<FreeIndex>();
			}

			template <int Index>
			typename boost::disable_if_c<Index == 0>::type decrement();

			template <int Index>
			typename boost::enable_if_c<Index == 0>::type decrement()
			{
				cursor_.decrement<FreeIndex>();
			}

			void decrement(integer index)
			{
				PENSURE1(index == 0, index);

				cursor_.decrement<FreeIndex>();
			}

			template <int Index>
			typename boost::disable_if_c<Index == 0>::type move(integer amount);

			template <int Index>
			typename boost::enable_if_c<Index == 0>::type move(integer amount)
			{
				cursor_.move<FreeIndex>(amount);
			}

			void move(integer index, integer amount)
			{
				PENSURE(index == 0);
				cursor_.move<FreeIndex>(amount);
			}

			void move(const Vector<1, integer>& amount)
			{
				cursor_.move<FreeIndex>(amount[0]);
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
		};

		template <int FreeIndex, typename Cursor>
		class RowViewCursor
			: public ConstRowViewCursor<FreeIndex, Cursor>
		{
		private:
			typedef ConstRowViewCursor<FreeIndex, Cursor> Base;

			using Base::cursor_;

		public:
			using typename Base::Element;
			using typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			typedef typename Cursor::Reference Reference;

			friend class ConstRowViewCursor<FreeIndex, Cursor>;

			RowViewCursor()
				: Base()
			{
			}

			explicit RowViewCursor(
				const Cursor& cursor)
				: Base(cursor)
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
