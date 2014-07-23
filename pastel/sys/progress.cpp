#include "pastel/sys/progress.h"

namespace Pastel
{

	Progress::Progress(
		integer steps,
		const std::string& name,
		real reportDelayInSeconds)
		: name_(name)
		, step_(0)
		, steps_(steps)
		, lastReportTime_(std::clock())
		, reportDelay_(reportDelayInSeconds)
	{
		ENSURE_OP(steps, >, 0);
	}
	
	void Progress::swap(Progress& that)
	{
		name_.swap(that.name_);
		std::swap(step_, that.step_);
		std::swap(steps_, that.steps_);
		std::swap(lastReportTime_, that.lastReportTime_);
		std::swap(reportDelay_, that.reportDelay_);
	}

	void Progress::report()
	{
		++step_;
		std::clock_t currentTime = std::clock();
		const real elapsedSeconds =
			(currentTime - lastReportTime_) / CLOCKS_PER_SEC;
		if (elapsedSeconds >= reportDelay_)
		{
			const integer percent = 

				(step_ * 100) / steps_;
			if (!name_.empty())
			{
				log() << name_ << " ";
			}
			log() << percent << "% " << logNewLine;
			lastReportTime_ = currentTime;
		}
	}

	void Progress::setReportDelay(real reportDelay)
	{
		ENSURE_OP(reportDelay, >, 0);

		reportDelay_ = reportDelay;
	}

	real Progress::reportDelay() const
	{
		return reportDelay_;
	}

	const std::string& Progress::name() const
	{
		return name_;
	}

	integer Progress::steps() const
	{
		return steps_;
	}

	integer Progress::step() const
	{
		return step_;
	}

}
