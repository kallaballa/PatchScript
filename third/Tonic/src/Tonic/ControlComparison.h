//
//  ControlComparison.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/31/13.
//
// See LICENSE.txt for license and usage information.
//

/**************************************************
 *
 *  For all comparison operators, only the lhs will
 *  drive new triggers at the output. This allows
 *  for trigger routing based on satisfying a
 *  condition.
 *
 *  Output is either 0 or 1.
 *
 ***************************************************/

#ifndef TONIC_CONTROLCOMPARISON_H
#define TONIC_CONTROLCOMPARISON_H

#include "ControlValue.h"

namespace Tonic {
  
  // --------------------------------------
  //   BASE COMPARISON CONTROLGENERATOR
  // --------------------------------------
  
  namespace Tonic_ {
    
    class ControlComparisonOperator_ : public ControlGenerator_ {
      
    protected:
            
      ControlGenerator lhsGen_;
      ControlGenerator rhsGen_;
      
      virtual bool satisfiesCondition( TonicFloat l, TonicFloat r) = 0;
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      
      ControlComparisonOperator_();
      
      void setLeft( ControlGenerator gen ) { lhsGen_ = gen; }
      void setRight( ControlGenerator gen ) { rhsGen_ = gen; }
      
    };
    
    inline void ControlComparisonOperator_::computeOutput(const SynthesisContext_ & context){
      
      output_.triggered = false;
      
      ControlGeneratorOutput lhsOut = lhsGen_.tick(context);
      ControlGeneratorOutput rhsOut = rhsGen_.tick(context);
      if (lhsOut.triggered){
        output_.value = satisfiesCondition(lhsOut.value, rhsOut.value) ? 1.0f : 0.0f;
        output_.triggered = true;
      }
      else{
        output_.value = 0;
      }
      
    }
    
  }
  
  template <class OperatorType>
  class TemplatedControlComparisonOperator : public TemplatedControlGenerator<OperatorType> {
    
  public:
    
    void setLeft( ControlGenerator gen ){
      this->gen()->setLeft(gen);
    }
    
    void setRight( ControlGenerator gen ){
      this->gen()->setRight(gen);
    }
    
  };
  
  // --------------------------------------
  //              EQUALITY
  // --------------------------------------
  
  namespace Tonic_ {
    
    class ControlEquals_ : public ControlComparisonOperator_ {
      
    protected:
      
      inline bool satisfiesCondition( TonicFloat l, TonicFloat r)
      {
        return l == r;
      }
      
    };
    
  }
  
  class ControlEquals : public TemplatedControlComparisonOperator<Tonic_::ControlEquals_> {};
  
  ControlEquals operator == (ControlGenerator lhs, ControlGenerator rhs);
  
  ControlEquals operator == (ControlGenerator lhs, TonicFloat right);
  // --------------------------------------
  //             INEQUALITY
  // --------------------------------------
  
  namespace Tonic_ {
    
    class ControlNotEquals_ : public ControlComparisonOperator_ {
      
    protected:
      
      inline bool satisfiesCondition( TonicFloat l, TonicFloat r)
      {
        return l != r;
      }
      
    };
  }
  
  class ControlNotEquals : public TemplatedControlComparisonOperator<Tonic_::ControlNotEquals_> {};
  
  ControlNotEquals operator != (ControlGenerator lhs, ControlGenerator rhs);
  ControlNotEquals operator != (ControlGenerator lhs, TonicFloat right);
  
  // --------------------------------------
  //            GREATER THAN
  // --------------------------------------
  
  namespace Tonic_ {

    class ControlGreaterThan_ : public ControlComparisonOperator_ {
      
    protected:
      
      inline bool satisfiesCondition( TonicFloat l, TonicFloat r)
      {
        return l > r;
      }
      
    };
    
  }
  
  class ControlGreaterThan : public TemplatedControlComparisonOperator<Tonic_::ControlGreaterThan_> {};
  
  ControlGreaterThan operator > (ControlGenerator lhs, ControlGenerator rhs);
  ControlGreaterThan operator > (ControlGenerator lhs, TonicFloat right);
  
  // --------------------------------------
  //          GREATER OR EQUAL
  // --------------------------------------
  
  namespace Tonic_ {
    
    class ControlGreaterOrEqual_ : public ControlComparisonOperator_ {
      
    protected:
      
      inline bool satisfiesCondition( TonicFloat l, TonicFloat r)
      {
        return l >= r;
      }
      
    };
    
  }
  
  class ControlGreaterOrEqual : public TemplatedControlComparisonOperator<Tonic_::ControlGreaterOrEqual_> {};
  
  ControlGreaterOrEqual operator >= (ControlGenerator lhs, ControlGenerator rhs);
  ControlGreaterOrEqual operator >= (ControlGenerator lhs, TonicFloat right);
  
  // --------------------------------------
  //              LESS THAN
  // --------------------------------------
  
  namespace Tonic_ {
    
    class ControlLessThan_ : public ControlComparisonOperator_ {
      
    protected:
      
      inline bool satisfiesCondition( TonicFloat l, TonicFloat r)
      {
        return l < r;
      }
      
    };
    
  }
  
  class ControlLessThan : public TemplatedControlComparisonOperator<Tonic_::ControlLessThan_> {};
  
  ControlLessThan operator < (ControlGenerator lhs, ControlGenerator rhs);
  ControlLessThan operator < (ControlGenerator lhs, TonicFloat right);
  
  // --------------------------------------
  //           LESS OR EQUAL
  // --------------------------------------
  
  namespace Tonic_ {
    
    class ControlLessOrEqual_ : public ControlComparisonOperator_ {
      
    protected:
      
      inline bool satisfiesCondition( TonicFloat l, TonicFloat r)
      {
        return l <= r;
      }
      
    };
    
  }
  
  class ControlLessOrEqual : public TemplatedControlComparisonOperator<Tonic_::ControlLessOrEqual_> {};
  
  ControlLessOrEqual operator <= (ControlGenerator lhs, ControlGenerator rhs);
  ControlLessOrEqual operator <= (ControlGenerator lhs, TonicFloat right);
}

#endif


