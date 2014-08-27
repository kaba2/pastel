#ifndef PASTELSYS_NATIVE_ALLOCATOR_HPP
#define PASTELSYS_NATIVE_ALLOCATOR_HPP

#include "pastel/sys/native_allocator.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/addressof.h"

namespace Pastel
{

	inline NativeAllocator::NativeAllocator(
		integer unitSize,
		bool isPodDataIgnored)
		: unitSize_(unitSize)
		, unitsAllocated_(0)
	{
		unused(isPodDataIgnored);

		ENSURE_OP(unitSize, >=, 0);
	}

	inline NativeAllocator::~NativeAllocator()
	{
		REPORT1(unitsAllocated_ != 0,
			unitsAllocated_);
	}

	inline bool NativeAllocator::operator==(
		const NativeAllocator& that) const
	{
		return this == &that;
	}

	inline bool NativeAllocator::operator<(
		const NativeAllocator& that) const
	{
		return this < &that;
	}

	inline void NativeAllocator::swap(NativeAllocator& that)
	{
		std::swap(unitSize_, that.unitSize_);
		std::swap(unitsAllocated_, that.unitsAllocated_);
	}

	inline void NativeAllocator::clear()
	{
	}

	inline integer NativeAllocator::unitSize() const
	{
		return unitSize_;
	}

	inline integer NativeAllocator::size() const
	{
		return unitsAllocated_;
	}

	inline integer NativeAllocator::capacity() const
	{
		return unitsAllocated_;
	}

	inline void* NativeAllocator::allocate()
	{
		void* memory = allocateRaw(unitSize_);

		++unitsAllocated_;

		return memory;
	}

	inline void NativeAllocator::deallocate(const void* memAddress)
	{
		// Clearly a null pointer can't
		// be allocated from this allocator.

		ENSURE(memAddress);

		deallocateRaw(memAddress);

		--unitsAllocated_;
	}

	inline void swap(NativeAllocator& left,
		NativeAllocator& right)
	{
		left.swap(right);
	}

}

#endif
