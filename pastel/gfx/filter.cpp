#include "pastel/gfx/filter.h"

namespace Pastel
{

	Filter::Filter(
		real radius,
		const std::string& name)
		: radius_(radius)
		, name_(name)
	{
		ENSURE_OP(radius, >, 0);
	}

	Filter::~Filter()
	{
	}

	void Filter::setRadius(real radius)
	{
		ENSURE_OP(radius, >, 0);
		radius_ = radius;
		onSetRadius();
	}

	real Filter::radius() const
	{
		return radius_;
	}

	void Filter::onSetRadius()
	{
	}

	real Filter::evaluate(real x) const
	{
		if (x < -radius_ || x > radius_)
		{
			return 0;
		}

		return evaluateInRange(x);
	}

	const std::string& Filter::name() const
	{
		return name_;
	}

}
