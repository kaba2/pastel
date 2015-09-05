// Description: Testing for sphere area
// DocumentationOf: sphere_area.h

#include "test/test_init.h"

#include <pastel/geometry/area/sphere_area.h>

TEST_CASE("sphere_area (sphere_area)")
{
	// The surface area of a Euclidean unit n-ball,
	// computed with Mathematica to 16 digits.
	real correctSet[] =
	{
		0,
		0, 1.837877066409345, 2.531024246969291,
		2.982606952258746, 3.270289024710527, 3.434189657548201, 
		3.498728178685772, 3.480307254729491, 3.390695096039804, 
		3.238742779459001, 3.031347585112930, 2.774034752874300, 
		2.471329378723905, 2.127005169495646, 1.744257087671714, 
		1.325824906289732, 0.8740839529788491, 0.3911132504592966, 
		-0.1212523246680215, -0.6613814410275226, -1.227814237425116, 
		-1.819236648172168, -2.434459608739194, -3.072402035121138, 
		-3.732076758258998, -4.412578799059739, -5.113075516717853, 
		-5.832798270671875, -6.571035316312837, -7.327125714437734, 
		-8.100454079889966, -8.890446029690543, -9.696564217965766, 
		-10.51830486608092, -11.35519471302290, -12.20678832428774, 
		-13.07266570810297, -13.95243019633450, -14.84570655433785, 
		-15.75213928965155, -16.67139113405815, -17.60314167735614, 
		-18.54708613435311, -19.50293422923016, -20.47040918363733, 
		-21.44924679673907, -22.43919460699830, -23.44001112681882, 
		-24.45146514229902, -25.47333507131737, -26.50540837400030, 
		-27.54748101033617, -28.59935694031528, -29.66084766250825, 
		-30.73177178745805, -31.81195464266318, -32.90122790628118, 
		-33.99942926698898, -35.10640210770638, -36.22199521112606, 
		-37.34606248520276, -38.47846270693881, -39.61905928296672, 
		-40.76772002557456, -41.92431694294891, -43.08872604252489, 
		-44.26082714643520, -45.44050371814197, -46.62764269941682, 
		-47.82213435690873, -49.02387213760474, -50.23275253254874, 
		-51.44867494823671, -52.67154158515545, -53.90125732297575, 
		-55.13772961195028, -56.38086837010272, -57.63058588582726, 
		-58.88679672554706, -60.14941764610751, -61.41836751160473, 
		-62.69356721437204, -63.97493959986782, -65.26240939522695, 
		-66.55590314125508, -67.85534912766092, -69.16067733133605, 
		-70.47181935750508, -71.78870838358129, -73.11127910557394, 
		-74.43946768690408, -75.77321170949486, -77.11245012701159, 
		-78.45712322013456, -79.80717255375550, -81.16254093599521, 
		-82.52317237894669, -83.88901206105371, -85.26000629104073, 
		-86.63610247331493, -88.01724907476597, -89.40339559289368, 
		-90.79449252519789, -92.19049133976860, -93.59134444701818, 
		-94.99700517250063, -96.40742773076636, -97.82256720020335, 
		-99.24237949881892, -100.6668213609182, -102.0958503146387, 
		-103.5294246603013, -104.9675034495417, -106.4100464651870, 
		-107.8570142018447, -109.3083678471722, -110.7640692637986, 
		-112.2240809718692, -113.6883661321870, -115.1568885299255, 
		-116.6296125588892, -118.1065032062982, -119.5875260380766, 
		-121.0726471846221, -122.5618333270397, -124.0550516838178, 
		-125.5522699979326, -127.0534565243600, -128.5585800179819, 
		-130.0676097218702, -131.5805153559342, -133.0972671059165, 
		-134.6178356127260, -136.1421919620935, -137.6703076745384, 
		-139.2021546956351, -140.7377053865675, -142.2769325149618, 
		-143.8198092459863, -145.3663091337096, -146.9164061127076, 
		-148.4700744899096, -150.0272889366764, -151.5880244811015, 
		-153.1522565005270, -154.7199607142682, -156.2911131765382, 
		-157.8656902695672, -159.4436686969076, -161.0250254769219, 
		-162.6097379364437, -164.1977837046088, -165.7891407068493, 
		-167.3837871590458, -168.9817015618324, -170.5828626950501, 
		-172.1872496123423, -173.7948416358902, -175.4056183512813, 
		-177.0195596025079, -178.6366454870922, -180.2568563513323, 
		-181.8801727856673, -183.5065756201554, -185.1360459200647, 
		-186.7685649815702, -188.4041143275559, -190.0426757035174, 
		-191.6842310735633, -193.3287626165113, -194.9762527220771, 
		-196.6266839871523, -198.2800392121704, -199.9363013975564, 
		-201.5954537402588, -203.2574796303616, -204.9223626477730, 
		-206.5900865589904, -208.2606353139375, -209.9339930428731, 
		-211.6101440533689, -213.2890728273540, -214.9707640182254, 
		-216.6552024480214, -218.3423731046574, -220.0322611392211, 
		-221.7248518633264, -223.4201307465249, -225.1180834137716, 
		-226.8186956429455, -228.5219533624219, -230.2278426486967, 
		-231.9363497240592, -233.6474609543151, -235.3611628465548, 
		-237.0774420469691, -238.7962853387092, -240.5176796397902, 
		-242.2416120010378, -243.9680696040754, -245.6970397593530, 
		-247.4285099042141, -249.1624676010027, -250.8989005352060, 
		-252.6377965136351, -254.3791434626409, -256.1229294263642, 
		-257.8691425650211, -259.6177711532202, -261.3688035783131, 
		-263.1222283387757, -264.8780340426212, -266.6362094058424, 
		-268.3967432508839, -270.1596245051425, -271.9248421994964, 
		-273.6923854668608, -275.4622435407712, -277.2344057539919, 
		-279.0088615371509, -280.7856004173991, -282.5646120170939, 
		-284.3458860525075, -286.1294123325569, -287.9151807575583, 
		-289.7031813180026, -291.4934040933533, -293.2858392508655, 
		-295.0804770444254, -296.8773078134106, -298.6763219815703, 
		-300.4775100559245, -302.2808626256827, -304.0863703611814, 
		-305.8940240128391, -307.7038144101298, -309.5157324605739, 
		-311.3297691487460, -313.1459155352997, -314.9641627560082, 
		-316.7845020208218, -318.6069246129408, -320.4314218879032, 
		-322.2579852726882, -324.0866062648344, -325.9172764315720, 
		-327.7499874089697, -329.5847309010950, -331.4214986791884, 
		-333.2602825808507, -335.1010745092437, -336.9438664323036, 
		-338.7886503819662, -340.6354184534056, -342.4841628042843, 
		-344.3348756540148
	};

	REQUIRE(areaUnitSphere<real>(1) == 0);
	REQUIRE(lnAreaUnitSphere<real>(1) == -(real)Infinity());

	integer elements = sizeof(correctSet) / sizeof(real);
	for (integer n = 2; n < elements; ++n)
	{
		real correct = correctSet[n];

		{
			real maxError = 4e-14;
			real measured = lnAreaUnitSphere<real>(n);
			real error = relativeError<real>(measured, correct);
			REQUIRE(error < maxError);
		}

		{
			real maxError = 4e-14;
			real measured = std::log(areaUnitSphere<real>(n));
			real error = relativeError<real>(measured, correct);
			REQUIRE(error < maxError);
		}
	}
}

