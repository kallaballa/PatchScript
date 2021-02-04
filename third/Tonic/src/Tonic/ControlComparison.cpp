//
//  ControlComparison.cpp
//  Tonic
//
//  Created by Nick Donaldson on 5/31/13.
//

#include "ControlComparison.h"

namespace Tonic { namespace Tonic_{
  
  ControlComparisonOperator_::ControlComparisonOperator_() :
    lhsGen_(ControlValue(0)),
    rhsGen_(ControlValue(0))
  {}
  
  ControlEquals operator == (ControlGenerator lhs, ControlGenerator rhs){
    ControlEquals c;
    c.setLeft(lhs);
    c.setRight(rhs);
    return c;
  }

  ControlEquals operator == (ControlGenerator lhs, TonicFloat right){
    ControlEquals c;
    c.setLeft(lhs);
    c.setRight(ControlValue(right));
    return c;
  }

  ControlNotEquals operator != (ControlGenerator lhs, ControlGenerator rhs){
    ControlNotEquals c;
    c.setLeft(lhs);
    c.setRight(rhs);
    return c;
  }

  ControlNotEquals operator != (ControlGenerator lhs, TonicFloat right){
    ControlNotEquals c;
    c.setLeft(lhs);
    c.setRight(ControlValue(right));
    return c;
  }

  ControlGreaterThan operator > (ControlGenerator lhs, ControlGenerator rhs){
    ControlGreaterThan c;
    c.setLeft(lhs);
    c.setRight(rhs);
    return c;
  }

  ControlGreaterThan operator > (ControlGenerator lhs, TonicFloat right){
    ControlGreaterThan c;
    c.setLeft(lhs);
    c.setRight(ControlValue(right));
    return c;
  }

  ControlGreaterOrEqual operator >= (ControlGenerator lhs, ControlGenerator rhs){
    ControlGreaterOrEqual c;
    c.setLeft(lhs);
    c.setRight(rhs);
    return c;
  }

  ControlGreaterOrEqual operator >= (ControlGenerator lhs, TonicFloat right){
    ControlGreaterOrEqual c;
    c.setLeft(lhs);
    c.setRight(ControlValue(right));
    return c;
  }

  ControlLessThan operator < (ControlGenerator lhs, ControlGenerator rhs){
    ControlLessThan c;
    c.setLeft(lhs);
    c.setRight(rhs);
    return c;
  }

  ControlLessThan operator < (ControlGenerator lhs, TonicFloat right){
    ControlLessThan c;
    c.setLeft(lhs);
    c.setRight(ControlValue(right));
    return c;
  }

  ControlLessOrEqual operator <= (ControlGenerator lhs, ControlGenerator rhs){
    ControlLessOrEqual c;
    c.setLeft(lhs);
    c.setRight(rhs);
    return c;
  }

  ControlLessOrEqual operator <= (ControlGenerator lhs, TonicFloat right){
    ControlLessOrEqual c;
    c.setLeft(lhs);
    c.setRight(ControlValue(right));
    return c;
  }


} // Namespace Tonic_
  
  
  
} // Namespace Tonic
