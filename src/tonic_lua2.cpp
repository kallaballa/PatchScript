#include "tonic_lua.hpp"

using namespace Tonic;

std::vector<std::string> bindings2(kaguya::State& state) {
	std::vector<std::string> allow;

	allow.push_back("dBToLin");
	state["dBToLin"].setFunction(&dBToLin);

	allow.push_back("linTodB");
	state["linTodB"].setFunction(&linTodB);

	allow.push_back("Synth");
	state["Synth"].setClass(
			kaguya::UserdataMetatable<Synth>()
			.setConstructors<Synth()>()
			.addFunction("setLimitOutput", &Synth::setLimitOutput)
			.addFunction("setOutputGen", &Synth::setOutputGen)
			.addOverloadedFunctions("addParameter",
					(ControlParameter (Synth::*)(string))&Synth::addParameter,
						(ControlParameter (Synth::*)(string, TonicFloat))&Synth::addParameter,
						(void (Synth::*)(ControlParameter))&Synth::addParameter));

	allow.push_back("SampleTable");
	state["SampleTable"].setClass(
			kaguya::UserdataMetatable<SampleTable>()
			.setConstructors<SampleTable()>()
			.addFunction("channels", &SampleTable::channels)
			.addFunction("frames", &SampleTable::frames)
			.addFunction("size", &SampleTable::size)
			.addFunction("resize", &SampleTable::resize)
			.addFunction("resample", &SampleTable::resample)
			.addFunction("dataPointer", &SampleTable::dataPointer)
);

	allow.push_back("ControlGenerator");
	state["ControlGenerator"].setClass(make_control_operators(
			kaguya::UserdataMetatable<ControlGenerator>()
			.setConstructors<ControlGenerator()>()
			.addFunction("smoothed", &ControlGenerator::smoothed)
			.addFunction("tick", &ControlGenerator::tick))
		);

	allow.push_back("ControlAdder");
	state["ControlAdder"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlAdder, TemplatedControlGenerator<Tonic_::ControlAdder_>>()
			.setConstructors<ControlAdder()>()
			.addFunction("input", &ControlAdder::input)
			.addFunction("numInputs", &ControlAdder::numInputs)));

	allow.push_back("ControlSubtractor");
	state["ControlSubtractor"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlSubtractor, TemplatedControlGenerator<Tonic_::ControlSubtractor_>>()
			.setConstructors<ControlSubtractor()>()
			.addOverloadedFunctions("left",
					(ControlSubtractor& (ControlSubtractor::*)(float))&ControlSubtractor::left,
					(ControlSubtractor& (ControlSubtractor::*)(ControlGenerator))&ControlSubtractor::left)
			.addOverloadedFunctions("right",
					(ControlSubtractor& (ControlSubtractor::*)(float))&ControlSubtractor::right,
					(ControlSubtractor& (ControlSubtractor::*)(ControlGenerator))&ControlSubtractor::right)
			));

	allow.push_back("ControlMultiplier");
	state["ControlMultiplier"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlMultiplier, TemplatedControlGenerator<Tonic_::ControlMultiplier_>>()
			.setConstructors<ControlMultiplier()>()
			.addFunction("input", &ControlMultiplier::input)
			.addFunction("numInputs", &ControlMultiplier::numInputs)));

	allow.push_back("ControlDivider");
	state["ControlDivider"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlDivider, TemplatedControlGenerator<Tonic_::ControlDivider_>>()
			.setConstructors<ControlDivider()>()
			.addOverloadedFunctions("left",
					(ControlDivider& (ControlDivider::*)(float))&ControlDivider::left,
					(ControlDivider& (ControlDivider::*)(ControlGenerator))&ControlDivider::left)
			.addOverloadedFunctions("right",
					(ControlDivider& (ControlDivider::*)(float))&ControlDivider::right,
					(ControlDivider& (ControlDivider::*)(ControlGenerator))&ControlDivider::right)
			));

	allow.push_back("ControlParameter");
	state["ControlParameter"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlParameter, TemplatedControlGenerator<Tonic_::ControlParameter_>>()
			.setConstructors<ControlParameter()>()
			.addFunction("getName", &ControlParameter::getName)
			.addFunction("name", &ControlParameter::name)
			.addFunction("getDisplayName", &ControlParameter::getDisplayName)
			.addFunction("displayName", &ControlParameter::displayName)
			.addFunction("getValue", &ControlParameter::getValue)
			.addFunction("value", &ControlParameter::value)
			.addFunction("getMin", &ControlParameter::getMin)
			.addFunction("min", &ControlParameter::min)
			.addFunction("getMax", &ControlParameter::getMax)
			.addFunction("max", &ControlParameter::max)
			.addFunction("getParameterType", &ControlParameter::getParameterType)
			.addFunction("parameterType", &ControlParameter::parameterType)
			.addFunction("getIsLogarithmic", &ControlParameter::getIsLogarithmic)
			.addFunction("logarithmic", &ControlParameter::logarithmic)
			.addFunction("getNormalizedValue", &ControlParameter::getNormalizedValue)
			.addFunction("setNormalizedValue", &ControlParameter::setNormalizedValue)
));

	allow.push_back("ControlRandom");
	state["ControlRandom"].setClass(make_control(state,
				kaguya::UserdataMetatable<ControlRandom, TemplatedControlGenerator<Tonic_::ControlRandom_>>()
				.setConstructors<ControlRandom()>()
				.addOverloadedFunctions("max",
						(ControlRandom& (ControlRandom::*)(ControlGenerator))&ControlRandom::max,
						(ControlRandom& (ControlRandom::*)(float))&ControlRandom::max)
				.addOverloadedFunctions("min",
						(ControlRandom& (ControlRandom::*)(ControlGenerator))&ControlRandom::min,
						(ControlRandom& (ControlRandom::*)(float))&ControlRandom::min)
				.addOverloadedFunctions("trigger",
						(ControlRandom& (ControlRandom::*)(ControlGenerator))&ControlRandom::trigger,
						(ControlRandom& (ControlRandom::*)(float))&ControlRandom::trigger)
	));

	allow.push_back("ControlMetro");
	state["ControlMetro"].setClass(make_control(state,
				kaguya::UserdataMetatable<ControlMetro,TemplatedControlGenerator<Tonic_::ControlMetro_>>()
				.setConstructors<ControlMetro(), ControlMetro(float)>()
				.addOverloadedFunctions("bpm",
						(ControlMetro& (ControlMetro::*)(ControlGenerator))&ControlMetro::bpm,
						(ControlMetro& (ControlMetro::*)(float))&ControlMetro::bpm)
	));

	allow.push_back("ControlPulse");
	state["ControlPulse"].setClass(make_control(state,
				kaguya::UserdataMetatable<ControlPulse,TemplatedControlGenerator<Tonic_::ControlPulse_>>()
				.setConstructors<ControlPulse(),ControlPulse(float)>()
				.addOverloadedFunctions("trigger",
						(ControlPulse& (ControlPulse::*)(ControlGenerator))&ControlPulse::trigger,
						(ControlPulse& (ControlPulse::*)(float))&ControlPulse::trigger)
					.addOverloadedFunctions("length",
							(ControlPulse& (ControlPulse::*)(ControlGenerator))&ControlPulse::length,
							(ControlPulse& (ControlPulse::*)(float))&ControlPulse::length)));

	allow.push_back("ControlSnapToScale");
	state["ControlSnapToScale"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlSnapToScale,TemplatedControlGenerator<Tonic_::ControlSnapToScale_>>()
			.setConstructors<ControlSnapToScale()>()
			.addFunction("setScale", &ControlSnapToScale::setScale)
			.addOverloadedFunctions("input",
				(ControlSnapToScale (ControlSnapToScale::*)(float))&ControlSnapToScale::input,
				(ControlSnapToScale (ControlSnapToScale::*)(ControlGenerator))&ControlSnapToScale::input)));

	allow.push_back("ControlStepper");
	state["ControlStepper"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlStepper,TemplatedControlGenerator<Tonic_::ControlStepper_>>()
			.setConstructors<ControlStepper()>()
			.addOverloadedFunctions("start",
				(ControlStepper& (ControlStepper::*)(float))&ControlStepper::start,
				(ControlStepper& (ControlStepper::*)(ControlGenerator))&ControlStepper::start)
			.addOverloadedFunctions("end_",
				(ControlStepper& (ControlStepper::*)(float))&ControlStepper::end,
				(ControlStepper& (ControlStepper::*)(ControlGenerator))&ControlStepper::end)
			.addOverloadedFunctions("step",
				(ControlStepper& (ControlStepper::*)(float))&ControlStepper::step,
				(ControlStepper& (ControlStepper::*)(ControlGenerator))&ControlStepper::step)
			.addOverloadedFunctions("trigger",
				(ControlStepper& (ControlStepper::*)(float))&ControlStepper::trigger,
				(ControlStepper& (ControlStepper::*)(ControlGenerator))&ControlStepper::trigger)
			.addOverloadedFunctions("bidirectional",
				(ControlStepper& (ControlStepper::*)(float))&ControlStepper::bidirectional,
				(ControlStepper& (ControlStepper::*)(ControlGenerator))&ControlStepper::bidirectional)
	));

	allow.push_back("ControlSwitcher");
	state["ControlSwitcher"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlSwitcher,TemplatedControlGenerator<Tonic_::ControlSwitcher_>>()
			.setConstructors<ControlSwitcher()>()
			.addFunction("setFloatInputs", &ControlSwitcher::setFloatInputs)
			.addFunction("triggerForIndex", &ControlSwitcher::triggerForIndex)
			.addOverloadedFunctions("addInput",
				(ControlSwitcher& (ControlSwitcher::*)(float))&ControlSwitcher::addInput,
				(ControlSwitcher& (ControlSwitcher::*)(ControlGenerator))&ControlSwitcher::addInput)
			.addOverloadedFunctions("inputIndex",
				(ControlSwitcher& (ControlSwitcher::*)(float))&ControlSwitcher::inputIndex,
				(ControlSwitcher& (ControlSwitcher::*)(ControlGenerator))&ControlSwitcher::inputIndex)
			.addOverloadedFunctions("doesWrap",
				(ControlSwitcher& (ControlSwitcher::*)(float))&ControlSwitcher::doesWrap,
				(ControlSwitcher& (ControlSwitcher::*)(ControlGenerator))&ControlSwitcher::doesWrap)
			.addOverloadedFunctions("addAfterWrap",
				(ControlSwitcher& (ControlSwitcher::*)(float))&ControlSwitcher::addAfterWrap,
				(ControlSwitcher& (ControlSwitcher::*)(ControlGenerator))&ControlSwitcher::addAfterWrap)
	));

	allow.push_back("ControlValue");
	state["ControlValue"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlValue,TemplatedControlGenerator<Tonic_::ControlValue_>>()
			.setConstructors<ControlValue()>()
			.addFunction("value", &ControlValue::value)
			.addFunction("getValue", &ControlValue::getValue)
	));

	allow.push_back("ControlMidiToFreq");
	state["ControlMidiToFreq"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlMidiToFreq,TemplatedControlGenerator<Tonic_::ControlMidiToFreq_>>()
			.setConstructors<ControlMidiToFreq()>()
			.addOverloadedFunctions("input",
				(ControlMidiToFreq (ControlMidiToFreq::*)(float))&ControlMidiToFreq::input,
				(ControlMidiToFreq (ControlMidiToFreq::*)(ControlGenerator))&ControlMidiToFreq::input)));

	allow.push_back("ControlDbToLinear");
	state["ControlDbToLinear"].setClass(make_control_conditioner(state,
			kaguya::UserdataMetatable<ControlDbToLinear,TemplatedControlConditioner<Tonic::ControlDbToLinear, Tonic_::ControlDbToLinear_>>()
			.setConstructors<ControlDbToLinear()>()
			));

	allow.push_back("ControlPrinter");
	state["ControlPrinter"].setClass(make_control_conditioner(state,
			kaguya::UserdataMetatable<ControlPrinter,TemplatedControlConditioner<Tonic::ControlPrinter, Tonic_::ControlPrinter_>>()
			.setConstructors<ControlPrinter()>()
			.addFunction("message", &ControlPrinter::message)
			));

	return allow;
}
