#include "PatchScript.hpp"
#include <iostream>
#include <fstream>
#ifdef __APPLE__
#include "filesystem.hpp"
#else
#include <experimental/filesystem>
#endif

using namespace patchscript;
int main(int argc, char** argv) {
	if(argc != 2) {
		std::cerr << "Usage: fsexporter <output-directory>" << std::endl;
		exit(1);
	}

	fs::path outDir(argv[1]);
	if (!fs::is_directory(outDir)) {
		std::cerr << "File: " << argv[1] << " is not a directory" << std::endl;
		exit(2);
	}

	PatchScript pscript;
	pscript.init();

	std::vector<PatchObject> list;
	pscript.listPatches(list);
	std::reverse(list.begin(), list.end());
	string lastName;
	for(const auto& po : list) {
		if(po.name_ != lastName) {
			fs::path outCode(po.name_ + ".pat");
			fs::path outMeta(po.name_ + ".met");

			fs::path outPathCode = outDir / outCode;
			fs::path outPathMeta = outDir / outMeta;

			//write code to pat file
			std::ofstream ofsCode(outPathCode.string());
			ofsCode << po.code_;
			ofsCode.close();

			//write meta data met file
			std::ofstream ofsMeta(outPathMeta.string());
			ofsMeta << "revision=" << po.revision_ << std::endl;
			ofsMeta << "runtimeName=" << po.runtimeName_ << std::endl;
			ofsMeta << "runtimeVersion=" << po.runtimeVersion_ << std::endl;
			ofsMeta << "description=" << po.description_ << std::endl;
			ofsMeta << "date=" << po.date_ << std::endl;
			ofsMeta << "layout=" << po.layout_ << std::endl;
			ofsMeta << "parameters=" << po.parameters_ << std::endl;
			ofsMeta << "keyboardBindings=" << po.keyboardBindings_ << std::endl;
			ofsMeta << "midiBindings=" << po.midiBindings_ << std::endl;
			ofsMeta.close();

			//prune the revisions file
			fs::path outRev(po.name_ + ".rev");
			fs::path outPathRev = outDir / outRev;
			std::ofstream ofsRev(outPathRev.string());
			ofsRev.close();
		} else {
			fs::path outRev(po.name_ + ".rev");
			fs::path outPathRev = outDir / outRev;

			//append each revision separated by "\x00\x10"
			std::ofstream ofsRev(outPathRev.string(), std::ios_base::app);
			ofsRev << "revision=" << po.revision_ << std::endl;
			ofsRev << "runtimeName=" << po.runtimeName_ << std::endl;
			ofsRev << "runtimeVersion=" << po.runtimeVersion_ << std::endl;
			ofsRev << "description=" << po.description_ << std::endl;
			ofsRev << "date=" << po.date_ << std::endl;
			ofsRev << "layout=" << po.layout_ << std::endl;
			ofsRev << "parameters=" << po.parameters_ << std::endl;
			ofsRev << "keyboardBindings=" << po.keyboardBindings_ << std::endl;
			ofsRev << "midiBindings=" << po.midiBindings_ << std::endl;
			ofsRev << po.code_ << (char)0 << std::endl;
		}
		lastName = po.name_;
	}

	return 0;
}
