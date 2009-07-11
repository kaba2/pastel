#ifndef PASTEL_RANGE_H
#define PASTEL_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	class PASTELSYS Range
	{
	public:
		Range(integer begin, integer end)
			: begin_(begin)
			, end_(end)
			, step_(1)
		{
			if (end < begin)
			{
				step_ = -1;
			}
		}

		Range(integer begin, integer end, integer step)
			: begin_(begin)
			, end_(end)
			, step_(step)
		{
			PENSURE(((begin <= end) && (step >= 0)) ||
				((begin > end) && (step <= 0)));
		}

		integer numbers() const
		{
			return (size() + std::abs(step_) - 1) / std::abs(step_);
		}

		integer begin() const
		{
			return begin_;
		}

		integer end() const
		{
			return end_;
		}

		integer size() const
		{
			return std::abs(end_ - begin_) + 1;
		}

		integer step() const
		{
			return step_;
		}

	private:
		integer begin_;
		integer end_;
		integer step_;
	};

}

#endif
