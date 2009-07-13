#include "pastel/sys/math_functions.h"

namespace Pastel
{

	namespace
	{

		// The digamma function evaluated at integer parameters
		// from 0 to 256. Computed with Maple, accurate to 16 decimals.

		real64 digamma_[257] = 
		{
			0,
			-.5772156649015329, .4227843350984671, .9227843350984671, 1.256117668431800, 
			1.506117668431800, 1.706117668431800, 1.872784335098467, 2.015641477955610, 
			2.140641477955610, 2.251752589066721, 2.351752589066721, 2.442661679975812, 
			2.525995013309145, 2.602918090232222, 2.674346661660794, 2.741013328327460, 
			2.803513328327460, 2.862336857739225, 2.917892413294780, 2.970523992242149, 
			3.020523992242149, 3.068143039861197, 3.113597585315742, 3.157075846185307, 
			3.198742512851974, 3.238742512851974, 3.277204051313512, 3.314241088350549, 
			3.349955374064835, 3.384438132685525, 3.417771466018858, 3.450029530534987, 
			3.481279530534987, 3.511582560838017, 3.540994325543900, 3.569565754115328, 
			3.597343531893106, 3.624370558920133, 3.650686348393817, 3.676327374034843, 
			3.701327374034843, 3.725717617937282, 3.749527141746806, 3.772782955700294, 
			3.795510228427567, 3.817732450649789, 3.839471581084572, 3.860748176829253, 
			3.881581510162586, 3.901989673427892, 3.921989673427892, 3.941597516565147, 
			3.960828285795916, 3.979696210324218, 3.998214728842737, 4.016396547024555, 
			4.034253689881698, 4.051797549530821, 4.069038928841165, 4.085988081383538, 
			4.102654748050205, 4.119048190673156, 4.135177222931220, 4.151050238804236, 
			4.166675238804236, 4.182059854188852, 4.197211369340367, 4.212136742474695, 
			4.226842624827636, 4.241335378450825, 4.255621092736539, 4.269705599778792, 
			4.283594488667681, 4.297293118804668, 4.310806632318181, 4.324139965651514, 
			4.337297860388357, 4.350284873375370, 4.363105386195882, 4.375763614043984, 
			4.388263614043984, 4.400609293056329, 4.412804415007549, 4.424852607778633, 
			4.436757369683395, 4.448522075565748, 4.460149982542492, 4.471644235416055, 
			4.483007871779692, 4.494243826835872, 4.505354937946983, 4.516343948935994, 
			4.527213514153385, 4.537966202325428, 4.548604500197768, 4.559130815987242, 
			4.569547482653909, 4.579856761004424, 4.590060842637077, 4.600161852738087, 
			4.610161852738087, 4.620062842837097, 4.629866764405725, 4.639575502269802, 
			4.649190886885187, 4.658714696408997, 4.668148658673148, 4.677494453065671, 
			4.686753712324930, 4.695928024251536, 4.705018933342445, 4.714027942351454, 
			4.722956513780025, 4.731806071302149, 4.740578001126710, 4.749273653300624, 
			4.757894342955796, 4.766441351502804, 4.774915927773991, 4.783319289118529, 
			4.791652622451862, 4.799917085261779, 4.808113806573255, 4.816243887874068, 
			4.824308404003100, 4.832308404003100, 4.840244911939608, 4.848118927687640, 
			4.855931427687640, 4.863683365672136, 4.871375673364443, 4.879009261150703, 
			4.886585018726460, 4.894103815718942, 4.901566502286106, 4.908973909693513, 
			4.916326850869984, 4.923626120942977, 4.930872497754571, 4.938066742358887, 
			4.945209599501744, 4.952301798083305, 4.959344051604431, 4.966337058597438, 
			4.973281503041883, 4.980178054766021, 4.987027369834514, 4.993830090922949, 
			5.000586847679706, 5.007298257075679, 5.013964923742346, 5.020587440298637, 
			5.027166387667058, 5.033702335379477, 5.040195841872983, 5.046647454776209, 
			5.053057711186465, 5.059427137938058, 5.065756251862108, 5.072045560038209, 
			5.078295560038209, 5.084506740162433, 5.090679579668605, 5.096814548993759, 
			5.102912109969369, 5.108972716029975, 5.114996812415517, 5.120984836367613, 
			5.126937217319994, 5.132854377083307, 5.138736730024484, 5.144584683240858, 
			5.150398636729230, 5.156178983550039, 5.161926109986821, 5.167640395701107, 
			5.173322213882925, 5.178971931397049, 5.184589908925139, 5.190176501103910, 
			5.195732056659465, 5.201256918537918, 5.206751424032424, 5.212215904906741, 
			5.217650687515437, 5.223056092920842, 5.228432437006863, 5.233780030589751, 
			5.239099179525921, 5.244390184816927, 5.249653342711664, 5.254888944805904, 
			5.260097278139238, 5.265278625289497, 5.270433264464755, 5.275561469592960, 
			5.280663510409286, 5.285739652541266, 5.290790157591771, 5.295815283219912,
			5.300815283219912, 5.305790407598021, 5.310740902647526, 5.315667011021910,
			5.320568971806224, 5.325447020586712, 5.330301389518751, 5.335132307393147, 
			5.339939999700839, 5.344724688696054, 5.349486593457959, 5.354225929950850, 
			5.358942911082926, 5.363637746763677, 5.368310643959938, 5.372961806750636, 
			5.377591436380266, 5.382199731311141, 5.386786887274444, 5.391353097320106, 
			5.395898551865561, 5.400423438743389, 5.404927943247893, 5.409412248180629, 
			5.413876533894914, 5.418320978339359, 5.422745757100421, 5.427151043444033, 
			5.431537008356314, 5.435903820583388, 5.440251646670345, 5.444580650999349, 
			5.448890995826935, 5.453182841320497, 5.457456345594002, 5.461711664742938, 
			5.465948952878531, 5.470168362161231, 5.474370042833500, 5.478554143251910, 
			5.482720809918577, 5.486870187511938, 5.491002418916897, 5.495117645254345, 
			5.499216005910083, 5.503297638563144, 5.507362679213551, 5.511411262209502, 
			5.515443520274018, 5.519459584531046, 5.523459584531046, 5.527443648276066, 
			5.531411902244320, 5.535364471414281, 5.539301479288297, 5.543223047915748
		};

	}

	PASTELSYS real64 digammaReal64(integer n)
	{
		// It holds that for integer parameters,
		// digamma(n) = H(n - 1) - y
		// where y is Euler-Mascheroni constant,
		// and H(n) is the n:th harmonic number:
		// H(n) = 1 / 1 + 1 / 2 + ... + 1 / n.
		//
		// From Wikipedia:
		// The harmonic number can be approximated
		// with:
		// H(n) = y + ln(n) + (1/2) n^-1 - (1/12) n^-2 + 
		//       (1/120) n^-4 + O(n^-6)
		//
		// Thus the digamma function can be approximated
		// with (note that the y is cancelled away):
		// digamma(n) = ln(n - 1) + (1/2) (n - 1)^-1 - (1/12) (n - 1)^-2 + 
		//       (1/120) (n - 1)^-4 + O(n^-6)
		//
		// Comparing the practical error of this formula with Maple,
		// It seems that that there is non-negligible
		// error in the beginning of this sequence.
		// Therefore we have computed the harmonic numbers
		// from 1 to 256 accurate to 16 decimals.
		// After this we use the approximation,
		// which is very accurate after this initial range.

		ENSURE_OP(n, >, 0);

		if (n < 257)
		{
			return digamma_[n];
		}
		
		const real64 nInv = (real)1 / (n - 1);
		const real64 nInv2 = square(nInv);

		// Add in increasing order for accuracy.

		real64 result = ((real)1 / 120) * square(nInv2);
		result -= ((real)1 / 12) * nInv2;
		result += ((real)1 / 2) * nInv;
		result += std::log((real)(n - 1));

		return result;
	}

	namespace
	{

		// lnFactorial[n] = ln(n!)
		// where ln is the natural logarithm.
		// Computed with Maple to 16 digits.
		
		real64 lnFactorial_[257] = {
			-infinity<real64>(),
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
			1150.633503306224, 1156.170837573242, 1161.712101118401, 1167.257278562880};
	
	}

	PASTELSYS real64 lnFactorialReal64(integer n)
	{
		PENSURE1(n >= 0 && n <= 256, n);
		
		return lnFactorial_[n];
	}

}