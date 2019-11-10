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
#include "PolySynth.hpp"

namespace kaguya {
class State;
}

namespace Tonic {
class Synth;
}


namespace patchscript {

class PatchScript {
	Tonic::Synth* synth_ = nullptr;
	PolySynth* poly_ = nullptr;
	kaguya::State* state = nullptr;
	std::vector<Tonic::Synth*> s;
public:
	PatchScript(size_t sampleRate);
	virtual ~PatchScript();
  void setErrorHandler(std::function<void(int,const char*)> errorfunction);
  bool init(const std::string& patchFile, const size_t& numVoices);
  void destroy();
  PolySynth* getPolySynth();
  void fill(float *outData,  unsigned int numFrames, unsigned int numChannels);
};

} /* namespace patchscript */

#endif /* SRC_PATCHSCRIPT_HPP_ */
