// Description: SliceViewCursor class
// Documentation: concrete_views.txt

#ifndef PASTELSYS_SLICEVIEWCURSOR_H
#define PASTELSYS_SLICEVIEWCURSOR_H

#include "pastel/sys/view/sliceview.h"

namespace Pastel
{

	namespace SliceView_
	{

		template <int N, typename ConstCursor>
		class ConstSliceViewCursor
		{
		public:
			static constexpr int Dimension = N - 1;

			using Element = typename ConstCursor::Element;
			using ConstReference = typename ConstCursor::ConstReference;

			ConstSliceViewCursor()
				: cursor_()
				, fixedIndex_(0)
			{
			}

			explicit ConstSliceViewCursor(
				const ConstCursor& cursor, integer fixedIndex)
				: cursor_(cursor)
				, fixedIndex_(fixedIndex)
			{
			}

			void swap(ConstSliceViewCursor& that)
			{
				cursor_.swap(that.cursor_);
				std::swap(fixedIndex_, that.fixedIndex_);
			}

			void increment(integer index)
			{
				if (index >= fixedIndex_)
				{
					cursor_.increment(index + 1);
				}
				else
				{
					cursor_.increment(index);
				}
			}

			void decrement(integer index)
			{
				if (index >= fixedIndex_)
				{
					cursor_.decrement(fixedIndex_ + 1);
				}
				else
				{
					cursor_.decrement(fixedIndex_);
				}
			}

			void move(integer index, integer amount)
			{
				if (index >= fixedIndex_)
				{
					cursor_.move(fixedIndex_ + 1, amount);
				}
				else
				{
					cursor_.move(fixedIndex_, amount);
				}
			}

			void move(const Vector<integer, SubN<N>>& amount)
			{
				cursor_.move(fixedIndex_, extend(amount, 0, fixedIndex_));
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
			integer fixedIndex_;
		};

		template <int N, typename Cursor>
		class SliceViewCursor
			: public ConstSliceViewCursor<N, Cursor>
		{
		private:
			using Base = ConstSliceViewCursor<N, Cursor>;

			using Base::cursor_;

		public:
			using Element = typename Base::Element;
			using ConstReference = typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			using Reference = typename Cursor::Reference;

			friend class ConstSliceViewCursor<N, Cursor>;

			SliceViewCursor()
				: Base()
			{
			}

			explicit SliceViewCursor(
				const Cursor& cursor,
				integer fixedIndex)
				: Base(cursor, fixedIndex)
			{
			}

			void swap(SliceViewCursor& that)
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
