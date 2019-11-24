#include <cstdlib>
#include "defines.hpp"
#include "PatchScript.hpp"
#include "Tonic.h"
#include "kaguya/kaguya.hpp"
#include "tonic_lua.hpp"

namespace patchscript {
using Tonic::Synth;


PatchScript::PatchScript(size_t sampleRate) : state_(new kaguya::State()) {
	Tonic::setSampleRate(sampleRate);
	for(const auto& s : bindings0(*state_))
		whiteList_.push_back(s);
	for(const auto& s : bindings1(*state_))
		whiteList_.push_back(s);
	for(const auto& s : bindings2(*state_))
		whiteList_.push_back(s);
}

PatchScript::~PatchScript() {
	delete(state_);
}

std::pair<bool, string> PatchScript::checkHomeDir() {
	const char* home = std::getenv(DEFAULT_ENV_VARIABLE);
	if(home == nullptr) {
		return {false,(string("Can't find ") + DEFAULT_ENV_VARIABLE + " environment variable")};
	}
	config_.patchScriptDir_ = fs::path(string(home) + "/" + DEFAULT_HOME_DIR);
	config_.dataDir_ = fs::path(string(home) + "/" + DEFAULT_DATA_DIR);
	config_.logDir_ = fs::path(string(home) + "/" + DEFAULT_LOG_DIR);
	config_.dbFile_ = fs::path(string(home) + "/" + DEFAULT_DB_FILE);

	if(fs::exists(config_.patchScriptDir_)) {
		if(!fs::is_directory(config_.patchScriptDir_)) {
			return {false,(config_.patchScriptDir_.string() + " is not a directory")};
		}

		if(!(fs::exists(config_.dataDir_) && fs::is_directory(config_.dataDir_))) {
			return {false,(config_.dataDir_.string() + " doesn't exist or is not a directory")};
		}

		if(!(fs::exists(config_.logDir_) && fs::is_directory(config_.logDir_))) {
			return {false,(config_.logDir_.string() + " doesn't exist or is not a directory")};
		}
	} else {
		fs::create_directory(config_.patchScriptDir_);
		fs::create_directory(config_.dataDir_);
		fs::create_directory(config_.logDir_);
	}

	return {true, "Success"};
}

void PatchScript::setErrorHandler(std::function<void(int,const char*)> errorfunction) {
	state_->setErrorHandler(errorfunction);
}

std::pair<bool, string> PatchScript::init(const std::string& patchFile, const size_t& numVoices) {
	//protect the sprintf buffer
	if(patchFile.size() > 255)
		throw std::runtime_error("File name too long: " + patchFile.size());

	auto result = checkHomeDir();
	if(!result.first) {
		return result;
	}

	try {
		store_ = new SqlStore(config_.dbFile_);
	} catch (std::exception& ex) {
		return {false, ex.what()};
	}

	synth_ = new Synth();
	poly_ = new PolySynth();

	try {
		for (size_t i = 0; i < numVoices; ++i) {
			Synth s;
			(*state_)["synth"] = &s;


			std::ostringstream ss;
			ss << "{\n";

			for(size_t i = 0; i < whiteList_.size(); ++i) {
				const auto& s = whiteList_[i];

				ss << s << '=' << s;
				if(i < whiteList_.size() - 1)
					 ss << ',';
				ss << '\n';
			}

			ss << "}";

			std::string sandboxRead = R"__SANDBOX__(
				function readAll(file)
						local f = assert(io.open(file, "rb"))
						local content = f:read("*all")
						f:close()
						return content
				end

			function run_sandbox(synth)
					local content = readAll("%s")
					local wrapped = "function _patchScriptWrapper(synth)\n" .. content .. "\nend"
			    chunk = load(wrapped)
			    chunk()
			    debug.setupvalue(_patchScriptWrapper, 1, %s)
			    return _patchScriptWrapper(synth)
			end
			run_sandbox(synth)
		)__SANDBOX__";

			std::string whiteList = ss.str();
			char buffer[(sandboxRead.size() + whiteList.size() + patchFile.size())*2];
			sprintf(buffer, sandboxRead.c_str(), patchFile.c_str(), whiteList.c_str());

			if (!state_->dostring(buffer)) {
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
	if(synth_)
		delete (synth_);
	poly_->clearAllNotes();
	if(poly_)
		delete (poly_);
}

PolySynth* PatchScript::getPolySynth() {
	return poly_;
}

void PatchScript::list(std::vector<SessionObject>& list) {
	store_->list(list);
}

void PatchScript::store(const SessionObject& so) {
	store_->store(so);
}

void PatchScript::remove(const SessionObject& so) {
	store_->remove(so);
}

void PatchScript::fill(float *outData,  unsigned int numFrames, unsigned int numChannels) {
	synth_->fillBufferOfFloats(outData, numFrames, numChannels);
}
} /* namespace patchscript */
