#include "pastel/math/mathcommon.h"
#include "pastel/sys/syscommon.h"

namespace Pastel
{

	namespace
	{

		// The harmonic numbers from H_0 to H_256.
		// H_0 = 0
		// H_n = 1/1 + 1/2 + 1/3 + ... + 1/n
		// Computed with Maple, accurate to 16 decimals.

		real harmonicNumber_[257] = 
		{
			0,
			1.000000000000000, 1.500000000000000, 1.833333333333333, 2.083333333333333, 
			2.283333333333333, 2.450000000000000, 2.592857142857143, 2.717857142857143, 
			2.828968253968254, 2.928968253968254, 3.019877344877345, 3.103210678210678, 
			3.180133755133755, 3.251562326562327, 3.318228993228993, 3.380728993228993, 
			3.439552522640758, 3.495108078196313, 3.547739657143682, 3.597739657143682, 
			3.645358704762730, 3.690813250217275, 3.734291511086840, 3.775958177753507, 
			3.815958177753507, 3.854419716215045, 3.891456753252082, 3.927171038966368, 
			3.961653797587058, 3.994987130920391, 4.027245195436520, 4.058495195436520, 
			4.088798225739550, 4.118209990445433, 4.146781419016861, 4.174559196794639, 
			4.201586223821666, 4.227902013295350, 4.253543038936376, 4.278543038936376, 
			4.302933282838815, 4.326742806648339, 4.349998620601827, 4.372725893329100, 
			4.394948115551322, 4.416687245986105, 4.437963841730786, 4.458797175064119, 
			4.479205338329425, 4.499205338329425, 4.518813181466680, 4.538043950697449, 
			4.556911875225751, 4.575430393744270, 4.593612211926088, 4.611469354783231, 
			4.629013214432353, 4.646254593742698, 4.663203746285071, 4.679870412951738, 
			4.696263855574689, 4.712392887832753, 4.728265903705769, 4.743890903705769, 
			4.759275519090384, 4.774427034241900, 4.789352407376228, 4.804058289729169, 
			4.818551043352358, 4.832836757638072, 4.846921264680325, 4.860810153569214, 
			4.874508783706200, 4.888022297219714, 4.901355630553047, 4.914513525289889, 
			4.927500538276902, 4.940321051097415, 4.952979278945517, 4.965479278945517, 
			4.977824957957862, 4.990020079909082, 5.002068272680166, 5.013973034584928, 
			5.025737740467281, 5.037365647444025, 5.048859900317588, 5.060223536681225, 
			5.071459491737404, 5.082570602848516, 5.093559613837527, 5.104429179054918, 
			5.115181867226961, 5.125820165099301, 5.136346480888775, 5.146763147555442, 
			5.157072425905957, 5.167276507538610, 5.177377517639620, 5.187377517639620, 
			5.197278507738630, 5.207082429307258, 5.216791167171335, 5.226406551786720, 
			5.235930361310529, 5.245364323574680, 5.254710117967204, 5.263969377226463, 
			5.273143689153068, 5.282234598243978, 5.291243607252987, 5.300172178681558, 
			5.309021736203682, 5.317793666028243, 5.326489318202156, 5.335110007857329, 
			5.343657016404337, 5.352131592675524, 5.360534954020062, 5.368868287353395, 
			5.377132750163312, 5.385329471474788, 5.393459552775601, 5.401524068904633, 
			5.409524068904633, 5.417460576841141, 5.425334592589172, 5.433147092589172, 
			5.440899030573668, 5.448591338265976, 5.456224926052236, 5.463800683627993, 
			5.471319480620475, 5.478782167187639, 5.486189574595046, 5.493542515771517, 
			5.500841785844509, 5.508088162656104, 5.515282407260420, 5.522425264403277, 
			5.529517462984838, 5.536559716505964, 5.543552723498971, 5.550497167943416, 
			5.557393719667554, 5.564243034736047, 5.571045755824482, 5.577802512581239, 
			5.584513921977212, 5.591180588643879, 5.597803105200170, 5.604382052568591, 
			5.610918000281010, 5.617411506774516, 5.623863119677742, 5.630273376087998, 
			5.636642802839591, 5.642971916763641, 5.649261224939742, 5.655511224939742, 
			5.661722405063965, 5.667895244570138, 5.674030213895292, 5.680127774870901, 
			5.686188380931508, 5.692212477317050, 5.698200501269145, 5.704152882221526, 
			5.710070041984840, 5.715952394926017, 5.721800348142391, 5.727614301630763, 
			5.733394648451572, 5.739141774888354, 5.744856060602639, 5.750537878784458, 
			5.756187596298582, 5.761805573826672, 5.767392166005443, 5.772947721560998, 
			5.778472583439451, 5.783967088933957, 5.789431569808274, 5.794866352416969, 
			5.800271757822375, 5.805648101908396, 5.810995695491284, 5.816314844427454, 
			5.821605849718460, 5.826869007613196, 5.832104609707437, 5.837312943040771, 
			5.842494290191030, 5.847648929366287, 5.852777134494492, 5.857879175310819, 
			5.862955317442799, 5.868005822493304, 5.873030948121444, 5.878030948121444, 
			5.883006072499554, 5.887956567549059, 5.892882675923443, 5.897784636707757, 
			5.902662685488245, 5.907517054420283, 5.912347972294680, 5.917155664602372, 
			5.921940353597587, 5.926702258359492, 5.931441594852383, 5.936158575984458, 
			5.940853411665210, 5.945526308861471, 5.950177471652169, 5.954807101281799, 
			5.959415396212674, 5.964002552175977, 5.968568762221639, 5.973114216767094, 
			5.977639103644922, 5.982143608149426, 5.986627913082162, 5.991092198796447,
			5.995536643240892, 5.999961422001954, 6.004366708345566, 6.008752673257847, 
			6.013119485484921, 6.017467311571877, 6.021796315900882, 6.026106660728468, 
			6.030398506222030, 6.034672010495535, 6.038927329644471, 6.043164617780064, 
			6.047384027062764, 6.051585707735033, 6.055769808153443, 6.059936474820110, 
			6.064085852413471, 6.068218083818430, 6.072333310155878, 6.076431670811616, 
			6.080513303464677, 6.084578344115084, 6.088626927111035, 6.092659185175551, 
			6.096675249432579, 6.100675249432579, 6.104659313177599, 6.108627567145853, 
			6.112580136315814, 6.116517144189829, 6.120438712817280, 6.124344962817280
		};

	}

	PASTELMATH real harmonicNumber(integer n)
	{
		// The n:th harmonic number H_n is given by:
		// H_0 = 0
		// H_n = 1/1 + 1/2 + 1/3 + ... + 1/n
		//
		// From Wikipedia:
		// The harmonic number can be approximated
		// with:
		// H_n = y + ln(n) + (1/2) n^-1 - (1/12) n^-2 + 
		//       (1/120) n^-4 + O(n^-6)
		// where y is Euler-Mascheroni constant.
		// 
		// Comparing the practical error of this formula with Maple,
		// It seems that that there is non-negligible
		// error in the beginning of this sequence, beginning with:
		// -0.0022156649015329, -0.0000503454614782, -0.000004908302153, ...
		// Therefore we have computed the harmonic numbers
		// from 1 to 256 accurate to 16 decimals.
		// After this we use the approximation,
		// which is very accurate after this initial range.

		ENSURE1(n >= 0, n);

		if (n < 257)
		{
			return harmonicNumber_[n];
		}
		
		const real nInv = (real)1 / n;
		const real nInv2 = square(nInv);

		// Add in increasing order for accuracy.

		real result = ((real)1 / 120) * square(nInv2);
		result -= ((real)1 / 12) * nInv2;
		result += ((real)1 / 2) * nInv;
		result += constantEulerMascheroni<real>();
		result += std::log((real)n);

		return result;
	}

}
