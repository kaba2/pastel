// Description: MirrorViewCursor class
// Documentation: view.txt

#ifndef PASTEL_MIRRORVIEWCURSOR_H
#define PASTEL_MIRRORVIEWCURSOR_H

#include "pastel/sys/mirrorview.h"

namespace Pastel
{

	namespace Detail_MirrorView
	{

		template <int N, int MirrorIndex, typename ConstCursor>
		class ConstMirrorViewCursor
		{
		public:
			typedef typename ConstCursor::Element Element;
			typedef typename ConstCursor::ConstReference ConstReference;

			explicit ConstMirrorViewCursor(
				const ConstCursor& cursor)
				: cursor_(cursor)
			{
			}

			void swap(ConstMirrorViewCursor& that)
			{
				cursor_.swap(that.cursor_);
			}

			void move(Vector<integer, N> amount)
			{
				amount[MirrorIndex] = -amount[MirrorIndex];
				cursor_.move(amount);
			}

			const Element* operator->() const
			{
				return cursor_.operator->();
			}

			ConstReference operator*() const
			{
				return *cursor_;
			}

		private:
			ConstCursor cursor_;
		};

		template <int N, int MirrorIndex, typename Cursor>
		class MirrorViewCursor
			: public ConstMirrorViewCursor<N, MirrorIndex, Cursor>
		{
		private:
			typedef ConstMirrorViewCursor<N, MirrorIndex, Cursor> Base;

		public:
			using typename Base::Element;
			using typename Base::ConstReference;

			typedef typename Cursor::Reference Reference;

			explicit MirrorViewCursor(
				const Cursor& cursor)
				: Base(cursor)
			{
			}

			void swap(MirrorViewCursor& that)
			{
				Base::swap(that);
			}

			Element* operator->() const
			{
				return (Element*)Base::operator->();
			}

			Reference operator*() const
			{
				return (Element&)Base::operator*();
			}
		};

	}

}

#endif
