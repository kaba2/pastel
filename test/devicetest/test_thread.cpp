#include "pasteldevicetest.h"

#include "pastel/device/thread.h"

using namespace Pastel;

namespace
{

	class SimpleThread
		: public Thread
	{
	public:
		SimpleThread(integer rangeBegin, integer rangeEnd)
			: rangeBegin_(rangeBegin)
			, rangeEnd_(rangeEnd)
		{
		}

		integer result() const
		{
			return result_;
		}

	private:
		virtual void run()
		{
			result_ = 0;
			for (integer i = rangeBegin_;i < rangeEnd_;++i)
			{
				result_ += i;
			}
		}

		integer rangeBegin_;
		integer rangeEnd_;
		integer result_;
	};

	typedef CountedPtr<SimpleThread> SimpleThreadPtr;

	integer compute(integer rangeBegin, integer rangeEnd)
	{
		const integer Threads = 4;
		std::vector<SimpleThreadPtr> threadSet_;
		threadSet_.reserve(Threads);

		const integer rangeDelta = rangeEnd - rangeBegin;
		const real rangeStep = (real)rangeDelta / Threads;

		for (integer i = 0;i < Threads;++i)
		{
			const integer blockBegin = rangeBegin + i * rangeStep;
			integer blockEnd = std::min(
				(integer)(rangeBegin + (i + 1) * rangeStep), rangeEnd);
			if (i == Threads - 1)
			{
				blockEnd = rangeEnd;
			}

			//log() << blockBegin << " to " << blockEnd << logNewLine;

			threadSet_.push_back(SimpleThreadPtr(
				new SimpleThread(blockBegin, blockEnd)));
		}

		for (integer i = 0;i < Threads;++i)
		{
			threadSet_[i]->launch();
		}

		for (integer i = 0;i < Threads;++i)
		{
			threadSet_[i]->wait();
		}

		integer result = 0;
		for (integer i = 0;i < Threads;++i)
		{
			const SimpleThreadPtr& thread = threadSet_[i];

			//log() << thread->result() << logNewLine;
			
			result += thread->result();
		}
		
		return result;
	}	

	integer computeDirect(integer rangeBegin, integer rangeEnd)
	{
		const integer rangeDelta = rangeEnd - rangeBegin;
		return ((rangeBegin + rangeEnd - 1) * rangeDelta) / 2;
	}

	void test()
	{
		const integer rangeBegin = 1;
		const integer rangeEnd = 100;

		const integer threadResult = compute(rangeBegin, rangeEnd);
		const integer directResult = computeDirect(rangeBegin, rangeEnd);

		REPORT2(threadResult != directResult, threadResult, directResult);
	}

	void testAdd()
	{
		deviceTestList().add("Thread", test);
	}

	CallFunction run(testAdd);

}
