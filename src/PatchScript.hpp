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
namespace kaguya {
class State;
}

namespace patchscript {

class Synth;
class PolySynth;

class PatchScript {
	Synth* synth = nullptr;
	PolySynth* poly = nullptr;
	kaguya::State state;
	std::vector<Synth*> s;
public:
	PatchScript();
	virtual ~PatchScript();
  void setErrorHandler(std::function<void(int,const char*)> errorfunction) {
  	state.setErrorHandler(errorfunction);
  }

  void init(const std::string& patchFile, const size_t& numVoices);
  void destroy();
};

} /* namespace patchscript */

#endif /* SRC_PATCHSCRIPT_HPP_ */
