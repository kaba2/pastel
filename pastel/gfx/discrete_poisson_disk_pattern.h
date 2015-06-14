// Description: Discrete poisson-disk pattern
// Detail: Sets those pixels for which the poisson-disk condition is fulfilled

#ifndef PASTELGFX_DISCRETE_POISSON_DISK_PATTERN_H
#define PASTELGFX_DISCRETE_POISSON_DISK_PATTERN_H

#include "pastel/sys/view/view.h"
#include "pastel/sys/view/view_visit.h"
#include "pastel/sys/array.h"

#include "pastel/gfx/drawing.h"

namespace Pastel
{

	namespace DiscretePoissonDiskPattern_
	{

		class Mixer
		{
		public:
			bool operator()(bool image,  bool texture) const
			{
				return (image && !texture);
			}
		};

		template <integer N, typename Image_View, typename Element_ConstView>
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
			Vector<integer, N> elementOffset_;
		};

	}

	template <integer N, typename Image_View, typename Element_ConstView>
	void discretePoissonDiskPattern(
		const ConstView<N, bool, Element_ConstView>& element,
		const View<N, bool, Image_View>& image)
	{
		DiscretePoissonDiskPattern_::Visitor<N, Image_View, Element_ConstView>
			visitor(element, image);

		visitPosition(image, visitor);
	}

}

#endif
