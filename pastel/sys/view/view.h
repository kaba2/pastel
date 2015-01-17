// Description: View class
// Documentation: views.txt

#ifndef PASTELSYS_VIEW_H
#define PASTELSYS_VIEW_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	// ConstView

	namespace View_
	{

		template <int N, typename Type, typename Contained_ConstView>
		class ConstViewBase
		{
		public:
			static PASTEL_CONSTEXPR int Dimension = N;

			using ConstCursor = typename Contained_ConstView::ConstCursor;
			using Element = typename Contained_ConstView::Element;
			using ConstReference = typename Contained_ConstView::ConstReference;

			ConstViewBase()
				: view_()
			{
			}

			explicit ConstViewBase(const Contained_ConstView& view)
				: view_(view)
			{
			}

			ConstViewBase(const ConstViewBase& that)
				: view_(that.view_)
			{
			}

			const Vector<integer, N>& extent() const
			{
				return view_.extent();
			}

			const Contained_ConstView& contained() const
			{
				return view_;
			}

			ConstReference operator()(const Vector<integer, N>& position) const
			{
				return *view_.constCursor(position);
			}

			ConstCursor constCursor(const Vector<integer, N>& position) const
			{
				return view_.constCursor(position);
			}

		private:
			Contained_ConstView view_;
		};

	}

	template <int N, typename Type, typename Contained_ConstView>
	class ConstView
		: public View_::ConstViewBase<N, Type, Contained_ConstView>
	{
	private:
		using Base = View_::ConstViewBase<N, Type, Contained_ConstView>;

	public:
		using ConstCursor = typename Contained_ConstView::ConstCursor;
		using Element = typename Contained_ConstView::Element;
		using ConstReference = typename Contained_ConstView::ConstReference;

		ConstView()
			: Base()
		{
		}


		explicit ConstView(const Contained_ConstView& view)
			: Base(view)
		{
		}

		ConstView(const ConstView& that)
			: Base(that.contained())
		{
		}
	};

	template <typename Type, typename Contained_ConstView>
	class ConstView<1, Type, Contained_ConstView>
		: public View_::ConstViewBase<1, Type, Contained_ConstView>
	{
	private:
		using Base = View_::ConstViewBase<1, Type, Contained_ConstView>;

	public:
		using ConstCursor = typename Contained_ConstView::ConstCursor;
		using Element = typename Contained_ConstView::Element;
		using ConstReference = typename Contained_ConstView::ConstReference;

		using Base::operator();
		using Base::constCursor;

		ConstView()
			: Base()
		{
		}

		explicit ConstView(const Contained_ConstView& view)
			: Base(view)
		{
		}

		ConstView(const ConstView& that)
			: Base(that)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		ConstReference operator()(integer x) const
		{
			return Base::operator()(Vector<integer, 1>(x));
		}

		ConstCursor constCursor(integer x) const
		{
			return Base::constCursor(Vector<integer, 1>(x));
		}
	};

	template <typename Type, typename Contained_ConstView>
	class ConstView<2, Type, Contained_ConstView>
		: public View_::ConstViewBase<2, Type, Contained_ConstView>
	{
	private:
		using Base = View_::ConstViewBase<2, Type, Contained_ConstView>;

	public:
		using ConstCursor = typename Contained_ConstView::ConstCursor;
		using Element = typename Contained_ConstView::Element;
		using ConstReference = typename Contained_ConstView::ConstReference;

		using Base::operator();
		using Base::constCursor;

		ConstView()
			: Base()
		{
		}

		explicit ConstView(const Contained_ConstView& view)
			: Base(view)
		{
		}

		ConstView(const ConstView& that)
			: Base(that)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		ConstReference operator()(integer x, integer y) const
		{
			return Base::operator()(Vector<integer, 2>(x, y));
		}

		ConstCursor constCursor(integer x, integer y) const
		{
			return Base::constCursor(Vector<integer, 2>(x, y));
		}
	};

	template <typename Type, typename Contained_ConstView>
	class ConstView<3, Type, Contained_ConstView>
		: public View_::ConstViewBase<3, Type, Contained_ConstView>
	{
	private:
		using Base = View_::ConstViewBase<3, Type, Contained_ConstView>;

	public:
		using ConstCursor = typename Contained_ConstView::ConstCursor;
		using Element = typename Contained_ConstView::Element;
		using ConstReference = typename Contained_ConstView::ConstReference;

		using Base::operator();
		using Base::constCursor;

		ConstView()
			: Base()
		{
		}

		explicit ConstView(const Contained_ConstView& view)
			: Base(view)
		{
		}

		ConstView(const ConstView& that)
			: Base(that)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		integer depth() const
		{
			return Base::extent()[2];
		}

		ConstReference operator()(integer x, integer y, integer z) const
		{
			return Base::operator()(Vector<integer, 3>(x, y, z));
		}

		ConstCursor constCursor(integer x, integer y, integer z) const
		{
			return Base::constCursor(Vector<integer, 3>(x, y, z));
		}
	};

	template <typename Contained_ConstView>
	ConstView<Contained_ConstView::Dimension, typename Contained_ConstView::Element, Contained_ConstView>
		wrapConstView(const Contained_ConstView& view)
	{
		return ConstView<Contained_ConstView::Dimension, typename Contained_ConstView::Element, Contained_ConstView>(view);
	}

	// View

	namespace View_
	{

		template <int N, typename Type, typename Contained_View>
		class ViewBase
			: public ConstView<N, Type, Contained_View>
		{
		private:
			using Base = ConstView<N, Type, Contained_View>;

		public:
			using Cursor = typename Contained_View::Cursor;
			using Reference = typename Contained_View::Reference;

			// Needed because base class is dependent on
			// template parameters.
			//using Base::Dimension;
			using Element = typename Base::Element;
			using ConstCursor = typename Base::ConstCursor;
			// We do not want to bring the ConstView's operator() into scope...
			//using Base::operator();
			using Base::extent;
			using Base::contained;
			using Base::constCursor;

			ViewBase()
				: Base()
			{
			}

			explicit ViewBase(const Contained_View& view)
				: Base(view)
			{
			}

			ViewBase(const ViewBase& that)
				: Base(that)
			{
			}

			Reference operator()(const Vector<integer, N>& position) const
			{
				return *contained().cursor(position);
			}

			Cursor cursor(const Vector<integer, N>& position) const
			{
				return contained().cursor(position);
			}
		};

	}

	template <int N, typename Type, typename Contained_View>
	class View
		: public View_::ViewBase<N, Type, Contained_View>
	{
	private:
		using Base = View_::ViewBase<N, Type, Contained_View>;

	public:
		// Needed because base class is dependent on
		// template parameters.
		using Element = typename Base::Element;
		using ConstReference = typename Base::ConstReference;
		using Reference = typename Base::Reference;
		using ConstCursor = typename Base::ConstCursor;
		using Cursor = typename Base::Cursor;
		using Base::operator();
		using Base::extent;
		using Base::contained;
		using Base::cursor;
		using Base::constCursor;

		View()
			: Base()
		{
		}

		explicit View(const Contained_View& view)
			: Base(view)
		{
		}

		View(const View& that)
			: Base(that)
		{
		}
	};

	template <typename Type, typename Contained_View>
	class View<1, Type, Contained_View>
		: public View_::ViewBase<1, Type, Contained_View>
	{
	private:
		using Base = View_::ViewBase<1, Type, Contained_View>;

	public:
		// Needed because base class is dependent on
		// template parameters.
		using Element = typename Base::Element;
		using ConstReference = typename Base::ConstReference;
		using Reference = typename Base::Reference;
		using ConstCursor = typename Base::ConstCursor;
		using Cursor = typename Base::Cursor;
		using Base::operator();
		using Base::extent;
		using Base::contained;
		using Base::cursor;
		using Base::constCursor;

		View()
			: Base()
		{
		}

		explicit View(const Contained_View& view)
			: Base(view)
		{
		}

		View(const View& that)
			: Base(that)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		Reference operator()(integer x) const
		{
			return *contained().cursor(Vector<integer, 1>(x));
		}

		Cursor cursor(integer x) const
		{
			return contained().cursor(Vector<integer, 1>(x));
		}
	};

	template <typename Type, typename Contained_View>
	class View<2, Type, Contained_View>
		: public View_::ViewBase<2, Type, Contained_View>
	{
	private:
		using Base = View_::ViewBase<2, Type, Contained_View>;

	public:
		// Needed because base class is dependent on
		// template parameters.
		using Element = typename Base::Element;
		using ConstReference = typename Base::ConstReference;
		using Reference = typename Base::Reference;
		using ConstCursor = typename Base::ConstCursor;
		using Cursor = typename Base::Cursor;
		using Base::operator();
		using Base::extent;
		using Base::contained;
		using Base::cursor;
		using Base::constCursor;

		View()
			: Base()
		{
		}

		explicit View(const Contained_View& view)
			: Base(view)
		{
		}

		View(const View& that)
			: Base(that)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		Reference operator()(integer x, integer y) const
		{
			return *contained().cursor(Vector<integer, 2>(x, y));
		}

		Cursor cursor(integer x, integer y) const
		{
			return contained().cursor(Vector<integer, 2>(x, y));
		}
	};

	template <typename Type, typename Contained_View>
	class View<3, Type, Contained_View>
		: public View_::ViewBase<3, Type, Contained_View>
	{
	private:
		using Base = View_::ViewBase<3, Type, Contained_View>;

	public:
		// Needed because base class is dependent on
		// template parameters.
		using Element = typename Base::Element;
		using ConstReference = typename Base::ConstReference;
		using Reference = typename Base::Reference;
		using ConstCursor = typename Base::ConstCursor;
		using Cursor = typename Base::Cursor;
		using Base::operator();
		using Base::extent;
		using Base::contained;
		using Base::cursor;
		using Base::constCursor;

		View()
			: Base()
		{
		}

		explicit View(const Contained_View& view)
			: Base(view)
		{
		}

		View(const View& that)
			: Base(that)
		{
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		integer depth() const
		{
			return Base::extent()[2];
		}

		Reference operator()(integer x, integer y, integer z) const
		{
			return *contained().cursor(Vector<integer, 3>(x, y, z));
		}

		Cursor cursor(integer x, integer y, integer z) const
		{
			return contained().cursor(Vector<integer, 3>(x, y, z));
		}
	};

	template <typename Contained_View>
	View<Contained_View::Dimension, typename Contained_View::Element, Contained_View>
		wrapView(const Contained_View& view)
	{
		return View<Contained_View::Dimension, typename Contained_View::Element, Contained_View>(view);
	}

}

#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/view/view_visit.h"

#endif
