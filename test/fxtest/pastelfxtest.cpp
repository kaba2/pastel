#include "pastel/device/fxdevice.h"
#include "pastel/device/timedevice.h"
#include "pastel/numeric/constant.h"
#include "pastel/fx/mixer.h"
#include "pastel/fx/sinsynth.h"
#include "pastel/sys/countedptr.h"
#include "pastel/numeric/complextools.h"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;
using namespace Pastel;

const integer SAMPLE_RATE = 44100;
const integer SAMPLES = 8192;
const real ROW_LENGTH = 0.1;
const integer BLOCK_SIZE = ROW_LENGTH * SAMPLE_RATE;

real frequency = 200;
real volume = 1;
real noteFrequency[12];
const real NOTE = std::pow((real)2, (real)1/12);

enum Note
{
	C, CS, D, DS, E, F, FS, G, GS, A, AS, B
};

CountedPtr<SinSynth> sinSynthMachine(new SinSynth);
CountedPtr<SinSynth> sinSynthMachine2(new SinSynth);
CountedPtr<Mixer> mixerMachine(new Mixer);

void changeParameters();
void mixer(const ArrayView<int8>& data);

int main()
{
#include "pastel/signal/discretefourier.h"

#include <vector>
#include <complex>
#include <string>
#include <iostream>

using namespace std;
using namespace Pastel;

void testPlay();

int main()
{
	testPlay();
	return 0;
}

void testPlay()
{
	sinSynthMachine->initialize(
		SAMPLE_RATE,
		BLOCK_SIZE);
	sinSynthMachine2->initialize(
		SAMPLE_RATE,
		BLOCK_SIZE);

	mixerMachine->initialize(
		SAMPLE_RATE,
		BLOCK_SIZE);
	mixerMachine->setPreAttenuation(0.5);
	mixerMachine->connectInput(
		MachineRef(sinSynthMachine), 0, 0);
	mixerMachine->connectInput(
		MachineRef(sinSynthMachine2), 0, 1);

	for (integer i = 0;i < 12;++i)
	{
		noteFrequency[i] = std::pow(NOTE, i);
	}

	fxDevice().initialize(
		SAMPLE_RATE,
		1,
		FxDevice::SOUNDFORMAT_INT16_LE,
		SAMPLES,
		mixer);

	fxDevice().play();

	std::cout << "Now playing: Ukko Nooa" << std::endl;
	std::cout << "Paska biisi." << std::endl;
	std::cout << "Paina entteriä niin tuska loppuu." << std::endl;

	string tmp;
	getline(cin, tmp);

	fxDevice().stop();
	fxDevice().deInitialize();

	return 0;
}

void changeParameters()
{
	static Note noteData[] =
	{
		C, C, C, E, D, D, D, F, E, E, D, D, C, C, C, C,
		E, E, E, E, G, G, F, F, D, D, D, D, F, F, E, E,
		C, C, C, E, D, D, D, F, E, E, D, D, C, C, C, C
	};

	static integer noteCount = sizeof(noteData) / sizeof(Note);

	std::cout << ".";

	const real BASE = 300;

	static integer i = 0;

	if (i < noteCount * 4)
	{
		integer j = i & 3;
		if (j == 0)
		{
			frequency = BASE * noteFrequency[noteData[i/4]];
			volume = (real)i / (noteCount*4);
		}
		else if (j == 1)
		{
			volume = 0;
		}
		else if (j == 2)
		{
			frequency = BASE * noteFrequency[noteData[i/4]] / 2;
			volume = (real)i / (noteCount*4);
		}
		else
		{
			volume = 0;
		}
		++i;
	}
}

void mixer(const ArrayView<int8>& inputData)
{
	static ArrayView<float> floatData;
	static integer floatPosition = 0;
	static integer time = 0;

	ArrayView<int16> data(
		(int16*)&inputData[0], inputData.size() / 2);

	integer dataPosition = 0;

	while(dataPosition < data.size())
	{
		if (floatPosition == 0)
		{
			changeParameters();

			sinSynthMachine->setFrequency(frequency);
			sinSynthMachine->setVolume(volume);
			sinSynthMachine2->setFrequency(frequency * noteFrequency[G]);
			sinSynthMachine2->setVolume(volume);

			floatData = mixerMachine->getOutput(0, time);
			++time;
		}

		integer size = 0;

		if (!floatData.empty())
		{
		size = std::min(
			floatData.size() - floatPosition,
			data.size() - dataPosition);
			for (integer i = 0;i < size;++i)
			{
				data[dataPosition + i] =
					floatData[floatPosition + i] * 30000;
			}
		}
		else
		{
			size = data.size() - dataPosition;
			for (integer i = 0;i < size;++i)
			{
				data[dataPosition + i] = 0;
			}
		}

		floatPosition += size;
		dataPosition += size;

		if (floatPosition >= BLOCK_SIZE)
		{
			floatPosition = 0;
		}
	}

}
