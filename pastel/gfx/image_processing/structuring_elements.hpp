#ifndef PASTELGFX_STRUCTURING_ELEMENTS_HPP
#define PASTELGFX_STRUCTURING_ELEMENTS_HPP

#include "pastel/gfx/image_processing/structuring_elements.h"

#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/view/view_visit.h"

namespace Pastel
{

	namespace EllipsoidElement_
	{

		template <integer N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<dreal, N>& center,
				const Vector<dreal, N>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Vector<dreal, N> delta

					= (Vector<dreal, N>(position) + 0.5 - center_) * invRadius_;

				image = dot(delta) < 1;
			}

		private:
			Vector<dreal, N> center_;
			Vector<dreal, N> radius_;
			Vector<dreal, N> invRadius_;
		};

	}

	template <integer N, typename Image_View>
	void ellipsoidElement(
		const View<N, bool, Image_View>& image,
		const NoDeduction<Vector<dreal, N>>& diameter)
	{
		EllipsoidElement_::VisitFunctor<N> visitFunctor(
			Vector<dreal, N>(image.extent()) / 2,
			diameter / 2);
		visitPosition(image, visitFunctor);
	}

	template <integer N, typename Image_View>
	void ellipsoidElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::ellipsoidElement(image, image.extent());
	}

	namespace DiamondElement_
	{

		template <integer N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<dreal, N>& center,
				const Vector<dreal, N>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Vector<dreal, N> delta

					= (Vector<dreal, N>(position) + 0.5 - center_) * invRadius_;

				image = manhattanNorm(delta) < 1;
			}

		private:
			Vector<dreal, N> center_;
			Vector<dreal, N> radius_;
			Vector<dreal, N> invRadius_;
		};

	}

	template <integer N, typename Image_View>
	void diamondElement(
		const View<N, bool, Image_View>& image,
		const NoDeduction<Vector<dreal, N>>& diameter)
	{
		DiamondElement_::VisitFunctor<N> visitFunctor(
			Vector<dreal, N>(image.extent()) / 2,
			diameter / 2);
		visitPosition(image, visitFunctor);
	}

	template <integer N, typename Image_View>
	void diamondElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::diamondElement(image, image.extent());
	}

	namespace pEllipsoidElement_
	{

		template <integer N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<dreal, N>& center,
				const Vector<dreal, N>& radius,
				dreal power)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
				, power_(power)
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Vector<dreal, N> delta

					= (Vector<dreal, N>(position) + 0.5 - center_) * invRadius_;

				image = powerSum(delta, power_) < 1;
			}

		private:
			Vector<dreal, N> center_;
			Vector<dreal, N> radius_;
			Vector<dreal, N> invRadius_;
			dreal power_;
		};

	}

	template <integer N, typename Image_View>
	void pEllipsoidElement(
		const View<N, bool, Image_View>& image,
		dreal power,
		const NoDeduction<Vector<dreal, N>>& diameter)
	{
		ENSURE_OP(power, >, 0);

		pEllipsoidElement_::VisitFunctor<N> visitFunctor(
			Vector<dreal, N>(image.extent()) / 2,
			diameter / 2, power);
		visitPosition(image, visitFunctor);
	}

	template <integer N, typename Image_View>
	void pEllipsoidElement(
		const View<N, bool, Image_View>& image,
		dreal power)
	{
		Pastel::pEllipsoidElement(image, power, image.extent());
	}

	namespace BoxElement_
	{

		template <integer N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<dreal, N>& center,
				const Vector<dreal, N>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Vector<dreal, N> delta

					= (Vector<dreal, N>(position) + 0.5 - center_) * invRadius_;

				image = maxNorm(delta) < 1;
			}

		private:
			Vector<dreal, N> center_;
			Vector<dreal, N> radius_;
			Vector<dreal, N> invRadius_;
		};

	}

	template <integer N, typename Image_View>
	void boxElement(
		const View<N, bool, Image_View>& image,
		const NoDeduction<Vector<dreal, N>>& diameter)
	{
		BoxElement_::VisitFunctor<N> visitFunctor(
			Vector<dreal, N>(image.extent()) / 2,
			diameter / 2);
		visitPosition(image, visitFunctor);
	}

	template <integer N, typename Image_View>
	void boxElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::boxElement(image, image.extent());
	}

}

#endif
