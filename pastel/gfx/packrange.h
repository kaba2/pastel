#ifndef PASTELGFX_PACKRANGE_H
#define PASTELGFX_PACKRANGE_H

#include "pastel/sys/array.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	/*
	template <typename ConstView>
	void minMaxView(
		const ConstView& view,
		typename ConstView::Element& min,
		typename ConstView::Element& max)
	{
		typedef typename ConstView::Element Element;
		typedef typename ConstView::ConstCursor ConstCursor;

		class MinMaxFunctor
		{
		public:
			MinMaxFunctor(
				Element& min,
				Element& max)
				: min_(min)
				, max_(max)
			{
			}

			void operator()(const ConstCursor& cursor) const
			{
				if (*cursor < min_)
				{
					min_ = *cursor;
				}
				if (*cursor > max_)
				{
					max_ = *cursor;
				}
			}

		private:
			Element& min_;
			Element& max_;
		};

		MinMaxFunctor minMaxFunctor(min, max);

		constViewVisit(view, minMaxFunctor);
	}
	*/

	template <typename ConstView>
	void minMaxView(
		const ConstView& view,
		typename ConstView::Element& min,
		typename ConstView::Element& max)
	{
		typedef typename ConstView::ConstCursor ConstCursor;
		typedef typename ConstView::Element Element;

		const integer width = view.width();
		const integer height = view.height();

		ConstCursor yCursor = view.cursor(0, 0);

		Element minValue = *yCursor;
		Element maxValue = minValue;

		for (integer y = 0;y < height;++y)
		{
			ConstCursor xCursor = yCursor;

			for (integer x = 0;x < width;++x)
			{
				if (*xCursor < minValue)
				{
					minValue = *xCursor;
				}
				else if (*xCursor > maxValue)
				{
					maxValue = *xCursor;
				}

				xCursor.xIncrement();
			}

			yCursor.yIncrement();
		}

		min = minValue;
		max = maxValue;
	}

	namespace Detail
	{

		namespace SetView
		{

			template <typename Type>
			class Functor
			{
			public:
				explicit Functor(const Type& data)
					: data_(data)
				{
				}

				template <typename Cursor>
				void operator()(const Cursor& cursor)
				{
					*cursor = data_;
				}

			private:
				const Type& data_;
			};

		}

	}

	template <typename View>
	void setView(
		const View& view,
		const typename View::Element& data)
	{
		typedef typename View::Element Element;
		Detail::SetView::Functor<Element> functor(data);

		viewVisit(view, functor);
	}

	/*!
	Requirements:
	View models ViewConcept
	View::Element models RealConcept
	*/

	template <typename View>
	void packRange(
		const View& image,
		const typename View::Element& newMin,
		const typename View::Element& newMax)
	{
		typedef typename View::Cursor Cursor;

		ENSURE2(newMin <= newMax, newMin, newMax);

		typedef typename View::Element Real;

		const Real newValueDelta = newMax - newMin;

		const integer width = image.width();
		const integer height = image.height();

		Real minValue = infinity<Real>();
		Real maxValue = -infinity<Real>();

		minMaxView(image, minValue, maxValue);

		const Real valueDelta = maxValue - minValue;
		if (valueDelta == 0 || newValueDelta == 0)
		{
			setView(image, newMax);
		}
		else
		{
			const Real scale = newValueDelta / valueDelta;

			Cursor yCursor = image.cursor(0, 0);

			for (integer y = 0;y < height;++y)
			{
				Cursor xCursor = yCursor;

				for (integer x = 0;x < width;++x)
				{
					*xCursor = (*xCursor - minValue) * scale + newMin;
					xCursor.xIncrement();
				}

				yCursor.yIncrement();
			}
		}
	}

}

#endif
