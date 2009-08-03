#ifndef PASTEL_RIPMAP_TOOLS_HPP
#define PASTEL_RIPMAP_TOOLS_HPP

#include "pastel/dsp/ripmap_tools.h"

#include "pastel/sys/view_tools.h"

namespace Pastel
{

	namespace Detail_RipMapTransform
	{

		template <int N, typename Type, typename TransformFunctor>
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

	template <int N, typename Type, typename TransformFunctor>
	void transform(
		RipMap<N, Type>& ripMap,
		const TransformFunctor& transform)
	{
		Detail_RipMapTransform::Visitor<N, Type, TransformFunctor> visitor(transform);

		visit(ripMap.view(), visitor);
	}

}

#endif
