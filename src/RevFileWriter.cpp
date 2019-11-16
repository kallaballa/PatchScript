#include "RevFileWriter.hpp"

namespace patchscript {

RevFileWriter::RevFileWriter(std::ostream& os) : out_(os) {
}

RevFileWriter::~RevFileWriter() {
}

void RevFileWriter::write(const SessionObject& so) {
	out_ << "author=" << so.author_ << '\n';
	out_ << "revision=" << so.revision_ << '\n';
	out_ << "runtimeName=" << so.runtimeName_ << '\n';
	out_ << "runtimeVersion=" << so.runtimeVersion_ << '\n';
	out_ << "description=" << so.description_ << '\n';
	out_ << "date=" << so.date_ << '\n';
	out_ << "layout=" << so.layout_ << '\n';
	out_ << "parameters=" << so.parameters_ << '\n';
	out_ << "keyboardBindings=" << so.keyboardBindings_ << '\n';
	out_ << "midiBindings=" << so.midiBindings_ << '\n' << (char) 0;
	out_ << so.code_ << '\n' << (char) 0;
}

} /* namespace patchscript */
