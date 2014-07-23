#ifndef PASTELGFX_STRUCTURING_ELEMENTS_HPP
#define PASTELGFX_STRUCTURING_ELEMENTS_HPP

#include "pastel/gfx/structuring_elements.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace EllipsoidElement_
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<real, N>& center,
				const Vector<real, N>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Vector<real, N> delta

					= (Vector<real, N>(position) + 0.5 - center_) * invRadius_;

				image = dot(delta) < 1;
			}

		private:
			Vector<real, N> center_;
			const Vector<real, N> radius_;
			const Vector<real, N> invRadius_;
		};

	}

	template <int N, typename Image_View>
	void ellipsoidElement(

		const View<N, bool, Image_View>& image,
		const PASTEL_NO_DEDUCTION((Vector<real, N>))& diameter)
	{
		EllipsoidElement_::VisitFunctor<N> visitFunctor(
			Vector<real, N>(image.extent()) / 2,
			diameter / 2);
		visitPosition(image, visitFunctor);
	}

	template <int N, typename Image_View>
	void ellipsoidElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::ellipsoidElement(image, image.extent());
	}

	namespace DiamondElement_
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<real, N>& center,
				const Vector<real, N>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Vector<real, N> delta

					= (Vector<real, N>(position) + 0.5 - center_) * invRadius_;

				image = manhattanNorm(delta) < 1;
			}

		private:
			Vector<real, N> center_;
			const Vector<real, N> radius_;
			const Vector<real, N> invRadius_;
		};

	}

	template <int N, typename Image_View>
	void diamondElement(

		const View<N, bool, Image_View>& image,
		const PASTEL_NO_DEDUCTION((Vector<real, N>))& diameter)
	{
		DiamondElement_::VisitFunctor<N> visitFunctor(
			Vector<real, N>(image.extent()) / 2,
			diameter / 2);
		visitPosition(image, visitFunctor);
	}

	template <int N, typename Image_View>
	void diamondElement(
		const View<N, bool, Image_View>& image)
	{
		Pastel::diamondElement(image, image.extent());
	}

	namespace pEllipsoidElement_
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<real, N>& center,
				const Vector<real, N>& radius,
				real power)
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
				Vector<real, N> delta

					= (Vector<real, N>(position) + 0.5 - center_) * invRadius_;

				image = powerSum(delta, power_) < 1;
			}

		private:
			Vector<real, N> center_;
			const Vector<real, N> radius_;
			const Vector<real, N> invRadius_;
			const real power_;
		};

	}

	template <int N, typename Image_View>
	void pEllipsoidElement(

		const View<N, bool, Image_View>& image,
		real power,
		const PASTEL_NO_DEDUCTION((Vector<real, N>))& diameter)
	{
		ENSURE_OP(power, >, 0);

		pEllipsoidElement_::VisitFunctor<N> visitFunctor(
			Vector<real, N>(image.extent()) / 2,
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

	namespace BoxElement_
	{

		template <int N>
		class VisitFunctor
		{
		public:
			explicit VisitFunctor(
				const Vector<real, N>& center,
				const Vector<real, N>& radius)
				: center_(center)
				, radius_(radius)
				, invRadius_(inverse(radius))
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Vector<real, N> delta

					= (Vector<real, N>(position) + 0.5 - center_) * invRadius_;

				image = maxNorm(delta) < 1;
			}

		private:
			Vector<real, N> center_;
			const Vector<real, N> radius_;
			const Vector<real, N> invRadius_;
		};

	}

	template <int N, typename Image_View>
	void boxElement(

		const View<N, bool, Image_View>& image,
		const PASTEL_NO_DEDUCTION((Vector<real, N>))& diameter)
	{
		BoxElement_::VisitFunctor<N> visitFunctor(
			Vector<real, N>(image.extent()) / 2,
			diameter / 2);
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
