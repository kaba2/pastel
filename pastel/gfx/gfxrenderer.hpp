#ifndef PASTELGFX_GFXRENDERER_HPP
#define PASTELGFX_GFXRENDERER_HPP

#include "pastel/gfx/gfxrenderer.h"

namespace Pastel
{

	template <typename Type>
	GfxState<Type>::GfxState()
		: filled_(true)
		, color_(1)
		, texture_(0)
		, resamplingMode_(ResamplingMode::Nearest)
		, viewTransformation_()
		, modelTransformation_()
		, viewWindow_(-1, -1, 1, 1)
	{
	}

	template <typename Type>
	void GfxState<Type>::swap(GfxState& that)
	{
		using std::swap;

		std::swap(filled_, that.filled_);
		swap(color_, that.color_);
		std::swap(texture_, that.texture_);
		std::swap(resamplingMode_, that.resamplingMode_);
		viewTransformation_.swap(that.viewTransformation_);
		modelTransformation_.swap(that.modelTransformation_);
		viewWindow_.swap(that.viewWindow_);
	}

}

namespace Pastel
{

	template <typename Type>
	GfxRenderer<Type>::GfxRenderer()
		: state_()
	{
	}

	template <typename Type>
	void GfxRenderer<Type>::swap(GfxRenderer<Type>& that)
	{
		state_.swap(that.state_);
	}

	template <typename Type>
	GfxRenderer<Type>& GfxRenderer<Type>::operator=(const GfxRenderer<Type>& that)
	{
		GfxState<Type> copyState(that.state_);
		state_.swap(copyState);
		return *this;
	}

	// Drawing state

	template <typename Type>
	void GfxRenderer<Type>::setState(
		const GfxState<Type>& state)
	{
		GfxState<Type> copy(state);
		state_.swap(copy);
		onSetState();
	}

	template <typename Type>
	const GfxState<Type>& GfxRenderer<Type>::state() const
	{
		return state_;
	}

	template <typename Type>
	void GfxRenderer<Type>::onSetState()
	{
		onSetTexture();
		onSetColor();
		onSetFilled();
		onSetResamplingMode();

		onSetViewTransformation();
		onSetModelTransformation();
		onSetViewWindow();
	}

	template <typename Type>
	void GfxRenderer<Type>::setFilled(bool filled)
	{
		state_.filled_ = filled;
		onSetFilled();
	}

	template <typename Type>
	bool GfxRenderer<Type>::filled() const
	{
		return state_.filled_;
	}

	template <typename Type>
	void GfxRenderer<Type>::setColor(const Type& color)
	{
		state_.color_ = color;
		onSetColor();
	}

	template <typename Type>
	Type GfxRenderer<Type>::color() const
	{
		return state_.color_;
	}

	template <typename Type>
	void GfxRenderer<Type>::setTexture(integer textureId)
	{
		state_.texture_ = textureId;
		onSetTexture();
	}

	template <typename Type>
	integer GfxRenderer<Type>::texture() const
	{
		return state_.texture_;
	}

	template <typename Type>
	void GfxRenderer<Type>::setResamplingMode(ResamplingMode::Enum resamplingMode)
	{
		state_.resamplingMode_ = resamplingMode;
		onSetResamplingMode();
	}

	template <typename Type>
	ResamplingMode::Enum GfxRenderer<Type>::resamplingMode() const
	{
		return state_.resamplingMode_;
	}

	// Transformation state

	template <typename Type>
	void GfxRenderer<Type>::setViewTransformation(
		const AffineTransformation2& viewTransformation)
	{
		state_.viewTransformation_ = viewTransformation;
		onSetViewTransformation();
	}

	template <typename Type>
	AffineTransformation2 GfxRenderer<Type>::viewTransformation() const
	{
		return state_.viewTransformation_;
	}

	template <typename Type>
	void GfxRenderer<Type>::setModelTransformation(
		const AffineTransformation2& modelTransformation)
	{
		state_.modelTransformation_ = modelTransformation;
		onSetModelTransformation();
	}

	template <typename Type>
	AffineTransformation2 GfxRenderer<Type>::modelTransformation() const
	{
		return state_.modelTransformation_;
	}

	template <typename Type>
	void GfxRenderer<Type>::setViewWindow(
		const AlignedBox2& window)
	{
		state_.viewWindow_ = window;
		onSetViewWindow();
	}

	template <typename Type>
	AlignedBox2 GfxRenderer<Type>::viewWindow() const
	{
		return state_.viewWindow_;
	}

}

#endif
