// Description: MirrorViewCursor class
// Documentation: concrete_views.txt

#ifndef PASTELSYS_MIRRORVIEWCURSOR_H
#define PASTELSYS_MIRRORVIEWCURSOR_H

#include "pastel/sys/view/mirrorview.h"

namespace Pastel
{

	namespace MirrorView_
	{

		template <int N, int MirrorIndex, typename ConstCursor>
		class ConstMirrorViewCursor
		{
		public:
			using Element = typename ConstCursor::Element;
			using ConstReference = typename ConstCursor::ConstReference;

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
			using Base = ConstMirrorViewCursor<N, MirrorIndex, Cursor>;

		public:
			using Element = typename Base::Element;
			using ConstReference = typename Base::ConstReference;

			using Reference = typename Cursor::Reference;

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
