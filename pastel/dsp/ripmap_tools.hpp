#ifndef PASTEL_RIPMAP_TOOLS_HPP
#define PASTEL_RIPMAP_TOOLS_HPP

#include "pastel/dsp/ripmap_tools.h"

#include "pastel/sys/view_tools.h"

namespace Pastel
{

	namespace Detail_RipMapTransform
	{

		template <typename Type, int N, typename TransformFunctor>
		class Visitor
		{
		public:
			explicit Visitor(
				const TransformFunctor& transform)
				: transform_(transform)
			{
			}

			void operator()(Array<Type, N>& image) const
			{
				Pastel::transform(arrayView(image), transform_);
			}

		private:
			const TransformFunctor& transform_;
		};

	}

	template <typename Type, int N, typename TransformFunctor>
	void transform(
		RipMap<Type, N>& ripMap,
		const TransformFunctor& transform)
	{
		Detail_RipMapTransform::Visitor<Type, N, TransformFunctor> visitor(transform);

		visit(ripMap.view(), visitor);
	}

}

#endif
