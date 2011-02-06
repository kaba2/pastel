#ifndef PASTEL_PROGRESS_H
#define PASTEL_PROGRESS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/log.h"

#include <ctime>

namespace Pastel
{

	class Progress
	{
	public:
		explicit Progress(
			integer steps,
			const std::string& name = "")
			: name_(name)
			, step_(0)
			, steps_(steps)
			, lastReportTime_(std::clock())
		{
			ENSURE_OP(steps, >, 0);
		}

		void report()
		{
			++step_;
			const std::clock_t currentTime = std::clock();
			const real elapsedSeconds =
				(currentTime - lastReportTime_) / CLOCKS_PER_SEC;
			const real ReportDelay = 5;
			if (elapsedSeconds >= ReportDelay)
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

	private:
		std::string name_;
		integer step_;
		integer steps_;
		std::clock_t lastReportTime_;
	};	

}

#endif
