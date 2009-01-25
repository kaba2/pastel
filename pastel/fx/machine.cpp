#include "pastel/fx/machine.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	Machine::~Machine()
	{
	}

	void Machine::initialize(
		integer sampleRate,
		integer outputSize)
	{
		if (WARNING1(sampleRate <= 0, sampleRate) ||
			WARNING1(outputSize <= 0, outputSize))
		{
			return;
		}

		sampleRate_ = sampleRate;
		outputSize_ = outputSize;

		setChannels(channels_.size());
	}

	void Machine::loadSound(
		integer soundSlot,
		const ConstSoundRef& sound)
	{
		if (WARNING1(soundSlot < 0, soundSlot) ||
			WARNING2(soundSlot >= sounds_.size(),
			soundSlot, sounds_.size()))
		{
			return;
		}

		sounds_[soundSlot] = sound;
	}

	void Machine::unloadSound(integer soundSlot)
	{
		if (WARNING1(soundSlot < 0, soundSlot) ||
			WARNING2(soundSlot >= sounds_.size(),
			soundSlot, sounds_.size()))
		{
			return;
		}

		sounds_[soundSlot].clear();
	}

	ConstSoundRef Machine::getSound(integer soundSlot) const
	{
		if (WARNING1(soundSlot < 0, soundSlot) ||
			WARNING2(soundSlot >= sounds_.size(),
			soundSlot, sounds_.size()))
		{
			return ConstSoundRef();
		}

		return sounds_[soundSlot];
	}

	integer Machine::sampleRate() const
	{
		return sampleRate_;
	}

	integer Machine::outputSize() const
	{
		return outputSize_;
	}

	integer Machine::inputPorts() const
	{
		return inputs_.size();
	}

	integer Machine::outputPorts() const
	{
		return outputs_.size();
	}

	integer Machine::soundSlots() const
	{
		return sounds_.size();
	}

	integer Machine::channels() const
	{
		return channels_.size();
	}

	void Machine::connectInput(
		const MachineRef& that,
		integer thatOutputPort,
		integer inputPort)
	{
		if (WARNING1(thatOutputPort < 0, thatOutputPort) ||
			WARNING2(thatOutputPort >= that->outputPorts(),
			thatOutputPort, that->outputPorts()))
		{
			return;
		}

		if (WARNING1(inputPort < 0, inputPort) ||
			WARNING2(inputPort >= inputs_.size(),
			inputPort, inputs_.size()))
		{
			return;
		}

		InputPort& port = inputs_[inputPort];
		port.machine_ = that;
		port.machineOutputPort_ = thatOutputPort;
	}

	void Machine::disConnectInput(
		integer inputPort)
	{
		if (WARNING1(inputPort < 0, inputPort) ||
			WARNING2(inputPort >= inputs_.size(),
			inputPort, inputs_.size()))
		{
			return;
		}

		InputPort& port = inputs_[inputPort];
		port.machine_.clear();
		port.machineOutputPort_ = 0;
	}

	void Machine::toggleOutputPort(
		integer outputPort, bool isEnabled)
	{
		if (WARNING1(outputPort < 0, outputPort) ||
			WARNING2(outputPort >= outputs_.size(),
			outputPort, outputs_.size()))
		{
			return;
		}

		outputs_[outputPort].enabled_ = isEnabled;
	}

	bool Machine::outputPortEnabled(
		integer outputPort) const
	{
		if (WARNING1(outputPort < 0, outputPort) ||
			WARNING2(outputPort >= outputs_.size(),
			outputPort, outputs_.size()))
		{
			return false;
		}

		return outputs_[outputPort].enabled_;
	}

	void Machine::toggleChannel(
		integer channel, bool isEnabled)
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= outputs_.size(),
			channel, channels_.size()))
		{
			return;
		}

		channels_[channel].enabled_ = isEnabled;
	}

	bool Machine::channelEnabled(
		integer channel) const
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= channels_.size(),
			channel, channels_.size()))
		{
			return false;
		}

		return channels_[channel].enabled_;
	}

	MachineRef Machine::getInputMachine(integer inputPort)
	{

	}

	ArrayView<float> Machine::getOutput(
		integer channelId,
		integer updateId)
	{
		if (WARNING1(channelId < 0, channelId) ||
			WARNING2(channelId >= channels_.size(),
			channelId, channels_.size()))
		{
			return ArrayView<float>();
		}

		Channel& channel = outputs_[channelId];

		ArrayView<float> result;

		if (channel.enabled_ && channel.enabledInside_ &&
			channel.updateId_ != updateId)
		{
			result.set(
				&channel.data_.front(),
				channel.data_.size());

			channel.updateId_ = updateId;

			work(channelId, result, updateId);
		}

		return result;
	}

	// Protected

	Machine::Machine()
		: ReferenceCounted()
		, sounds_()
		, inputs_()
		, outputs_()
		, channels_()
		, sampleRate_(0)
		, outputSize_(0)
	{
	}

	Machine::Machine(const Machine& that)
		: ReferenceCounted()
		, sounds_()
		, inputs_()
		, outputs_()
		, channels_()
		, sampleRate_(0)
		, outputSize_(0)
	{
	}

	Machine& Machine::operator=(const Machine& that)
	{
		Machine copy(that);
		swap(copy);
		return *this;
	}

	void Machine::setInputPorts(integer inputPorts)
	{
		if (WARNING1(inputPorts < 0, inputPorts))
		{
			return;
		}

		inputs_.resize(inputPorts);
	}

	void Machine::setOutputPorts(integer outputPorts)
	{
		if (WARNING1(outputPorts < 0, outputPorts))
		{
			return;
		}

		outputs_.resize(outputPorts);
	}

	void Machine::setSoundSlots(integer soundSlots)
	{
		if (WARNING1(soundSlots < 0, soundSlots))
		{
			return;
		}

		sounds_.resize(soundSlots);
	}

	void Machine::setChannels(integer channels)
	{
		if (WARNING1(channels < 0, channels))
		{
			return;
		}

		channels_.resize(channels);

		for (integer i = 0;i < channels_.size();++i)
		{
			channels_[i].data_.resize(outputSize_);
		}
	}

	void Machine::toggleOutputPortInside(
		integer outputPort, bool isEnabled)
	{
		if (WARNING1(outputPort < 0, outputPort) ||
			WARNING2(outputPort >= outputs_.size(),
			outputPort, outputs_.size()))
		{
			return;
		}

		outputs_[outputPort].enabledInside_ = isEnabled;
	}

	void Machine::toggleChannelInside(
		integer channelId, bool isEnabled)
	{
		if (WARNING1(channelId < 0, channelId) ||
			WARNING2(channelId >= channels_.size(),
			channelId, channels_.size()))
		{
			return;
		}

		channels_[channelId].enabledInside_ = isEnabled;
	}

	ArrayView<float> Machine::getInput(
		integer inputPort,
		integer updateId) const
	{
		if (WARNING1(inputPort < 0, inputPort) ||
			WARNING2(inputPort >= inputs_.size(),
			inputPort, inputs_.size()))
		{
			return ArrayView<float>();
		}

		ArrayView<float> result;

		const InputPort& port = inputs_[inputPort];
		if (!port.machine_.empty())
		{
			result = port.machine_->getOutput(
				port.machineOutputPort_, updateId);
		}

		return result;
	}

}
