#include "PatchScript.hpp"
#include "Tonic.h"
#include "kaguya/kaguya.hpp"
#include "tonic_lua.hpp"

namespace patchscript {
using Tonic::Synth;

PatchScript::PatchScript(size_t sampleRate) : state(new kaguya::State()) {
	Tonic::setSampleRate(sampleRate);
	bindings0(*state);
	bindings1(*state);
	bindings2(*state);
}

PatchScript::~PatchScript() {
	delete(state);
}

void PatchScript::setErrorHandler(std::function<void(int,const char*)> errorfunction) {
	state->setErrorHandler(errorfunction);
}

bool PatchScript::init(const std::string& patchFile, const size_t& numVoices) {
	synth_ = new Synth();
	poly_ = new PolySynth();
	try {
		for (size_t i = 0; i < numVoices; ++i) {
			Synth s;
			(*state)["synth"] = &s;
			if (!state->dofile(patchFile)) {
				break;
			}
			poly_->addVoice(s);
		}
	} catch (std::exception& e) {
		return false;
	}

	synth_->setOutputGen(*poly_);

	return !poly_->getVoices().empty();
}

void PatchScript::destroy() {
	delete (synth_);
	delete (poly_);
}

PolySynth* PatchScript::getPolySynth() {
	return poly_;
}


void PatchScript::fill(float *outData,  unsigned int numFrames, unsigned int numChannels) {
	synth_->fillBufferOfFloats(outData, numFrames, numChannels);
}
} /* namespace patchscript */
