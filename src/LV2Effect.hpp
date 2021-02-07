#ifndef LV2EFFECT_HPP_
#define LV2EFFECT_HPP_

#include "lilv-0/lilv/lilv.h"

#include "lv2.h"

#include "../third/Tonic/src/Tonic/Synth.h"
#include "../third/Tonic/src/Tonic/Effect.h"
#include "../third/Tonic/src/Tonic/ControlParameter.h"
#include <cassert>
/** Port type (only float ports are supported) */
typedef enum {
	TYPE_CONTROL,
	TYPE_AUDIO,
	TYPE_CV,
	TYPE_OTHER
} PortType;

/** Runtime port information */
struct Port {
	const LilvPort* lilv_port;  ///< Port description
	PortType        type;       ///< Datatype
	uint32_t        index;      ///< Port index
	float           value;      ///< Control value (if applicable)
	bool            is_input;   ///< True iff an input port
	bool            optional;   ///< True iff connection optional
};

/** Application state */
struct LV2Struct {
	static LilvWorld* world;
	const LilvPlugin* plugin;
	LilvInstance*     instance;
	unsigned          n_ports;
	unsigned          n_audio_in;
	unsigned          n_audio_out;
	unsigned          n_cv_in;
	unsigned          n_cv_out;
	Port*             ports;
};

/** Clean up all resources. */
static int
cleanup(int status, LV2Struct* self)
{
	lilv_instance_free(self->instance);
//	lilv_world_free(self->world);
	//free(self->ports);
	return status;
}

/** Print a fatal error and clean up for exit. */
static void
fatal(LV2Struct* self, int status, const char* fmt, ...)
{
	int s;
	if(self) {
		 s = cleanup(status, self);
	} else {
		s = status;
	}
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	sprintf(buffer, "error: ");
	vsprintf(buffer, fmt, args);
	va_end(args);
	throw runtime_error(buffer);
}

/**
   Create port structures from data (via create_port()) for all ports.
*/
static int create_ports(LV2Struct* self);

static inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

namespace Tonic {

  // -----------------------------------------
  //                 ADDER
  // -----------------------------------------

  namespace Tonic_{
  using namespace Tonic_;
  class LV2Effect_ : public WetDryEffect_ {

  protected:
  	std::map<string, ControlParameter> params;
    LV2Struct self = {
    		NULL, NULL, 0, 0, 0, 0, 0, NULL
    	};
  	float* in_buf = nullptr;
  	float* out_buf = nullptr;
  	float* incv_buf = nullptr;
  	float* outcv_buf = nullptr;
  	const LilvPlugin*  plugin = nullptr;

    void computeSynthesisBlock( const SynthesisContext_ &context );

  public:
    LV2Effect_() {
    	if(LV2Struct::world == nullptr) {
    		LV2Struct::world = lilv_world_new();
    		/* Discover world */
    		lilv_world_load_all(LV2Struct::world);
    	}
    }


    ~LV2Effect_() {
    	cleanup(0, &self);
    }

    void initialize(const char* pluginUri);
    void setControl(const char* name, ControlParameter p) {
    		params[name] = p;
    }

    std::vector<std::tuple<string, float, float, float>> getControls() {
    	std::vector<std::tuple<string, float, float, float>> controls;
  	 	for (uint32_t p = 0; p < lilv_plugin_get_num_ports(plugin); ++p) {
    		const LilvNode* sym = lilv_port_get_symbol(plugin, self.ports[p].lilv_port);
  	 		const char* symbol = lilv_node_as_string (sym);
    		const LilvPort* port  = lilv_plugin_get_port_by_symbol(plugin, sym);
    		LilvNode *min, *max, *def;
    		lilv_port_get_range(plugin, port, &def, &min, &max);

    		if (self.ports[p].type == TYPE_CONTROL && !(ends_with(string(symbol), string("_in")) || ends_with(string(symbol), string("_out")) || ends_with(string(symbol), string("_outL"))|| ends_with(string(symbol), string("_outR")) || ends_with(string(symbol), string("_inL")) || ends_with(string(symbol), string("_inR")))) {
    			controls.push_back({symbol, lilv_node_as_float(min), lilv_node_as_float(max), lilv_node_as_float(def)});
    		}

    		lilv_node_free(min);
    		lilv_node_free(max);
    		lilv_node_free(def);
  	 	}
  	 	return controls;
    }

    void setInput( Generator input );
    void setIsStereoInput(bool stereo);
  };



}
  class LV2Effect : public TemplatedWetDryEffect<LV2Effect, Tonic_::LV2Effect_>{
  public:
  		LV2Effect(const char* uri) {
  			gen()->initialize(uri);
  		}

      void setControl(const char* name, ControlParameter param) {
      	gen()->setControl(name,param);
      }

      std::vector<std::tuple<string, float, float, float>> getControls() {
      	return gen()->getControls();
      }

      void createParameters(const string& prefix, Synth s) {
      	auto controls = getControls();
      	string name;
      	for(const auto& t : controls) {
      		name = prefix + "." + std::get<0>(t);
      		auto cp = s.addParameter(name, std::get<3>(t), std::get<1>(t), std::get<2>(t));
      		setControl(std::get<0>(t).c_str(), cp);
      	}
      }
  };
}
#endif
