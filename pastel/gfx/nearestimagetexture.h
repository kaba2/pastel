#ifndef PASTELGFX_NEARESTIMAGETEXTURE_H
#define PASTELGFX_NEARESTIMAGETEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/sys/arrayextender.h"

namespace Pastel
{

	template <typename Type>
	class NearestImageTexture
		: public Texture<Type>
	{
	private:
		typedef Texture<Type> Base;

	public:
		using Base::Element;

		NearestImageTexture()
			: image_(0)
			, extender_()
			, extent_()
		{
		}

		explicit NearestImageTexture(
			const LinearArray<2, Type>& image,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>())
			: image_(&image)
			, extender_(extender)
			, extent_(image.extent())
		{
		}

		virtual ~NearestImageTexture()
		{
		}

		virtual Element operator()(
			const Point2& p, 
			const Vector2& dpdx, 
			const Vector2& dpdy) const
		{
			const Point<2, integer> x(asVector(p) * extent_);
			
			return extender_(*image_, x);
		}

		void setImage(const LinearArray<2, Type>& image)
		{
			image_ = &image;
			extent_ = Vector2(image.extent());
		}

		void setExtender(const ArrayExtender<2, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("NearestImage");
		}

	private:
		const LinearArray<2, Type>* image_;
		ArrayExtender<2, Type> extender_;
		Vector2 extent_;
	};

	template <typename Type>
	NearestImageTexture<Type> nearestImageTexture(
		const LinearArray<2, Type>& image,
		const ArrayExtender<2, PASTEL_NO_DEDUCTION(Type)>& extender = ArrayExtender<2, Type>())
	{
		return NearestImageTexture<Type>(image, extender);
	}

}

#include "pastel/gfx/nearestimagetexture.hpp"

#endif
