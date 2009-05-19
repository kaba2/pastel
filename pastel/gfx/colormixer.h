#ifndef PASTEL_COLORMIXER_H
#define PASTEL_COLORMIXER_H

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
