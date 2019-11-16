#include "RevFileWriter.hpp"

namespace patchscript {

RevFileWriter::RevFileWriter(std::ostream& os) : out_(os) {
}

RevFileWriter::~RevFileWriter() {
}

void RevFileWriter::write(const SessionObject& po) {
		out_ << "revision=" << po.revision_ << '\n';
		out_ << "runtimeName=" << po.runtimeName_ << '\n';
		out_ << "runtimeVersion=" << po.runtimeVersion_ << '\n';
		out_ << "description=" << po.description_ << '\n';
		out_ << "date=" << po.date_ << '\n';
		out_ << "layout=" << po.layout_ << '\n';
		out_ << "parameters=" << po.parameters_ << '\n';
		out_ << "keyboardBindings=" << po.keyboardBindings_ << '\n';
		out_ << "midiBindings=" << po.midiBindings_ << '\n' << (char) 0;
		out_ << po.code_  << '\n' << (char) 0;
}

} /* namespace patchscript */
