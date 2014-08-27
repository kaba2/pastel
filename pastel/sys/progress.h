// Description: Progress reporter
// Documentation: progress.txt

#ifndef PASTELSYS_PROGRESS_H
#define PASTELSYS_PROGRESS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/log.h"

#include <ctime>

namespace Pastel
{

	class Progress
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs a Progress object.
		explicit Progress(
			integer steps,
			const std::string& name = "",
			real reportDelayInSeconds = 5);

		//! Swaps two Progress objects.
		void swap(Progress& that);

		//! Makes one step of progress.
		void report();

		//! Sets the reporting delay.
		/*!
		Preconditions:
		reportDelayInSeconds > 0

		The actual reporting delay might be longer,
		but not shorter.
		*/
		void setReportDelay(real reportDelayInSeconds);

		//! Returns the reporting delay.
		real reportDelay() const;

		//! Returns the name of the task.
		const std::string& name() const;

		//! Returns the total number of steps.
		integer steps() const;

		//! Returns the current step.
		integer step() const;

	private:
		std::string name_;
		integer step_;
		integer steps_;
		std::clock_t lastReportTime_;
		real reportDelay_;
	};	

}

#endif
