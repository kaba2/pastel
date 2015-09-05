// Description: Testing for sphere volume
// DocumentationOf: sphere_volume.h

#include "test/test_init.h"

#include <pastel/geometry/volume/sphere_volume.h>
#include <pastel/sys/math_functions.h>

TEST_CASE("Euclidean (sphere_volume)")
{
	// Natural logarithms of the volumes of hyperspheres 
	// in dimensions 0 -> 256. Computed with Maple to 16 digits.
	real correctSet[] =
	{
		0,
		.6931471805599453, 1.144729885849400, 1.432411958301181, 1.596312591138855,
		1.660851112276426, 1.642430188320145, 1.552818029630458, 1.400865713049655,
		1.193470518703585, .9361576864649543, .6334523123145588, .2891281030862997,
		-0.9361997873763275e-1, -.5120521601196137, -.9637931134304974, -1.446763815950049,
		-1.959129391077368, -2.499258507436869, -3.065691303834463, -3.657113714581514,
		-4.272336675148540, -4.910279101530485, -5.569953824668344, -6.250455865469085,
		-6.950952583127200, -7.670675337081221, -8.408912382722183, -9.165002780847080,
		-9.938331146299312, -10.72832309609989, -11.53444128437511, -12.35618193249027,
		-13.19307177943225, -14.04466539069709, -14.91054277451232, -15.79030726274385,
		-16.68358362074719, -17.59001635606089, -18.50926820046750, -19.44101874376548,
		-20.38496320076246, -21.34081129563951, -22.30828625004668, -23.28712386314842,
		-24.27707167340765, -25.27788819322817, -26.28934220870836, -27.31121213772672,
		-28.34328544040964, -29.38535807674552, -30.43723400672463, -31.49872472891760,
		-32.56964885386740, -33.64983170907253, -34.73910497269053, -35.83730633339833,
		-36.94427917411573, -38.05987227753541, -39.18393955161211, -40.31633977334816,
		-41.45693634937607, -42.60559709198391, -43.76219400935826, -44.92660310893423,
		-46.09870421284455, -47.27838078455131, -48.46551976582617, -49.66001142331808,
		-50.86174920401408, -52.07062959895809, -53.28655201464605, -54.50941865156480,
		-55.73913438938510, -56.97560667835962, -58.21874543651207, -59.46846295223661,
		-60.72467379195640, -61.98729471251686, -63.25624457801408, -64.53144428078139,
		-65.81281666627717, -67.10028646163630, -68.39378020766443, -69.69322619407027,
		-70.99855439774540, -72.30969642391443, -73.62658544999064, -74.94915617198329,
		-76.27734475331343, -77.61108877590421, -78.95032719342093, -80.29500028654390,
		-81.64504962016484, -83.00041800240456, -84.36104944535604, -85.72688912746305,
		-87.09788335745008, -88.47397953972428, -89.85512614117532, -91.24127265930303,
		-92.63236959160724, -94.02836840617795, -95.42922151342753, -96.83488223890998,
		-98.24530479717570, -99.66044426661270, -101.0802565652283, -102.5046984273276,
		-103.9337273810481, -105.3673017267106, -106.8053805159511, -108.2479235315964,
		-109.6948912682540, -111.1462449135815, -112.6019463302080, -114.0619580382786,
		-115.5262431985964, -116.9947655963349, -118.4674896252985, -119.9443802727076,
		-121.4254031044859, -122.9105242510315, -124.3997103934490, -125.8929287502272,
		-127.3901470643420, -128.8913335907693, -130.3964570843912, -131.9054867882796,
		-133.4183924223435, -134.9351441723258, -136.4557126791353, -137.9800690285029,
		-139.5081847409478, -141.0400317620444, -142.5755824529768, -144.1148095813711,
		-145.6576863123956, -147.2041862001190, -148.7542831791170, -150.3079515563189,
		-151.8651660030858, -153.4259015475109, -154.9901335669364, -156.5578377806775,
		-158.1289902429476, -159.7035673359765, -161.2815457633170, -162.8629025433313,
		-164.4476150028531, -166.0356607710182, -167.6270177732587, -169.2216642254551,
		-170.8195786282418, -172.4207397614594, -174.0251266787517, -175.6327187022996,
		-177.2434954176906, -178.8574366689172, -180.4745225535015, -182.0947334177417,
		-183.7180498520766, -185.3444526865647, -186.9739229864740, -188.6064420479796,
		-190.2419913939653, -191.8805527699268, -193.5221081399727, -195.1666396829207,
		-196.8141297884864, -198.4645610535616, -200.1179162785797, -201.7741784639657,
		-203.4333308066682, -205.0953566967709, -206.7602397141823, -208.4279636253997,
		-210.0985123803468, -211.7718701092825, -213.4480211197782, -215.1269498937633,
		-216.8086410846347, -218.4930795144308, -220.1802501710668, -221.8701382056304,
		-223.5627289297358, -225.2580078129343, -226.9559604801810, -228.6565727093549,
		-230.3598304288313, -232.0657197151060, -233.7742267904686, -235.4853380207244,
		-237.1990399129641, -238.9153191133784, -240.6341624051185, -242.3555567061996,
		-244.0794890674472, -245.8059466704848, -247.5349168257623, -249.2663869706235,
		-251.0003446674120, -252.7367776016153, -254.4756735800445, -256.2170205290502,
		-257.9608064927735, -259.7070196314304, -261.4556482196296, -263.2066806447224,
		-264.9601054051850, -266.7159111090305, -268.4740864722518, -270.2346203172932,
		-271.9975015715518, -273.7627192659057, -275.5302625332701, -277.3001206071805,
		-279.0722828204013, -280.8467386035603, -282.6234774838084, -284.4024890835033,
		-286.1837631189168, -287.9672893989662, -289.7530578239676, -291.5410583844119,
		-293.3312811597627, -295.1237163172749, -296.9183541108347, -298.7151848798200,
		-300.5141990479796, -302.3153871223338, -304.1187396920921, -305.9242474275908,
		-307.7319010792484, -309.5416914765391, -311.3536095269832, -313.1676462151554,
		-314.9837926017090, -316.8020398224175, -318.6223790872312, -320.4448016793502,
		-322.2692989543125, -324.0958623390975, -325.9244833312437, -327.7551534979814,
		-329.5878644753791, -331.4226079675044, -333.2593757455977, -335.0981596472600,
		-336.9389515756531, -338.7817434987129, -340.6265274483755, -342.4732955198150,
		-344.3220398706937, -346.1727527204242, -348.0254263494428, -349.8800530984944
	};

	integer elements = sizeof(correctSet) / sizeof(real);
	for (integer n = 1; n < elements; ++n)
	{
		real correct = correctSet[n];

		{
			real maxError = 8e-15;
			real measured = lnVolumeUnitSphere<real>(n);
			real error = relativeError<real>(measured, correct);
			REQUIRE(error < maxError);
		}

		{
			real maxError = 2e-14;
			real measured = std::log(volumeUnitSphere<real>(n));
			real error = relativeError<real>(measured, correct);
			REQUIRE(error < maxError);
		}
	}
}

TEST_CASE("Manhattan (sphere_volume)")
{
	// The logarithm of the volume of an n-dimensional Manhattan 
	// unit sphere, computed with Mathematica to 16 digits.
	real correctSet[] =
	{
		0,
		0.6931471805599453, 0.6931471805599453, 0.2876820724517809,
		-0.4054651081081644, -1.321755839982319, -2.420368128650429,
		-3.673131097145797, -5.059425458265688, -6.563502855041962,
		-8.172940767476062, -9.877688859714487, -11.66944832894254,
		-13.54125050584413, -15.48716065489945, -17.50206367544171,
		-19.58150521712155, -21.72157138061782, -23.91879595795404,
		-26.17008775656053, -28.47267284955458, -30.82404810671806,
		-33.22194337951643, -35.66429041488563, -38.14919706467363,
		-40.67492570898189, -43.23987506644342, -45.84256475188781,
		-48.48162208150307, -51.15577073092960, -53.86382093203181,
		-56.60466095595701, -59.37724967819679, -62.18061005910332,
		-65.01382340315954, -67.87602428408901, -70.76639604198517,
		-73.68416677406945, -76.62860575323589, -79.59902021880559,
		-82.59475249235958, -85.61517737850395, -88.65969981622737,
		-91.72775275136099, -94.81879520471930, -97.93231051392968,
		-101.0678047298588, -104.2248051510089, -107.4028589813569,
		-110.6015320989076, -113.8204079237758, -117.0590863759401,
		-120.3171829139616, -123.5943276469538, -126.8901645129581,
		-130.2043505176307, -133.5365550278059, -136.8864591150805,
		-140.2537549450669, -143.6381452084127, -147.0393425900749,
		-150.4570692736882, -153.8910564781734, -157.3410440240050,
		-160.8067799268047, -164.2880200161404, -167.7845275776069,
		-171.2960730164379, -174.8224335410541, -178.3633928650914,
		-181.9187409265808, -185.4882736230622, -189.0717925615183,
		-192.6691048221067, -196.2800227347509, -199.9043636677273,
		-203.5419498274537, -207.1926080687474, -210.8561697148771,
		-214.5324703867841, -218.2213498408981, -221.9226518150106,
		-225.6362238817149, -229.3619173089515, -233.0995869272349,
		-236.8490910031653, -240.6102911188588, -244.3830520569535,
		-248.1672416908717, -251.9627308800439, -255.7693933698142,
		-259.5871056957712, -263.4157470922602, -267.2551994048536,
		-271.1053470065636, -274.9660767176042, -278.8372777285121,
		-282.7188415264555, -286.6106618245662, -290.5126344941408,
		-294.4246574995690, -298.3466308358503, -302.2784564685746,
		-306.2200382762443, -310.1712819948257, -314.1320951644233,
		-318.1023870779754, -322.0820687318774, -326.0710527784417,
		-330.0692534801109, -334.0765866653433, -338.0929696860957,
		-342.1183213768309, -346.1525620149833, -350.1956132828178,
		-354.2473982306211, -358.3078412411675, -362.3768679954053,
		-366.4544054393111, -370.5403817518626, -374.6347263140847,
		-378.7373696791215, -382.8482435432949, -386.9672807181073,
		-391.0944151031524, -395.2295816598948, -399.3727163862863,
		-403.5237562921849, -407.6826393755446, -411.8493045993463,
		-416.0236918692420, -420.2057420118832, -424.3953967539096,
		-428.5925987015714, -432.7972913209624, -437.0094189188409,
		-441.2289266240170, -445.4557603692852, -449.6898668738824,
		-453.9311936264532, -458.1796888685025, -462.4353015783207,
		-466.6979814553621, -470.9676789050620, -475.2443450240781,
		-479.5279315859387, -483.8183910270871, -488.1156764333059,
		-492.4197415265101, -496.7305406518956, -501.0480287654319,
		-505.3721614216869, -509.7028947619732, -514.0401855028057,
		-518.3839909246594, -522.7342688610187, -527.0909776877083,
		-531.4540763124966, -535.8235241649636, -540.1992811866239,
		-544.5813078212978, -548.9695650057223, -553.3640141603948,
		-557.7646171806416, -562.1713364279058, -566.5841347212465,
		-571.0029753290431, -575.4278219608999, -579.8586387597432,
		-584.2953902941063, -588.7380415505966, -593.1865579265394,
		-597.6409052227929, -602.1010496367307, -606.5669577553853,
		-611.0385965487489, -615.5159333632271, -619.9989359152409,
		-624.4875722849731, -628.9818109102539, -633.4816205805842,
		-637.9869704312900, -642.4978299378069, -647.0141689100884,
		-651.5359574871374, -656.0631661316558, -660.5957656248090,
		-665.1337270611037, -669.6770218433737, -674.2256216778734,
		-678.7794985694739, -683.3386248169606, -687.9029730084284,
		-692.4725160167734, -697.0472269952768, -701.6270793732806,
		-706.2120468519511, -710.8021034001292, -715.3972232502638,
		-719.9973808944283, -724.6025510804164, -729.2127088079155,
		-733.8278293247568, -738.4478881232386, -743.0728609365229,
		-747.7027237351014, -752.3374527233310, -756.9770243360364,
		-761.6214152351778, -766.2706023065827, -770.9245626567402,
		-775.5832736096563, -780.2467127037684, -784.9148576889179,
		-789.5876865233798, -794.2651773709475, -798.9473085980717,
		-803.6340587710522, -808.3254066532814, -813.0213312025379,
		-817.7218115683303, -822.4268270892881, -827.1363572906005,
		-831.8503818815007, -836.5688807527957, -841.2918339744402,
		-846.0192217931526, -850.7510246300740, -855.4872230784685,
		-860.2277979014628, -864.9727300298261, -869.7220005597879,
		-874.4755907508943, -879.2334820239000, -883.9956559586978,
		-888.7620942922820, -893.5327789167477, -898.3076918773229,
		-903.0868153704344, -907.8701317418059, -912.6576234845880,
		-917.4492732375187, -922.2450637831154, -927.0449780458960,
		-931.8489990906293, -936.6571101206141, -941.4692944759865,
		-946.2855356320545, -951.1058171976596, -955.9301229135643,
		-960.7584366508666, -965.5907424094385, -970.4270243163899,
		-975.2672666245575, -980.1114537110161, -984.9595700756146,
		-989.8116003395342
	};

	integer elements = sizeof(correctSet) / sizeof(real);
	for (integer n = 1; n < elements; ++n)
	{
		real correct = correctSet[n];

		{
			real maxError = 7e-15;
			real measured = lnVolumeUnitSphereManhattan<real>(n);
			real error = relativeError<real>(measured, correct);
			REQUIRE(error < maxError);
		}

		{
			real maxError = 7e-15;
			real measured = std::log(volumeUnitSphereManhattan<real>(n));
			if (measured > -(real)Infinity())
			{
				real error = relativeError<real>(measured, correct);
				REQUIRE(error < maxError);
			}
		}
	}
}
