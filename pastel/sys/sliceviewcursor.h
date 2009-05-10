#ifndef PASTELSYS_SLICEVIEWCURSOR_H
#define PASTELSYS_SLICEVIEWCURSOR_H

#include "pastel/sys/sliceview.h"

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	namespace Detail_SliceView
	{

		template <int N, typename ConstCursor>
		class ConstSliceViewCursor
		{
		public:
			enum
			{
				Dimension = N - 1
			};

			typedef typename ConstCursor::Element Element;
			typedef typename ConstCursor::ConstReference ConstReference;

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

			template <int Index>
			void increment()
			{
				if (Index >= fixedIndex_)
				{
					cursor_.increment(fixedIndex_ + 1);
				}
				else
				{
					cursor_.increment(fixedIndex_);
				}
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

			template <int Index>
			void decrement()
			{
				if (Index >= fixedIndex_)
				{
					cursor_.decrement(fixedIndex_ + 1);
				}
				else
				{
					cursor_.decrement(fixedIndex_);
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

			template <int Index>
			void move(integer amount)
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

			void move(const Vector<PASTEL_ADD_N(N, -1), integer>& amount)
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
			typedef ConstSliceViewCursor<N, Cursor> Base;

			using Base::cursor_;

		public:
			using typename Base::Element;
			using typename Base::ConstReference;
			using Base::increment;
			using Base::decrement;
			using Base::move;

			typedef typename Cursor::Reference Reference;

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
