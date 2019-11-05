#include "tonic_lua.hpp"

using namespace Tonic;

void bindings0(kaguya::State& state) {
	state["Generator"].setClass(make_generator_operators(
			kaguya::UserdataMetatable<Generator>()
			.setConstructors<Generator()>()
			.addFunction("isStereoOutput", &Generator::isStereoOutput)
			.addFunction("tick", &Generator::tick)
					));

	state["Adder"].setClass(make_generator(state,
			kaguya::UserdataMetatable<Adder, TemplatedGenerator<Tonic_::Adder_>>()
			.setConstructors<Adder()>()
			.addFunction("input", &Adder::input)
			.addFunction("numInputs", &Adder::numInputs)));


	state["Subtractor"].setClass(make_generator(state,
			kaguya::UserdataMetatable<Subtractor, TemplatedGenerator<Tonic_::Subtractor_>>()
			.setConstructors<Subtractor()>()
			.addOverloadedFunctions("left",
					(Subtractor& (Subtractor::*)(Generator))&Subtractor::left,
					(Subtractor& (Subtractor::*)(float))&Subtractor::left,
					(Subtractor& (Subtractor::*)(ControlGenerator))&Subtractor::left)
			.addOverloadedFunctions("right",
					(Subtractor& (Subtractor::*)(Generator))&Subtractor::right,
					(Subtractor& (Subtractor::*)(float))&Subtractor::right,
					(Subtractor& (Subtractor::*)(ControlGenerator))&Subtractor::right)
			));

	state["Multiplier"].setClass(make_generator(state,
			kaguya::UserdataMetatable<Multiplier,TemplatedGenerator<Tonic_::Multiplier_>>()
			.setConstructors<Multiplier()>()
			.addFunction("input", &Multiplier::input)
			.addFunction("numInputs", &Multiplier::numInputs)));

	state["ControlMultiplier"].setClass(make_control(state,
			kaguya::UserdataMetatable<ControlMultiplier, TemplatedControlGenerator<Tonic_::ControlMultiplier_>>()
			.setConstructors<ControlMultiplier()>()
			.addFunction("input", &ControlMultiplier::input)
			.addFunction("numInputs", &ControlMultiplier::numInputs)));

	state["Divider"].setClass(make_generator(state,
				kaguya::UserdataMetatable<Divider, TemplatedGenerator<Tonic_::Divider_>>()
				.setConstructors<Divider()>()
				.addOverloadedFunctions("left",
						(Divider& (Divider::*)(Generator))&Divider::left,
						(Divider& (Divider::*)(float))&Divider::left,
						(Divider& (Divider::*)(ControlGenerator))&Divider::left)
				.addOverloadedFunctions("right",
						(Divider& (Divider::*)(Generator))&Divider::right,
						(Divider& (Divider::*)(float))&Divider::right,
						(Divider& (Divider::*)(ControlGenerator))&Divider::right)
			));

	state["FixedValue"].setClass(make_generator(state,
			kaguya::UserdataMetatable<FixedValue, TemplatedGenerator<Tonic_::FixedValue_>>()
			.setConstructors<FixedValue(), FixedValue(float)>()
			.addOverloadedFunctions("setValue",
			(FixedValue& (FixedValue::*)(float))&FixedValue::setValue,
			(FixedValue& (FixedValue::*)(ControlGenerator))&FixedValue::setValue)
	));

	state["RampedValue"].setClass(make_generator(state,
			kaguya::UserdataMetatable<RampedValue, TemplatedGenerator<Tonic_::RampedValue_>>()
			.setConstructors<RampedValue()>()
			.addFunction("isFinished", &RampedValue::isFinished)
			.addOverloadedFunctions("target",
					(RampedValue& (RampedValue::*)(float))&RampedValue::target,
					(RampedValue& (RampedValue::*)(ControlGenerator))&RampedValue::target)
			.addOverloadedFunctions("length",
					(RampedValue& (RampedValue::*)(float))&RampedValue::length,
					(RampedValue& (RampedValue::*)(ControlGenerator))&RampedValue::length)
			.addOverloadedFunctions("value",
					(RampedValue& (RampedValue::*)(float))&RampedValue::value,
					(RampedValue& (RampedValue::*)(ControlGenerator))&RampedValue::value)
			));

	state["SineWave"].setClass(make_generator(state,
			kaguya::UserdataMetatable<SineWave, TemplatedGenerator<Tonic_::TableLookupOsc_>>()
			.setConstructors<SineWave()>()
			.addOverloadedFunctions("freq",
					(SineWave& (SineWave::*)(Generator))&SineWave::freq,
					(SineWave& (SineWave::*)(float))&SineWave::freq,
					(SineWave& (SineWave::*)(ControlGenerator))&SineWave::freq)
				));

	state["RectWave"].setClass(make_generator(state,
			kaguya::UserdataMetatable<RectWave, TemplatedGenerator<Tonic_::RectWave_>>()
			.setConstructors<RectWave()>()
			.addOverloadedFunctions("freq",
					(RectWave& (RectWave::*)(Generator))&RectWave::freq,
					(RectWave& (RectWave::*)(float))&RectWave::freq,
					(RectWave& (RectWave::*)(ControlGenerator))&RectWave::freq)
			.addOverloadedFunctions("pwm",
					(RectWave& (RectWave::*)(Generator))&RectWave::pwm,
					(RectWave& (RectWave::*)(float))&RectWave::pwm,
					(RectWave& (RectWave::*)(ControlGenerator))&RectWave::pwm)
				));

	state["RectWaveBL"].setClass(make_generator(state,
			kaguya::UserdataMetatable<RectWaveBL, TemplatedGenerator<Tonic_::RectWaveBL_>>()
			.setConstructors<RectWaveBL()>()
			.addOverloadedFunctions("freq",
					(RectWaveBL& (RectWaveBL::*)(Generator))&RectWaveBL::freq,
					(RectWaveBL& (RectWaveBL::*)(float))&RectWaveBL::freq,
					(RectWaveBL& (RectWaveBL::*)(ControlGenerator))&RectWaveBL::freq)
			.addOverloadedFunctions("pwm",
					(RectWaveBL& (RectWaveBL::*)(Generator))&RectWaveBL::pwm,
					(RectWaveBL& (RectWaveBL::*)(float))&RectWaveBL::pwm,
					(RectWaveBL& (RectWaveBL::*)(ControlGenerator))&RectWaveBL::pwm)
	));

	state["SquareWave"].setClass(make_generator(state,
			kaguya::UserdataMetatable<SquareWave, TemplatedGenerator<Tonic_::RectWave_>>()
			.setConstructors<SquareWave()>()
			.addOverloadedFunctions("freq",
					(SquareWave& (SquareWave::*)(Generator))&SquareWave::freq,
					(SquareWave& (SquareWave::*)(float))&SquareWave::freq,
					(SquareWave& (SquareWave::*)(ControlGenerator))&SquareWave::freq)
	));

	state["TriangleWave"].setClass(make_generator(state,
			kaguya::UserdataMetatable<TriangleWave, TemplatedGenerator<Tonic_::AngularWave_>>()
			.setConstructors<TriangleWave()>()
			.addOverloadedFunctions("freq",
					(TriangleWave& (TriangleWave::*)(Generator))&TriangleWave::freq,
					(TriangleWave& (TriangleWave::*)(float))&TriangleWave::freq,
					(TriangleWave& (TriangleWave::*)(ControlGenerator))&TriangleWave::freq)
			.addOverloadedFunctions("slope",
					(TriangleWave& (TriangleWave::*)(Generator))&TriangleWave::slope,
					(TriangleWave& (TriangleWave::*)(float))&TriangleWave::slope,
					(TriangleWave& (TriangleWave::*)(ControlGenerator))&TriangleWave::slope)
));

	state["SquareWaveBL"].setClass(make_generator(state,
			kaguya::UserdataMetatable<SquareWaveBL, TemplatedGenerator<Tonic_::RectWaveBL_>>()
			.setConstructors<SquareWaveBL()>()
			.addOverloadedFunctions("freq",
					(SquareWaveBL& (SquareWaveBL::*)(Generator))&SquareWaveBL::freq,
					(SquareWaveBL& (SquareWaveBL::*)(float))&SquareWaveBL::freq,
					(SquareWaveBL& (SquareWaveBL::*)(ControlGenerator))&SquareWaveBL::freq)
	));
}
