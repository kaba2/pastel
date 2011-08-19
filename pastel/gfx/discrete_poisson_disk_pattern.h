// Description: Discrete poisson-disk pattern
// Detail: Sets those pixels for which the poisson-disk condition is fulfilled

#ifndef PASTEL_DISCRETE_POISSON_DISK_PATTERN_H
#define PASTEL_DISCRETE_POISSON_DISK_PATTERN_H

#include "pastel/sys/view.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/array.h"

#include "pastel/gfx/draw.h"

namespace Pastel
{

	namespace Detail_DiscretePoissonDiskPattern
	{

		class Mixer
		{
		public:
			bool operator()(bool image,  bool texture) const
			{
				return (image && !texture);
			}
		};

		template <int N, typename Image_View, typename Element_ConstView>
		class Visitor
		{
		public:
			Visitor(
				const ConstView<N, bool, Element_ConstView>& element,
				const View<N, bool, Image_View>& image)
				: element_(element)
				, image_(image)
				, elementOffset_(element.extent() / 2)
			{
			}


			void operator()(
				const Vector<integer, N>& position,
				bool& image) const
			{
				Mixer mixer;
				if (image)
				{
					drawView(
						element_,
						position - elementOffset_,
						image_,
						mixer);

					image = true;
				}
			}

		private:
			const ConstView<N, bool, Element_ConstView>& element_;
			const View<N, bool, Image_View>& image_;
			const Vector<integer, N> elementOffset_;
		};

	}

	template <int N, typename Image_View, typename Element_ConstView>
	void discretePoissonDiskPattern(
		const ConstView<N, bool, Element_ConstView>& element,
		const View<N, bool, Image_View>& image)
	{
		Detail_DiscretePoissonDiskPattern::Visitor<N, Image_View, Element_ConstView>
			visitor(element, image);

		visitPosition(image, visitor);
	}

}

#endif
