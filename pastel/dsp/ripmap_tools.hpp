#ifndef PASTELDSP_RIPMAP_TOOLS_HPP
#define PASTELDSP_RIPMAP_TOOLS_HPP

#include "pastel/dsp/ripmap_tools.h"

#include "pastel/sys/view_tools.h"

namespace Pastel
{

	namespace RipMapTransform_
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
		RipMapTransform_::Visitor<Type, N, TransformFunctor> visitor(transform);

		visit(ripMap.view(), visitor);
	}

}

#endif
