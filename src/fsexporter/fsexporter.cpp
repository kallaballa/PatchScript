#include "PatchScript.hpp"
#include "FileStore.hpp"
#include <iostream>

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

	std::vector<SessionObject> storedSessions;
	pscript.list(storedSessions);

	FileStore store(outDir.string());
	store.store(storedSessions);


	return 0;
}
