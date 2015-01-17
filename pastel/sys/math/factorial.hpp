#ifndef PASTELSYS_FACTORIAL_HPP
#define PASTELSYS_FACTORIAL_HPP

#include "pastel/sys/math/factorial.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	namespace Factorial_
	{

		static PASTEL_CONSTEXPR integer RealTableSize = 257;
		static PASTEL_CONSTEXPR integer IntegerTableSize = 21;

		//! Returns the logarithm of the factorial.
		/*!
		Preconditions:
		i >= 0
		*/
		inline real64 lnFactorialReal64(integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, RealTableSize);

			// lnFactorial_[n] = ln(n!) = sum{i = 1}^n ln(i)
			// where ln is the natural logarithm.
			// Computed with Maple to 16 digits.
			
			real64 lnFactorial_[RealTableSize] = 
			{
				0,
				0., .6931471805599453, 1.791759469228055, 3.178053830347946, 
				4.787491742782046, 6.579251212010101, 8.525161361065414, 10.60460290274525, 
				12.80182748008147, 15.10441257307552, 17.50230784587389, 19.98721449566189, 
				22.55216385312342, 25.19122118273868, 27.89927138384089, 30.67186010608067, 
				33.50507345013689, 36.39544520803305, 39.33988418719949, 42.33561646075348, 
				45.38013889847691, 48.47118135183522, 51.60667556776437, 54.78472939811232, 
				58.00360522298052, 61.26170176100200, 64.55753862700633, 67.88974313718154, 
				71.25703896716801, 74.65823634883016, 78.09222355331531, 81.55795945611504, 
				85.05446701758152, 88.58082754219768, 92.13617560368709, 95.71969454214320, 
				99.33061245478743, 102.9681986145138, 106.6317602606435, 110.3206397147574, 
				114.0342117814617, 117.7718813997451, 121.5330815154386, 125.3172711493569, 
				129.1239336391272, 132.9525750356163, 136.8027226373264, 140.6739236482343, 
				144.5657439463449, 148.4777669517730, 152.4095925844974, 156.3608363030788, 
				160.3311282166309, 164.3201122631952, 168.3274454484277, 172.3527971391628, 
				176.3958484069974, 180.4562914175438, 184.5338288614495, 188.6281734236716, 
				192.7390472878449, 196.8661816728900, 201.0093163992815, 205.1681994826412, 
				209.3425867525368, 213.5322414945633, 217.7369341139542, 221.9564418191303, 
				226.1905483237276, 230.4390435657770, 234.7017234428183, 238.9783895618343, 
				243.2688490029827, 247.5729140961869, 251.8904022097232, 256.2211355500095, 
				260.5649409718632, 264.9216497985528, 269.2910976510198, 273.6731242856937, 
				278.0675734403661, 282.4742926876304, 286.8931332954270, 291.3239500942703, 
				295.7666013507606, 300.2209486470141, 304.6868567656687, 309.1641935801469, 
				313.6528299498791, 318.1526396202093, 322.6634991267262, 327.1852877037752, 
				331.7178871969285, 336.2611819791985, 340.8150588707990, 345.3794070622669, 
				349.9541180407702, 354.5390855194408, 359.1342053695754, 363.7393755555635, 
				368.3544960724047, 372.9794688856890, 377.6141978739187, 382.2585887730600, 
				386.9125491232176, 391.5759882173296, 396.2488170517915, 400.9309482789157, 
				405.6222961611449, 410.3227765269373, 415.0323067282496, 419.7508055995447, 
				424.4781934182571, 429.2143918666516, 433.9593239950148, 438.7129141861212, 
				443.4750881209189, 448.2457727453846, 453.0248962384961, 457.8123879812782, 
				462.6081785268749, 467.4121995716082, 472.2243839269806, 477.0446654925856, 
				481.8729792298879, 486.7092611368394, 491.5534482232980, 496.4054784872176, 
				501.2652908915793, 506.1328253420349, 511.0080226652360, 515.8908245878224, 
				520.7811737160442, 525.6790135159951, 530.5842882944335, 535.4969431801695, 
				540.4169241059977, 545.3441777911549, 550.2786517242856, 555.2202941468949, 
				560.1690540372730, 565.1248810948743, 570.0877257251342, 575.0575390247102, 
				580.0342727671308, 585.0178793888391, 590.0083119756179, 595.0055242493820, 
				600.0094705553274, 605.0201058494237, 610.0373856862386, 615.0612662070849, 
				620.0917041284773, 625.1286567308909, 630.1720818478102, 635.2219378550597, 
				640.2781836604080, 645.3407786934350, 650.4096828956552, 655.4848567108891, 
				660.5662610758735, 665.6538574111059, 670.7476076119127, 675.8474740397369, 
				680.9534195136375, 686.0654073019940, 691.1834011144108, 696.3073650938140, 
				701.4372638087371, 706.5730622457873, 711.7147258022900, 716.8622202791035, 
				722.0155118736012, 727.1745671728158, 732.3393531467393, 737.5098371417774, 
				742.6859868743513, 747.8677704246433, 753.0551562304841, 758.2481130813743, 
				763.4466101126401, 768.6506167997169, 773.8601029525584, 779.0750387101673, 
				784.2953945352457, 789.5211412089589, 794.7522498258135, 799.9886917886434, 
				805.2304388037030, 810.4774628758635, 815.7297363039102, 820.9872316759379, 
				826.2499218648428, 831.5177800239062, 836.7907795824699, 842.0688942417004, 
				847.3520979704384, 852.6403650011329, 857.9336698258574, 863.2319871924055, 
				868.5352921004645, 873.8435597978658, 879.1567657769075, 884.4748857707518, 
				889.7978957498902, 895.1257719186797, 900.4584907119451, 905.7960287916464, 
				911.1383630436112, 916.4854705743287, 921.8373287078048, 927.1939149824768, 
				932.5552071481862, 937.9211831632081, 943.2918211913357, 948.6670995990199, 
				954.0469969525604, 959.4314920153494, 964.8205637451659, 970.2141912915183, 
				975.6123539930361, 981.0150313749083, 986.4222031463685, 991.8338491982235, 
				997.2499496004279, 1002.670484599700, 1008.095434617182, 1013.524780246136, 
				1018.958502249690, 1024.396581558613, 1029.838999269135, 1035.285736640802, 
				1040.736775094367, 1046.192096209725, 1051.651681723869, 1057.115513528895, 
				1062.583573670030, 1068.055844343701, 1073.532307895633, 1079.012946818975, 
				1084.497743752466, 1089.986681478622, 1095.479742921963, 1100.976911147256, 
				1106.478169357801, 1111.983500893733, 1117.492889230361, 1123.006317976526, 
				1128.523770872991, 1134.045231790853, 1139.570684729985, 1145.100113817496, 
				1150.633503306224, 1156.170837573242, 1161.712101118401, 1167.257278562880
			};

			return lnFactorial_[i];
		}

		//! Returns the factorial.
		/*!
		Preconditions:
		i <= 0 < 21

		It holds that 20! <= 2^64 - 1 < 21!.
		*/
		inline uint64 factorialUint64(integer i)
		{
			// factorial_[n] = n!
			// Computed with Mathematica.
			// It holds that 20! <= 2^64 - 1 < 21!.
			uint64 factorialSet[IntegerTableSize] =
			{
				1, 
				1, 
				2, 
				6, 
				24, 
				120, 
				720, 
				5040, 
				40320, 
				362880, 
				3628800, 
				39916800,
				479001600, 
				6227020800, 
				87178291200, 
				1307674368000, 
				20922789888000,
				355687428096000, 
				6402373705728000, 
				121645100408832000,
				2432902008176640000
			};

			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, IntegerTableSize);

			return factorialSet[i];
		}

	}

	template <typename Real>
	Real factorial(integer i)
	{
		PENSURE_OP(i, >=, 0);
		if (i < Factorial_::IntegerTableSize)
		{
			// For small parameters, we look the answer
			// up from a table.
			return Factorial_::factorialUint64(i);
		}

		// For large parameters, we return the value
		// of the gamma function.
		return gamma<Real>(i + 1);
	}

	template <typename Real>
	Real lnFactorial(integer i)
	{
		PENSURE_OP(i, >=, 0);

		if (i < Factorial_::RealTableSize)
		{
			// For small parameters, we look the answer
			// up from a table.
			return Factorial_::lnFactorialReal64(i);
		}

		// For large parameters, we return the value
		// of the log-gamma function.
		return lnGamma<Real>(i + 1);
	}

}

#endif