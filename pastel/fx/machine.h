#ifndef PASTEL_MACHINE_H
#define PASTEL_MACHINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/fx/fxlibrary.h"
#include "pastel/sys/arrayview.h"
#include "pastel/fx/sound.h"
#include "pastel/sys/countedptr.h"
#include "pastel/sys/smallmap.h"

#include <vector>

namespace Pastel
{

	class PASTELFX Machine;

	typedef CountedPtr<Machine> MachineRef;
	typedef CountedPtr<const Machine> ConstMachineRef;

	class PASTELFX Machine
		: public ReferenceCounted
	{
	public:
		virtual ~Machine();

		void initialize(
			integer sampleRate,
			integer outputSize);

		integer createOutput();
		void removeOutput(integer outputId);

		void loadSound(integer soundSlot,
			const ConstSoundRef& sound);
		void unloadSound(integer soundSlot);

		ConstSoundRef getSound(integer soundSlot) const;

		integer sampleRate() const;
		integer outputSize() const;

		integer inputPorts() const;
		integer outputIds() const;
		integer soundSlots() const;
		integer channels() const;

		void connectInput(
			const MachineRef& that,
			integer thatOutput,
			integer inputPort);

		void disConnectInput(
			integer inputPort);

		void toggleOutput(integer outputId, bool isEnabled);
		bool outputIdEnabled(integer outputId) const;

		void toggleChannel(integer channel, bool isEnabled);
		bool channelEnabled(integer channel) const;

		MachineRef getInputMachine(integer inputPort);
		ArrayView<const integer> getInputChannels(integer inputPort);

		ArrayView<float> getOutput(
			integer channel,
			integer updateId);

	protected:
		Machine();
		Machine(const Machine& that);
		Machine& operator=(const Machine& that);

		void swap(Machine& that);

		integer createInput();
		void removeInput(integer inputId);

		void toggleOutputInside(
			integer outputId, bool isEnabledInside);

		void toggleChannelInside(
			integer channel, bool isEnabledInside);

		ArrayView<float> getInput(
			integer inputPort,
			integer updateId) const;

	private:
		virtual void work(
			integer outputId,
			const ArrayView<float>& output,
			integer updateId) = 0;

		struct Channel
			: public ReferenceCounted
		{
			Channel()
				: data_()
				, updateId_(0)
				, enabled_(true)
				, enabledInside_(true)
			{
			}

			std::vector<float> data_;
			integer updateId_;
			bool enabled_;
			bool enabledInside_;
		};

		typedef CountedPtr<Channel> ChannelRef;
		typedef CountedPtr<const Channel> ConstChannelRef;

		struct Output
			: public ReferenceCounted
		{
			Output()
				: channels_()
				, updatedId_(0)
				, enabled_(true)
				, enabledInside_(true)
			{
			}

			std::vector<integer> channels_;
			integer updateId_;
			bool enabled_;
			bool enabledInside_;
		};

		typedef CountedPtr<Output> OutputRef;
		typedef CountedPtr<const Output> ConstOutputRef;

		struct Input
			: public ReferenceCounted
		{
			Input()
				: machine_()
				, machineOutput_(0)
			{
			}

			MachineRef machine_;
			integer machineOutput_;
		};

		typedef CountedPtr<Input> InputRef;
		typedef CountedPtr<const Input> ConstInputRef;

		SmallMap<integer, OutputRef> outputs_;
		SmallMap<integer, InputRef> inputs_;
		SmallMap<integer, ConstSoundRef> sounds_;
		SmallMap<integer, ChannelRef> channels_;

		integer sampleRate_;
		integer outputSize_;
	};

}

#endif
