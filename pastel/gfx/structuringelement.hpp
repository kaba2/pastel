#ifndef PASTEL_STRUCTURINGELEMENT_HPP
#define PASTEL_STRUCTURINGELEMENT_HPP

#include "pastel/gfx/structuringelement.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_EllipsoidElement
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Point<N, real>& center,
				const Vector<N, real>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Point<N, integer>& position,
				bool& image) const
			{
				const Vector<N, real> delta
					= (Point<N, real>(position) + 0.5 - center_) * invRadius_;

				image = dot(delta) < 1;
			}

		private:
			const Point<N, real> center_;
			const Vector<N, real> radius_;
			const Vector<N, real> invRadius_;
		};

	}

	template <int N, typename Image_View>
	void ellipsoidElement(
		const View<N, bool, Image_View>& image,
		const PASTEL_NO_DEDUCTION((Vector<N, real>))& diameter)
	{
		Detail_EllipsoidElement::VisitFunctor<N> visitFunctor(
			asPoint(Vector<N, real>(image.extent()) / 2),
			diameter / 2);
		visitPosition(image, visitFunctor);
	}

	template <int N, typename Image_View>
	void ellipsoidElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::ellipsoidElement(image, image.extent());
	}

	namespace Detail_DiamondElement
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Point<N, real>& center,
				const Vector<N, real>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Point<N, integer>& position,
				bool& image) const
			{
				const Vector<N, real> delta
					= (Point<N, real>(position) + 0.5 - center_) * invRadius_;

				image = normManhattan(delta) < 1;
			}

		private:
			const Point<N, real> center_;
			const Vector<N, real> radius_;
			const Vector<N, real> invRadius_;
		};

	}

	template <int N, typename Image_View>
	void diamondElement(
		const View<N, bool, Image_View>& image,
		const PASTEL_NO_DEDUCTION((Vector<N, real>))& diameter)
	{
		Detail_DiamondElement::VisitFunctor<N> visitFunctor(
			asPoint(Vector<N, real>(image.extent()) / 2),
			diameter / 2);
		visitPosition(image, visitFunctor);
	}

	template <int N, typename Image_View>
	void diamondElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::diamondElement(image, image.extent());
	}

	namespace Detail_pEllipsoidElement
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Point<N, real>& center,
				const Vector<N, real>& radius,
				real power)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
				, power_(power)
			{
			}

			void operator()(
				const Point<N, integer>& position,
				bool& image) const
			{
				const Vector<N, real> delta
					= (Point<N, real>(position) + 0.5 - center_) * invRadius_;

				image = powerSum(delta, power_) < 1;
			}

		private:
			const Point<N, real> center_;
			const Vector<N, real> radius_;
			const Vector<N, real> invRadius_;
			const real power_;
		};

	}

	template <int N, typename Image_View>
	void pEllipsoidElement(
		const View<N, bool, Image_View>& image,
		real power,
		const PASTEL_NO_DEDUCTION((Vector<N, real>))& diameter)
	{
		ENSURE_OP(power, >, 0);

		Detail_pEllipsoidElement::VisitFunctor<N> visitFunctor(
			asPoint(Vector<N, real>(image.extent()) / 2),
			diameter / 2, power);
		visitPosition(image, visitFunctor);
	}

	template <int N, typename Image_View>
	void pEllipsoidElement(
		const View<N, bool, Image_View>& image,
		real power)
	{
		Pastel::pEllipsoidElement(image, power, image.extent());
	}

	namespace Detail_BoxElement
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Point<N, real>& center,
				const Vector<N, real>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Point<N, integer>& position,
				bool& image) const
			{
				const Vector<N, real> delta
					= (Point<N, real>(position) + 0.5 - center_) * invRadius_;

				image = normInfinity(delta) < 1;
			}

		private:
			const Point<N, real> center_;
			const Vector<N, real> radius_;
			const Vector<N, real> invRadius_;
		};

	}

	template <int N, typename Image_View>
	void boxElement(
		const View<N, bool, Image_View>& image,
		const PASTEL_NO_DEDUCTION((Vector<N, real>))& diameter)
	{
		Detail_BoxElement::VisitFunctor<N> visitFunctor(
			asPoint(Vector<N, real>(image.extent()) / 2),
			diameter / 2, power);
		visitPosition(image, visitFunctor);
	}

	template <int N, typename Image_View>
	void boxElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::boxElement(image, image.extent());
	}

}

#endif
