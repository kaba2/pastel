// Description: ColorMixer class
// Detail: An abstract class for color mixers

#ifndef PASTELGFX_COLORMIXER_H
#define PASTELGFX_COLORMIXER_H

namespace Pastel
{

	template <typename Type>
	class ColorMixer
	{
	public:
		virtual ~ColorMixer()
		{
		}

		virtual Type operator()(
			const Type& background,
			const Type& texture) const = 0;
	};

}

#endif
