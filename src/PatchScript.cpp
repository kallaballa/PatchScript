#include "PatchScript.hpp"
#include "Tonic.h"
#include "PolySynth.hpp"
#include "kaguya/kaguya.hpp"
#include "tonic_lua.hpp"

namespace patchscript {

PatchScript::PatchScript() {
	bindings0(state);
	bindings1(state);
	bindings2(state);
}

PatchScript::~PatchScript() {
	// TODO Auto-generated destructor stub
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
			state["synth"] = s[i];
			if (!state.dofile(patchFile)) {
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

} /* namespace patchscript */
