//
//  ControlArithmetic.cpp
//  Tonic
//
//  Created by Nick Donaldson on 2/24/13.

//

#include "ControlArithmetic.h"

namespace Tonic {
ControlAdder operator+(float a, ControlGenerator b){
  ControlAdder adder;
  adder.input(ControlValue(a));
  adder.input(ControlGenerator(b));
  return adder;
}

ControlSubtractor operator-(float a, ControlGenerator b){
  ControlSubtractor subtractor;
  subtractor.left(ControlValue(a));
  subtractor.right(ControlGenerator(b));
  return subtractor;
}

ControlMultiplier operator*(float a, ControlGenerator b){
  ControlMultiplier mult;
  mult.input(ControlValue(a));
  mult.input(ControlGenerator(b));
  return mult;
}

	namespace Tonic_{
    
  void ControlAdder_::input(ControlGenerator input){
    inputs.push_back(input);
  }

  void ControlMultiplier_::input(ControlGenerator input){
    inputs.push_back(input);
  }
  
} // Namespace Tonic_

} // Namespace Tonic
