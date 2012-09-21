// Description: GfxRenderer class
// Detail: An abstraction for 2d per-object renderers
// Documentation: renderer.txt

#ifndef PASTELGFX_GFXRENDERER_H
#define PASTELGFX_GFXRENDERER_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/math/affine_transformation.h"
#include "pastel/geometry/triangle.h"
#include "pastel/geometry/segment.h"

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/sys/array.h"

namespace Pastel
{

	class ResamplingMode
	{
	public:
		enum Enum
		{
			Nearest,
			Bilinear,
			Trilinear,
			Anisotropic
		};
	};

	template <typename Type>
	class GfxState
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		GfxState();

		void swap(GfxState& that);

		// Drawing state

		bool filled_;
		Type color_;
		integer texture_;
		ResamplingMode::Enum resamplingMode_;

		// Transformation state

		AffineTransformation<real> viewTransformation_;
		AffineTransformation<real> modelTransformation_;
		AlignedBox2 viewWindow_;
	};

	template <typename Type>
	class GfxRenderer
	{
	public:
		// Using default copy constructor.

		GfxRenderer();

		virtual ~GfxRenderer() {}

		void swap(GfxRenderer<Type>& that);

		GfxRenderer<Type>& operator=(const GfxRenderer& that);

		void setState(const GfxState<Type>& state);
		const GfxState<Type>& state() const;
		virtual void onSetState();

		// Drawing state

		void setFilled(bool filled);
		bool filled() const;
		virtual void onSetFilled() {}

		void setColor(const Type& color);
		Type color() const;
		virtual void onSetColor() {}

		void setTexture(integer textureId);
		integer texture() const;
		virtual void onSetTexture() {}

		void setResamplingMode(ResamplingMode::Enum resamplingMode);
		ResamplingMode::Enum resamplingMode() const;
		virtual void onSetResamplingMode() {}

		// Transformation state

		void setViewTransformation(
			const AffineTransformation<real>& viewTransformation);
		AffineTransformation<real> viewTransformation() const;
		virtual void onSetViewTransformation() {}

		void setModelTransformation(
			const AffineTransformation<real>& modelTransformation);
		AffineTransformation<real> modelTransformation() const;
		virtual void onSetModelTransformation() {}

		void setViewWindow(
			const AlignedBox2& window);
		AlignedBox2 viewWindow() const;
		virtual void onSetViewWindow() {}

		// Drawing

		virtual integer uploadTexture(const Array<Type, 2>& image) = 0;

		virtual void clear() const = 0;

		virtual void drawPoint(
			const Vector2& point) const = 0;

		virtual void drawSegment(
			const Segment2& segment) const = 0;

		virtual void drawTriangle(
			const Triangle2& triangle) const = 0;

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Tuple<Type, 3>& colorTriangle) const = 0;

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Triangle2& texture) const = 0;

	private:
		GfxState<Type> state_;
	};

}

#include "pastel/gfx/gfxrenderer.hpp"

#endif
