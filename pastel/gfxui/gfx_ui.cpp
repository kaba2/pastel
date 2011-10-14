#include "pastel/gfxui/gfx_ui.h"

#include "pastel/device/devicesystem.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	Gfx_Ui::Gfx_Ui(
		GfxRenderer<Color>* renderer,
		integer logicToRenderRatio)
		: renderer_(renderer)
		, renderTime_(0)
		, logicToRenderRatio_(logicToRenderRatio)
	{
	}

	void Gfx_Ui::onLogic(real deltaTime)
	{
		// Handle keys.

		handleKeys();

		// Handle logic.

		onGfxLogic();

		++renderTime_;
		if (renderTime_ >= logicToRenderRatio_)
		{
			renderTime_ = 0;

			// Render stuff.
			render();
		}
	}

	void Gfx_Ui::render()
	{
		onRender();
	}

	GfxRenderer<Color>& Gfx_Ui::renderer()
	{
		return *renderer_;
	}

	const GfxRenderer<Color>& Gfx_Ui::renderer() const
	{
		return *renderer_;
	}

	Vector2 Gfx_Ui::screenPointToWorld(const Vector2& screenPoint) const
	{
		return transformPoint(
			renderer_->viewWindow().at(screenPoint),
			renderer_->viewTransformation());
	}

	Vector2 Gfx_Ui::screenVectorToWorld(const Vector2& screenVector) const
	{
		return transformVector(
			evaluate(renderer_->viewWindow().extent() * screenVector),
			renderer_->viewTransformation());
	}

	// Private

	void Gfx_Ui::handleKeys()
	{
		const real TranslationSpeed = 0.02;
		const real ZoomFactor = 1.05;
		const real RotationSpeed = 0.02;

		if (deviceSystem().keyDown(SDLK_q))
		{
			const AffineTransformation2 transformation(renderer_->viewTransformation());
			renderer_->setViewTransformation(
				rotation2<real>(RotationSpeed) * transformation);
		}
		if (deviceSystem().keyDown(SDLK_e))
		{
			const AffineTransformation2 transformation(renderer_->viewTransformation());
			renderer_->setViewTransformation(
				rotation2<real>(-RotationSpeed) * transformation);
		}
		if (deviceSystem().keyDown(SDLK_a))
		{
			const AffineTransformation2 transformation(renderer_->viewTransformation());
			renderer_->setViewTransformation(
				transformation *
				translation2<real>(-TranslationSpeed * transformation.matrix()[0]));

		}
		if (deviceSystem().keyDown(SDLK_d))
		{
			const AffineTransformation2 transformation(renderer_->viewTransformation());
			renderer_->setViewTransformation(
				transformation *
				translation2<real>(TranslationSpeed * transformation.matrix()[0]));
		}
		if (deviceSystem().keyDown(SDLK_w))
		{
			const AffineTransformation2 transformation(renderer_->viewTransformation());
			renderer_->setViewTransformation(
				transformation *
				translation2<real>(TranslationSpeed * transformation.matrix()[1]));
		}
		if (deviceSystem().keyDown(SDLK_s))
		{
			const AffineTransformation2 transformation(renderer_->viewTransformation());
			renderer_->setViewTransformation(
				transformation *
				translation2<real>(-TranslationSpeed * transformation.matrix()[1]));
		}
		if (deviceSystem().keyDown(SDLK_r))
		{
			AffineTransformation2 transformation(renderer_->viewTransformation());
			transformation.matrix() /= ZoomFactor;
			renderer_->setViewTransformation(transformation);
		}

		if (deviceSystem().keyDown(SDLK_f))
		{
			AffineTransformation2 transformation(renderer_->viewTransformation());
			transformation.matrix() *= ZoomFactor;
			renderer_->setViewTransformation(transformation);
		}
	}

	void Gfx_Ui::onRender()			
	{
	}

	void Gfx_Ui::onGfxLogic()
	{
	}

}
