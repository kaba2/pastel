// Description: Sequence

#ifndef PASTELSYS_SEQUENCE_H
#define PASTELSYS_SEQUENCE_H

#include "pastel/sys/sequence_concepts.h"
#include "pastel/sys/sequence_node.h"
#include "pastel/sys/number_tests.h"
#include "pastel/sys/skiplist.h"

#include <boost/integer/static_log2.hpp>

#include <bitset>
#include <array>
#include <vector>
#include <unordered_map>

namespace Pastel
{

	class Default_Sequence_Settings
	{
	public:
		static const integer Bits = sizeof(integer) * 8;
		using Value = void;
	};

	//! Sequence
	/*!
	Preconditions:
	isPowerOfTwo(Bits).

	Bits:	
	The number of bits in a stored integer.

	References
	----------

	"Log-logarithmic Queries are Possible in Space Theta(Bits)",
	Dan E. Willard, Information Processing Letters 17 (1983),
	pp. 81-84.

	"Fast Local Searches and Updates in Bounded Universes",
	Prosenjit Bose et al., CCCG 2010 (2010).
	*/
	template <typename Sequence_Settings = Default_Sequence_Settings>
	class Sequence
	{
	private:
		using Settings = Sequence_Settings;

		enum
		{
			Bits = Settings::Bits
		};

		PASTEL_STATIC_ASSERT(Bits > 0);

		enum
		{
			NIsPowerOfTwo = (Bits & (Bits - 1) == 0)
		};

		// FIX: Replace with isPowerOfTwo constexpr
		// after constexpr becomes available in
		// Visual Studio.
		PASTEL_STATIC_ASSERT(NIsPowerOfTwo);
		// PASTEL_STATIC_ASSERT(isPowerOfTwo(Bits));

		//    B | log(B) | ceil(log(log(B)))
		// ---------------------------------
		//   16 | 4      | 3
		//   32 | 5      | 3
		//   64 | 6      | 3
		//  128 | 7      | 3
		//  256 | 8      | 3
		//  512 | 9      | 4
		enum
		{
			LogBits = boost::static_log2<Bits>::value,
			FloorLogLogBits = boost::static_log2<LogBits>::value,
			// ceil(log(log(Bits)))
			CeilLogLogBits = NIsPowerOfTwo ? 
				FloorLogLogBits : (FloorLogLogBits + 1)
		};

		// The levels to visit in the double-exponential search,
		// after level 0, are of the form 2^(2^i), up until to 
		// ceil(log(log(Bits))). For example, when Bits = 64, 
		// this means that the levels are 0, 2, 4, 16, 64.
		// The M gives the number of levels.
		enum
		{
			M = CeilLogLogBits + 2
		};

		using Value = typename Settings::Value;
		using Node = Sequence_::Node;
		using Leaf_Node = Sequence_::Leaf_Node<DataSet>;
		using Integer = Pastel::Integer<Bits>;

		using DataSet = SkipList_Map<Integer, Value>;
		using Iterator = typename DataSet::Iterator;
		using ConstIterator = typename DataSet::ConstIterator;

		using LevelSet = std::unordered_map<Integer, YFastTrie*>;
		using Level_Iterator = typename LevelSet::iterator;
		using Level_ConstIterator = typename LevelSet::const_iterator;
		using LevelSetSet = std::array<LevelSet, M>;

	public:

		//! Level-search structures.
		/*!
		The levelSetSet_[i] is a hash-table which allows to 
		efficiently find whether a suffix of the searched 
		key is stored in the sequence. These level-search
		structures cover M levels: 
		* the level 0, which compares the full keys, 
		stored in levelSetSet_[0]. The y-fast tries on this
		level are all trivial.
		* the levels 2^(2^i), for i in [0, M - 2], which
		compares the [2^(2^i), Bits) suffixes of the keys, 
		stored in levelSetSet_[i + 1], and
		* the level (Bits - 1), which compares empty suffixes
		of the keys, stored in levelSetSet_[M - 1]. This level
		has only a single element; it is the y-fast trie to
		all elements.
		*/
		LevelSetSet levelSetSet_;
	};

}

{

	template <integer Bits_, typename Value_>
	class Sequence_Map_Settings
	{
	public:
		static const integer Bits = Bits_;
		using Value = Value_;
	};

	template <integer Bits, typename Value>
	using Sequence_Map = Sequence<Sequence_Map_Settings<Bits, Value>>;

	template <integer Bits> 
	using Sequence_Set_Settings = Sequence_Map_Settings<Bits, void>;

	template <integer Bits>
	using Sequence_Set = Sequence<Sequence_Set_Settings<Bits>>;

}

#endif
