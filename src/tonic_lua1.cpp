#include "tonic_lua.hpp"
#include "lvplugin.hpp"

using namespace Tonic;

void bindings1(kaguya::State& state) {
	state["Noise"].setClass(make_generator(state,
				kaguya::UserdataMetatable<Noise, TemplatedGenerator<Tonic_::Noise_>>()
				.setConstructors<Noise()>()
				));

	state["HPF24"].setClass(make_generator(state,
			kaguya::UserdataMetatable<HPF24, TemplatedGenerator<Tonic_::HPF24_>>()
			.setConstructors<HPF24()>()
			.addFunction("normalizesGain", (HPF24& (HPF24::*)(bool))&HPF24::normalizesGain)
			.addFunction("input", (HPF24& (HPF24::*)(Generator))&HPF24::input)
			.addOverloadedFunctions("cutoff",
					(HPF24& (HPF24::*)(Generator))&HPF24::cutoff,
					(HPF24& (HPF24::*)(float))&HPF24::cutoff)
					.addOverloadedFunctions("Q",
							(HPF24& (HPF24::*)(Generator))&HPF24::Q,
							(HPF24& (HPF24::*)(float))&HPF24::Q)
));

	state["LPF12"].setClass(make_generator(state,
			kaguya::UserdataMetatable<LPF12, TemplatedGenerator<Tonic_::LPF12_>>()
			.setConstructors<LPF12()>()
			.addFunction("normalizesGain", (LPF12& (LPF12::*)(bool))&LPF12::normalizesGain)
			.addFunction("input", (LPF12& (LPF12::*)(Generator))&LPF12::input)
			.addOverloadedFunctions("cutoff",
					(LPF12& (LPF12::*)(Generator))&LPF12::cutoff,
					(LPF12& (LPF12::*)(float))&LPF12::cutoff)
					.addOverloadedFunctions("Q",
							(LPF12& (LPF12::*)(Generator))&LPF12::Q,
							(LPF12& (LPF12::*)(float))&LPF12::Q)
							));

	state["LPF24"].setClass(make_generator(state,
			kaguya::UserdataMetatable<LPF24, TemplatedGenerator<Tonic_::LPF24_>>()
			.setConstructors<LPF24()>()
			.addFunction("normalizesGain", (LPF24& (LPF24::*)(bool))&LPF24::normalizesGain)
			.addFunction("input", (LPF24& (LPF24::*)(Generator))&LPF24::input)
			.addOverloadedFunctions("cutoff",
					(LPF24& (LPF24::*)(Generator))&LPF24::cutoff,
					(LPF24& (LPF24::*)(float))&LPF24::cutoff)
					.addOverloadedFunctions("Q",
							(LPF24& (LPF24::*)(Generator))&LPF24::Q,
							(LPF24& (LPF24::*)(float))&LPF24::Q)
						));

	state["BPF12"].setClass(make_generator(state,
			kaguya::UserdataMetatable<BPF12, TemplatedGenerator<Tonic_::BPF12_>>()
			.setConstructors<BPF12()>()
			.addFunction("normalizesGain", (BPF12& (BPF12::*)(bool))&BPF12::normalizesGain)
			.addFunction("input", (BPF12& (BPF12::*)(Generator))&BPF12::input)
			.addOverloadedFunctions("cutoff",
					(BPF12& (BPF12::*)(Generator))&BPF12::cutoff,
					(BPF12& (BPF12::*)(float))&BPF12::cutoff)
					.addOverloadedFunctions("Q",
							(BPF12& (BPF12::*)(Generator))&BPF12::Q,
							(BPF12& (BPF12::*)(float))&BPF12::Q)
						));

	state["BPF24"].setClass(make_generator(state,
			kaguya::UserdataMetatable<BPF24, TemplatedGenerator<Tonic_::BPF24_>>()
			.setConstructors<BPF24()>()
			.addFunction("normalizesGain", (BPF24& (BPF24::*)(bool))&BPF24::normalizesGain)
			.addFunction("input", (BPF24& (BPF24::*)(Generator))&BPF24::input)
			.addOverloadedFunctions("cutoff",
					(BPF24& (BPF24::*)(Generator))&BPF24::cutoff,
					(BPF24& (BPF24::*)(float))&BPF24::cutoff)
					.addOverloadedFunctions("Q",
							(BPF24& (BPF24::*)(Generator))&BPF24::Q,
							(BPF24& (BPF24::*)(float))&BPF24::Q)
						));

	state["ADSR"].setClass(make_generator(state,
			kaguya::UserdataMetatable<ADSR, TemplatedGenerator<Tonic_::ADSR_>>()
			.setConstructors<ADSR()>()
					.addOverloadedFunctions("trigger",
					(ADSR& (ADSR::*)(float))&ADSR::trigger,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::trigger)
					.addOverloadedFunctions("attack",
					(ADSR& (ADSR::*)(float))&ADSR::attack,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::attack)
					.addOverloadedFunctions("decay",
					(ADSR& (ADSR::*)(float))&ADSR::decay,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::decay)
					.addOverloadedFunctions("sustain",
					(ADSR& (ADSR::*)(float))&ADSR::sustain,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::sustain)
					.addOverloadedFunctions("release",
					(ADSR& (ADSR::*)(float))&ADSR::release,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::release)
					.addOverloadedFunctions("exponential",
					(ADSR& (ADSR::*)(float))&ADSR::exponential,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::exponential)
					.addOverloadedFunctions("doesSustain",
					(ADSR& (ADSR::*)(float))&ADSR::doesSustain,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::doesSustain)
					.addOverloadedFunctions("legato",
					(ADSR& (ADSR::*)(float))&ADSR::legato,
					(ADSR& (ADSR::*)(ControlGenerator))&ADSR::legato)));

	state["Compressor"].setClass(make_effect(state,
			kaguya::UserdataMetatable<Compressor,TemplatedEffect<Tonic::Compressor, Tonic_::Compressor_>>()
			.setConstructors<Compressor()>()
			.addFunction("input", (Compressor& (Compressor::*)(Generator))&Compressor::input)
			.addFunction("audioInput", &Compressor::audioInput)
			.addFunction("sidechainInput", &Compressor::sidechainInput)
			.addFunction("setIsStereo", &Compressor::setIsStereo)
					.addOverloadedFunctions("attack",
							(Compressor& (Compressor::*)(float))&Compressor::attack,
							(Compressor& (Compressor::*)(ControlGenerator))&Compressor::attack)
					.addOverloadedFunctions("release",
							(Compressor& (Compressor::*)(float))&Compressor::release,
							(Compressor& (Compressor::*)(ControlGenerator))&Compressor::release)
					.addOverloadedFunctions("threshold",
							(Compressor& (Compressor::*)(float))&Compressor::threshold,
							(Compressor& (Compressor::*)(ControlGenerator))&Compressor::threshold)
					.addOverloadedFunctions("ratio",
							(Compressor& (Compressor::*)(float))&Compressor::ratio,
							(Compressor& (Compressor::*)(ControlGenerator))&Compressor::ratio)
					.addOverloadedFunctions("lookahead",
							(Compressor& (Compressor::*)(float))&Compressor::lookahead,
							(Compressor& (Compressor::*)(ControlGenerator))&Compressor::lookahead)
					.addOverloadedFunctions("makeupGain",
							(Compressor& (Compressor::*)(float))&Compressor::makeupGain,
							(Compressor& (Compressor::*)(ControlGenerator))&Compressor::makeupGain)));

	state["StereoDelay"].setClass(make_wet_dry(state,
			kaguya::UserdataMetatable<StereoDelay,TemplatedWetDryEffect<StereoDelay,Tonic_::StereoDelay_>>()
			.setConstructors<StereoDelay(float,float,float,float)>()
			.addOverloadedFunctions("feedback",
					(StereoDelay& (StereoDelay::*)(Generator))&StereoDelay::feedback,
					(StereoDelay& (StereoDelay::*)(float))&StereoDelay::feedback,
					(StereoDelay& (StereoDelay::*)(ControlGenerator))&StereoDelay::feedback)
			.addOverloadedFunctions("delayTimeLeft",
					(StereoDelay& (StereoDelay::*)(Generator))&StereoDelay::delayTimeLeft,
					(StereoDelay& (StereoDelay::*)(float))&StereoDelay::delayTimeLeft,
					(StereoDelay& (StereoDelay::*)(ControlGenerator))&StereoDelay::delayTimeLeft)
			.addOverloadedFunctions("delayTimeRight",
					(StereoDelay& (StereoDelay::*)(Generator))&StereoDelay::delayTimeRight,
					(StereoDelay& (StereoDelay::*)(float))&StereoDelay::delayTimeRight,
					(StereoDelay& (StereoDelay::*)(ControlGenerator))&StereoDelay::delayTimeRight)
					));

	state["Reverb"].setClass(make_wet_dry(state,
			kaguya::UserdataMetatable<Reverb,TemplatedWetDryEffect<Reverb,Tonic_::Reverb_>>()
			.setConstructors<Reverb()>()
			.addOverloadedFunctions("preDelayTime",
					(Reverb& (Reverb::*)(float))&Reverb::preDelayTime,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::preDelayTime)
			.addOverloadedFunctions("bypassInputFilter",
					(Reverb& (Reverb::*)(float))&Reverb::bypassInputFilter,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::bypassInputFilter)
			.addOverloadedFunctions("inputLPFCutoff",
					(Reverb& (Reverb::*)(float))&Reverb::inputLPFCutoff,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::inputLPFCutoff)
			.addOverloadedFunctions("inputHPFCutoff",
					(Reverb& (Reverb::*)(float))&Reverb::inputHPFCutoff,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::inputHPFCutoff)
			.addOverloadedFunctions("density",
					(Reverb& (Reverb::*)(float))&Reverb::density,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::density)
			.addOverloadedFunctions("roomShape",
					(Reverb& (Reverb::*)(float))&Reverb::roomShape,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::roomShape)
			.addOverloadedFunctions("roomSize",
					(Reverb& (Reverb::*)(float))&Reverb::roomSize,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::roomSize)
			.addOverloadedFunctions("decayTime",
					(Reverb& (Reverb::*)(float))&Reverb::decayTime,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::decayTime)
			.addOverloadedFunctions("decayLPFCutoff",
					(Reverb& (Reverb::*)(float))&Reverb::decayLPFCutoff,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::decayLPFCutoff)
			.addOverloadedFunctions("decayHPFCutoff",
					(Reverb& (Reverb::*)(float))&Reverb::decayHPFCutoff,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::decayHPFCutoff)
			.addOverloadedFunctions("stereoWidth",
					(Reverb& (Reverb::*)(float))&Reverb::stereoWidth,
					(Reverb& (Reverb::*)(ControlGenerator))&Reverb::stereoWidth)
					));

//	state["LV2Plugin"].setClass(make_wet_dry(state,
//			kaguya::UserdataMetatable<LV2Effect,TemplatedWetDryEffect<LV2Effect,Tonic_::LV2Effect_>>()
//			.setConstructors<LV2Effect(const char*)>()
//			.addFunction("setControl", &LV2Effect::setControl)
//			.addFunction("getControlNames", &LV2Effect::getControlNames)
//			.addFunction("getControlDefaults", &LV2Effect::getControlDefaults)
//					));

	state["BitCrusher"].setClass(make_effect(state,
			kaguya::UserdataMetatable<BitCrusher,TemplatedEffect<BitCrusher,Tonic_::BitCrusher_>>()
			.setConstructors<BitCrusher()>()
			.addOverloadedFunctions("bitDepth",
										(BitCrusher& (BitCrusher::*)(float))&BitCrusher::bitDepth,
										(BitCrusher& (BitCrusher::*)(ControlGenerator))&BitCrusher::bitDepth))
					);
}
