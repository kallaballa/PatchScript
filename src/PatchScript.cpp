#include <cstdlib>
#include "defines.hpp"
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

std::pair<bool, string> PatchScript::checkHomeDir() {
	const char* home = std::getenv(DEFAULT_ENV_VARIABLE);
	if(home == nullptr) {
		return {false,(string("Can't find ") + DEFAULT_ENV_VARIABLE + " environment variable")};
	}
	config.patchScriptDir_ = fs::path(string(home) + "/" + DEFAULT_HOME_DIR);
	config.dataDir_ = fs::path(string(home) + "/" + DEFAULT_DATA_DIR);
	config.logDir_ = fs::path(string(home) + "/" + DEFAULT_LOG_DIR);

	if(fs::exists(config.patchScriptDir_)) {
		if(!fs::is_directory(config.patchScriptDir_)) {
			return {false,(config.patchScriptDir_.string() + " is not a directory")};
		}

		if(!(fs::exists(config.dataDir_) && fs::is_directory(config.dataDir_))) {
			return {false,(config.dataDir_.string() + " doesn't exist or is not a directory")};
		}

		if(!(fs::exists(config.logDir_) && fs::is_directory(config.logDir_))) {
			return {false,(config.logDir_.string() + " doesn't exist or is not a directory")};
		}
	} else {
		fs::create_directory(config.patchScriptDir_);
		fs::create_directory(config.dataDir_);
		fs::create_directory(config.logDir_);
	}

	return {true, "Success"};
}

void PatchScript::setErrorHandler(std::function<void(int,const char*)> errorfunction) {
	state->setErrorHandler(errorfunction);
}

std::pair<bool, string> PatchScript::init(const std::string& patchFile, const size_t& numVoices) {
	auto result = checkHomeDir();
	if(!result.first) {
		return result;
	}

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
		return {false, e.what()};
	}

	synth_->setOutputGen(*poly_);

	return {true, "Success"};
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
