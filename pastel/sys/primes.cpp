#include "pastel/sys/primes.h"

#include <algorithm>

namespace Pastel
{

	namespace
	{

		const unsigned long primes[256 + 48 + 1] =
		{

			2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul,
			31ul, 37ul, 41ul, 43ul, 47ul, 53ul, 59ul, 61ul, 67ul,
			71ul, 73ul, 79ul, 83ul, 89ul, 97ul, 103ul, 109ul, 113ul,
			127ul, 137ul, 139ul, 149ul, 157ul, 167ul, 179ul, 193ul,
			199ul, 211ul, 227ul, 241ul, 257ul, 277ul, 293ul, 313ul,
			337ul, 359ul, 383ul, 409ul, 439ul, 467ul, 503ul, 541ul,
			577ul, 619ul, 661ul, 709ul, 761ul, 823ul, 887ul, 953ul,
			1031ul, 1109ul, 1193ul, 1289ul, 1381ul, 1493ul, 1613ul,
			1741ul, 1879ul, 2029ul, 2179ul, 2357ul, 2549ul, 2753ul,
			2971ul, 3209ul, 3469ul, 3739ul, 4027ul, 4349ul, 4703ul,
			5087ul, 5503ul, 5953ul, 6427ul, 6949ul, 7517ul, 8123ul,
			8783ul, 9497ul, 10273ul, 11113ul, 12011ul, 12983ul,
			14033ul, 15173ul, 16411ul, 17749ul, 19183ul, 20753ul,
			22447ul, 24281ul, 26267ul, 28411ul, 30727ul, 33223ul,
			35933ul, 38873ul, 42043ul, 45481ul, 49201ul, 53201ul,
			57557ul, 62233ul, 67307ul, 72817ul, 78779ul, 85229ul,
			92203ul, 99733ul, 107897ul, 116731ul, 126271ul, 136607ul,
			147793ul, 159871ul, 172933ul, 187091ul, 202409ul,
			218971ul, 236897ul, 256279ul, 277261ul, 299951ul,
			324503ul, 351061ul, 379787ul, 410857ul, 444487ul,
			480881ul, 520241ul, 562841ul, 608903ul, 658753ul,
			712697ul, 771049ul, 834181ul, 902483ul, 976369ul,
			1056323ul, 1142821ul, 1236397ul, 1337629ul, 1447153ul,
			1565659ul, 1693859ul, 1832561ul, 1982627ul, 2144977ul,
			2320627ul, 2510653ul, 2716249ul, 2938679ul, 3179303ul,
			3439651ul, 3721303ul, 4026031ul, 4355707ul, 4712381ul,
			5098259ul, 5515729ul, 5967347ul, 6456007ul, 6984629ul,
			7556579ul, 8175383ul, 8844859ul, 9569143ul, 10352717ul,
			11200489ul, 12117689ul, 13109983ul, 14183539ul,
			15345007ul, 16601593ul, 17961079ul, 19431899ul,
			21023161ul, 22744717ul, 24607243ul, 26622317ul,
			28802401ul, 31160981ul, 33712729ul, 36473443ul,
			39460231ul, 42691603ul, 46187573ul, 49969847ul,
			54061849ul, 58488943ul, 63278561ul, 68460391ul,
			74066549ul, 80131819ul, 86693767ul, 93793069ul,
			101473717ul, 109783337ul, 118773397ul, 128499677ul,
			139022417ul, 150406843ul, 162723577ul, 176048909ul,
			190465427ul, 206062531ul, 222936881ul, 241193053ul,
			260944219ul, 282312799ul, 305431229ul, 330442829ul,
			357502601ul, 386778277ul, 418451333ul, 452718089ul,
			489790921ul, 529899637ul, 573292817ul, 620239453ul,
			671030513ul, 725980837ul, 785430967ul, 849749479ul,
			919334987ul, 994618837ul, 1076067617ul, 1164186217ul,
			1259520799ul, 1362662261ul, 1474249943ul, 1594975441ul,
			1725587117ul, 1866894511ul, 2019773507ul, 2185171673ul,
			2364114217ul, 2557710269ul, 2767159799ul, 2993761039ul,
			3238918481ul, 3504151727ul, 3791104843ul, 4101556399ul,
			4294967291ul,

			// Sentinel, so we don't have to test the result of
			// lower_bound, or, on 64-bit machines, rest of the
			// table.
			sizeof(ulong) != 8 ? 4294967291ul :
			(unsigned long)6442450933ull,
			(unsigned long)8589934583ull,
			(unsigned long)12884901857ull,
			(unsigned long)17179869143ull,
			(unsigned long)25769803693ull,
			(unsigned long)34359738337ull,
			(unsigned long)51539607367ull,
			(unsigned long)68719476731ull,
			(unsigned long)103079215087ull,
			(unsigned long)137438953447ull,
			(unsigned long)206158430123ull,
			(unsigned long)274877906899ull,
			(unsigned long)412316860387ull,
			(unsigned long)549755813881ull,
			(unsigned long)824633720731ull,
			(unsigned long)1099511627689ull,
			(unsigned long)1649267441579ull,
			(unsigned long)2199023255531ull,
			(unsigned long)3298534883309ull,
			(unsigned long)4398046511093ull,
			(unsigned long)6597069766607ull,
			(unsigned long)8796093022151ull,
			(unsigned long)13194139533241ull,
			(unsigned long)17592186044399ull,
			(unsigned long)26388279066581ull,
			(unsigned long)35184372088777ull,
			(unsigned long)52776558133177ull,
			(unsigned long)70368744177643ull,
			(unsigned long)105553116266399ull,
			(unsigned long)140737488355213ull,
			(unsigned long)211106232532861ull,
			(unsigned long)281474976710597ull,
			(unsigned long)562949953421231ull,
			(unsigned long)1125899906842597ull,
			(unsigned long)2251799813685119ull,
			(unsigned long)4503599627370449ull,
			(unsigned long)9007199254740881ull,
			(unsigned long)18014398509481951ull,
			(unsigned long)36028797018963913ull,
			(unsigned long)72057594037927931ull,
			(unsigned long)144115188075855859ull,
			(unsigned long)288230376151711717ull,
			(unsigned long)576460752303423433ull,
			(unsigned long)1152921504606846883ull,
			(unsigned long)2305843009213693951ull,
			(unsigned long)4611686018427387847ull,
			(unsigned long)9223372036854775783ull,
			(unsigned long)18446744073709551557ull,
			(unsigned long)18446744073709551557ull
		};

		const integer primeCount = sizeof(ulong) != 8 ? 256 : 256 + 48;
	}

	PASTELSYS ulong followingPrime(ulong minimum)
	{
		const ulong* pointer =
			std::lower_bound(primes, primes + primeCount, minimum);

		// TODO: Do something if no bigger prime can be found.
		return *pointer;
	}

}
