#include "pastel/sys/nativeallocator.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/addressof.h"

namespace Pastel
{

	NativeAllocator::NativeAllocator(
		integer unitSize,
		bool isPodDataIgnored)
		: unitSize_(unitSize)
		, unitsAllocated_(0)
	{
		unused(isPodDataIgnored);

		ENSURE1(unitSize >= 0, unitSize);
	}

	NativeAllocator::~NativeAllocator()
	{
		REPORT1(unitsAllocated_ != 0,
			unitsAllocated_);
	}

	bool NativeAllocator::operator==(
		const NativeAllocator& that) const
	{
		return this == &that;
	}

	bool NativeAllocator::operator<(
		const NativeAllocator& that) const
	{
		return this < &that;
	}

	void NativeAllocator::swap(NativeAllocator& that)
	{
		std::swap(unitSize_, that.unitSize_);
		std::swap(unitsAllocated_, that.unitsAllocated_);
	}

	void NativeAllocator::clear()
	{
	}

	integer NativeAllocator::unitSize() const
	{
		return unitSize_;
	}

	integer NativeAllocator::size() const
	{
		return unitsAllocated_;
	}

	integer NativeAllocator::capacity() const
	{
		return unitsAllocated_;
	}

	void* NativeAllocator::allocate()
	{
		void* memory = allocateRaw(unitSize_);

		++unitsAllocated_;

		return memory;
	}

	void NativeAllocator::deallocate(void* memAddress)
	{
		// Clearly a null pointer can't
		// be allocated from this allocator.

		ENSURE(memAddress);

		deallocateRaw(memAddress);

		--unitsAllocated_;
	}

}
