/*
 * PatchScript.hpp
 *
 *  Created on: Nov 9, 2019
 *      Author: elchaschab
 */

#ifndef SRC_PATCHSCRIPT_HPP_
#define SRC_PATCHSCRIPT_HPP_

#include <functional>
#include <vector>
#include <string>

#ifdef __APPLE__
#include "filesystem.hpp"
#else
#include <experimental/filesystem>
#endif

#include "PolySynth.hpp"
#include "SqlStore.hpp"
#include "defines.hpp"

namespace kaguya {
class State;
}

namespace Tonic {
class Synth;
}

namespace patchscript {

#ifdef __APPLE__
namespace fs = ghc::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

class PatchScript {
	struct Config {
		fs::path patchScriptDir_;
		fs::path configFile_;
		fs::path controlStateFile_;
		fs::path dataDir_;
		fs::path logDir_;
		fs::path dbFile_;
		fs::path patFile_;
	};

	Tonic::Synth* synth_ = nullptr;
	PolySynth* poly_ = nullptr;
	kaguya::State* state_ = nullptr;
	SqlStore* store_ = nullptr;
	std::vector<Tonic::Synth*> synthesizers_;
	Config config_;
	std::vector<std::string> whiteList_ = {
			"print",
			"pairs"
	};
	bool sandbox_ = true;
	std::pair<bool, string> checkHomeDir();
public:
	PatchScript(size_t sampleRate= 44100);
	virtual ~PatchScript();
	//has to be called before init
	void setSandbox(bool enable) {
		sandbox_ = enable;
	}
  void setErrorHandler(std::function<void(int,const char*)> errorfunction);
  std::pair<bool, string> init(const std::string& patchFile = "", const size_t& numVoices = 0);
  void destroy();
  PolySynth* getPolySynth();
  void list(std::vector<SessionObject>& sessions);
  void store(const SessionObject& so);
  void remove(const SessionObject& so);
  void fill(float *outData,  unsigned int numFrames, unsigned int numChannels);
};

} /* namespace patchscript */

#endif /* SRC_PATCHSCRIPT_HPP_ */
