#include "TonicCore.h"

namespace Tonic {
	void setSampleRate(TonicFloat sampleRate){
		Tonic_::sampleRate_ = sampleRate;
	}

  float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
  }

	void error(string message, bool fatal) {
		// maybe also log to console?
		printf("Tonic::error: %s\n", message.c_str());
		if (fatal) {
			throw TonicException(message);
		}
	}

	void warning(string message) {
		printf("Tonic::warning: %s\n", message.c_str());
	}

	void debug(string message) {
	#ifdef TONIC_DEBUG
		printf("Tonic::debug: %s\n", message.c_str());
	#endif
	}
}
