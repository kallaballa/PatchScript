#include "PatchScript.hpp"
#include "Tonic.h"
#include "kaguya/kaguya.hpp"
#include "tonic_lua.hpp"

namespace patchscript {
using Tonic::Synth;

PatchScript::PatchScript() : state(new kaguya::State()) {
	bindings0(*state);
	bindings1(*state);
	bindings2(*state);
}

PatchScript::~PatchScript() {
}

void PatchScript::setErrorHandler(std::function<void(int,const char*)> errorfunction) {
	state->setErrorHandler(errorfunction);
}

bool PatchScript::init(const std::string& patchFile, const size_t& numVoices) {
	synth = new Synth();
	poly = new PolySynth();
	try {
		for (size_t i = 0; i < numVoices; ++i) {
			if (s[i] != nullptr) {
				delete (s[i]);
			}
			s[i] = new Synth();
			(*state)["synth"] = s[i];
			if (!state->dofile(patchFile)) {
				break;
			}
			poly->addVoice(*s[i]);
		}
	} catch (std::exception& e) {
		return false;
	}

	synth->setOutputGen(*poly);

	return !poly->getVoices().empty();
}

void PatchScript::destroy() {

}

PolySynth* PatchScript::getPolySynth() {
	return poly;
}


} /* namespace patchscript */
