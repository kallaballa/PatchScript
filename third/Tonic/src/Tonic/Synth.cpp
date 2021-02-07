//
//  Synth.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#include "Synth.h"
#include <sstream>

namespace Tonic {
	char specialChars[10] = {'-', '_', '.', '+', '!', '(', ')'};

	bool isValidControlParameterName(const string& n) {
		if(n.empty() || n.size() > 255)
			return false;

		for(const auto& c : n) {
			if((c >= '\x30' && c <= '\x39') || (c >= '\x41' && c <= '\x5A') || (c >= '\x61' && c <= '\x7A')) {
				continue;
			} else {
				bool found = false;
				for(const auto& s : specialChars) {
					if(c == s) {
						found = true;
						break;
					}
				}
				if(found)
					continue;
			}
			return false;
		}

		return true;
	}

  // Synth Factory
  SynthFactory::map_type * SynthFactory::map;

  namespace Tonic_ {
    
    Synth_::Synth_() : limitOutput_(true) {
      limiter_.setIsStereo(true);
    }

    void Synth_::setParameter(string name, float value, bool normalized){
      
      if (parameters_.find(name)!=parameters_.end()) {
        
        ControlParameter & param = parameters_[name];
        
        if (normalized){
          param.setNormalizedValue(value);
        }
        else{
          param.value(value);
        }
        
      }
      else{
        error("message: " + name + " was not registered. You can register a message using Synth::addParameter.");
      }

    }
    
    ControlParameter Synth_::addParameter(string name, TonicFloat initialValue, TonicFloat minValue, TonicFloat maxValue)
    {
    	if(!isValidControlParameterName(name)) {
    		throw std::invalid_argument("Illegal control parameter name: \"" + name + "\"");
    	}

    	if(name.find(".") == string::npos && name.at(0) != '_') {
    		name = "Global." + name;
    	}

    	if (parameters_.find(name) == parameters_.end())
      {
        ControlParameter param = ControlParameter().name(name).value(initialValue).min(minValue).max(maxValue).displayName(name);
        parameters_[name] = param;
        orderedParameterNames_.push_back(name);
      } else {
        error("Duplicate control parameter: " + name);
      }
      return parameters_[name];
    }
    
    void Synth_::addParameter(ControlParameter parameter){
      string name = parameter.getName();
      parameters_[name] = parameter;
      orderedParameterNames_.push_back(name);
    }
    
    void Synth_::addParametersFromSynth(Synth synth){
      vector<ControlParameter> params = synth.getParameters();
      for (unsigned int i=0; i<params.size(); i++)
        addParameter(params[i]);
    }
    
    vector<ControlParameter> Synth_::getParameters(){
      vector<ControlParameter> returnParams;
      for (std::vector<string>::iterator it = orderedParameterNames_.begin(); it != orderedParameterNames_.end(); it++){
        std::map<string, ControlParameter>::iterator paramIt = parameters_.find(*it);
        if (paramIt != parameters_.end()){
          returnParams.push_back(paramIt->second);
        }
      }
      return returnParams;
    }
    
    void Synth_::sendControlChangesToSubscribers(){
      vector<ControlChangeNotifier>::iterator it = controlChangeNotifiersList_.begin();
      for (; it != controlChangeNotifiersList_.end(); it++) {
        it->sendControlChangesToSubscribers();
      }
    }
    
    void Synth_::addControlChangeSubscriber(string name, ControlChangeSubscriber* resp){
      if(controlChangeNotifiers_.find(name) != controlChangeNotifiers_.end()){
        controlChangeNotifiers_[name].addValueChangedSubscriber(resp);
      }else{
        error("No value called " + name + " was exposed to the UI.");
      }
    }
    
    void Synth_::addControlChangeSubscriber(ControlChangeSubscriber* sub){
      for(vector<ControlChangeNotifier>::iterator it = controlChangeNotifiersList_.begin(); it != controlChangeNotifiersList_.end(); it++){
        it->addValueChangedSubscriber(sub);
      }
    }
    
    void Synth_::removeControlChangeSubscriber(ControlChangeSubscriber* sub){
      vector<ControlChangeNotifier>::iterator it = controlChangeNotifiersList_.begin();
      for (; it != controlChangeNotifiersList_.end(); it++) {
        it->removeValueChangedSubscriber(sub);
      }
    }
    
    ControlChangeNotifier Synth_::publishChanges(ControlGenerator input, string name){
      ControlChangeNotifier messenger;
      messenger.setName(name);
      messenger.input(input);
      controlChangeNotifiersList_.push_back(messenger);
      if(name != ""){
        controlChangeNotifiers_[name] = messenger;
      }
      addAuxControlGenerator(messenger);
      return messenger;
    }
    
  }
}
