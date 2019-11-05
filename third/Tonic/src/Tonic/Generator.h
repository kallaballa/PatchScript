//
//  Generator.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_GENERATOR_H
#define TONIC_GENERATOR_H

#include "TonicFrames.h"
#include "ControlGenerator.h"

#include <cmath>
namespace Tonic {

  namespace Tonic_{

    class Generator_{
      
    public:
      
      Generator_();
      virtual ~Generator_();
      
      virtual void tick( TonicFrames& frames, const SynthesisContext_ &context );
      
      bool isStereoOutput(){ return isStereoOutput_; };
      
      // set stereo/mono - changes number of channels in outputFrames_
      // subclasses should call in constructor to determine channel output
      virtual void setIsStereoOutput( bool stereo );
      
    protected:
      
      // override point for defining generator behavior
      // subclasses should implment to fill frames with new data
      virtual void computeSynthesisBlock( const SynthesisContext_ &context ) {};

      
      bool            isStereoOutput_;
      TonicFrames     outputFrames_;
      unsigned long   lastFrameIndex_;
      
    };
    
    inline void Generator_::tick(TonicFrames &frames, const SynthesisContext_ &context ){
      
      // check context to see if we need new frames
      if (context.forceNewOutput || lastFrameIndex_ != context.elapsedFrames){
        computeSynthesisBlock(context);
        lastFrameIndex_ = context.elapsedFrames;
      }
    
      // copy synthesis block to frames passed in
      frames.copy(outputFrames_);
      
    }

  }

  class Adder;
  class Subtractor;
  class Multiplier;
  class Divider;
  class Generator : public TonicSmartPointer<Tonic_::Generator_>{

  public:
    
    Generator( Tonic_::Generator_ * gen = new Tonic_::Generator_ ) : TonicSmartPointer<Tonic_::Generator_>(gen) {}
    
    inline bool isStereoOutput(){
      return obj->isStereoOutput();
    }
    
    virtual void tick(TonicFrames& frames, const Tonic_::SynthesisContext_ & context){
      obj->tick(frames, context);
    }

    Adder operator+ (Generator b);
    Adder operator+ (float b);
    Adder operator+ (ControlGenerator b);
    Subtractor operator- (Generator b);
    Subtractor operator- (float b);
    Subtractor operator- (ControlGenerator b);
    Multiplier operator*(Generator b);
    Multiplier operator*(float b);
    Multiplier operator* (ControlGenerator b);
    Divider operator/ (Generator b);
    Divider operator/ (float b);
    Divider operator/ (ControlGenerator b);
  };
  
  template<class GenType>
  class TemplatedGenerator : public Generator{
  protected:
    GenType* gen(){
      return static_cast<GenType*>(obj);
    }
  public:
    typedef GenType gen_t;
    TemplatedGenerator() : Generator(new GenType) {}
  };
  
}

// Each generator should have three flavors of setter -- one that accepts a float, one that accepts a
// ControlGenerator, and one that accepts a Generator. This macro will automatically build those three
// setters

#include "FixedValue.h"

#define TONIC_MAKE_GEN_SETTERS(generatorClassName, methodNameInGenerator, methodNameInGenerator_) \
                                                                                      \
  generatorClassName& methodNameInGenerator(Generator arg){ \
    this->gen()->methodNameInGenerator_(arg);                                           \
    return static_cast<generatorClassName&>(*this);                                     \
  }                                                                                     \
                                                                                        \
  generatorClassName& methodNameInGenerator(float arg){                                 \
    return methodNameInGenerator( FixedValue(arg) );                                    \
  }                                                                                     \
                                                                                        \
  generatorClassName& methodNameInGenerator(ControlGenerator arg){                      \
    return methodNameInGenerator(  FixedValue().setValue(arg) );                        \
  }



#endif
