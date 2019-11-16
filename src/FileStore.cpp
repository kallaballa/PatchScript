#include "FileStore.hpp"
#include "PatFileWriter.hpp"
#include "PatFileReader.hpp"
#include "MetFileReader.hpp"
#include "MetFileWriter.hpp"
#include "RevFileWriter.hpp"
#include "RevFileReader.hpp"
#include <fstream>
#include <sstream>
#include <cassert>

namespace patchscript {

FileStore::FileStore(const string& strDirectory) : fsDir_(strDirectory) {
}

FileStore::~FileStore() {
}

void FileStore::store(std::vector<SessionObject> list) {
	std::reverse(list.begin(), list.end());
	string lastName;

	for (const auto& so : list) {
		fs::path outRev(so.name_ + ".rev");
		fs::path outPathRev = fsDir_ / outRev;
		std::ofstream ofsRev(outPathRev.string(), std::ios_base::app);
		RevFileWriter revWriter(ofsRev);

		if (so.name_ != lastName) {
			fs::path outCode(so.name_ + ".pat");
			fs::path outMeta(so.name_ + ".met");
			fs::path outPathCode = fsDir_ / outCode;
			fs::path outPathMeta = fsDir_ / outMeta;
			std::ofstream ofsPat(outPathCode.string());
			std::ofstream ofsMet(outPathMeta.string());
			PatFileWriter patWriter(ofsPat);
			MetFileWriter metWriter(ofsMet);

			//write code to pat file
			patWriter.write(so);

			//write meta data met file
			metWriter.write(so);
			//prune the revisions file
			std::ofstream ofsRev(outPathRev.string());
			ofsRev.close();
		} else {
			revWriter.write(so);
		}
		lastName = so.name_;
	}
}


void FileStore::retrieve(std::vector<SessionObject>& list) {
	for (const auto & entry : fs::directory_iterator(fsDir_)) {
		if(entry.path().extension().string()  == ".pat") {
			SessionObject so;
			so.name_ = entry.path().stem().string();

			fs::path codePath(entry.path());
			fs::path metaPath((entry.path().parent_path() / entry.path().stem()).string() + ".met");
			fs::path revPath((entry.path().parent_path() / entry.path().stem()).string() + ".rev");
			std::ifstream ifsPat(codePath.string());
			std::ifstream ifsMet(metaPath.string());
			std::ifstream ifsRev(revPath.string());
			PatFileReader patReader(ifsPat);
			MetFileReader metReader(ifsMet);
			RevFileReader revReader(so.name_, ifsRev);

			patReader.read(so);
			metReader.read(so);
			list.push_back(so);

			SessionObject soRev;
			while(revReader.read(soRev)) {
				list.push_back(soRev);
				soRev = SessionObject();
			}
		}
	}
}
} /* namespace patchscript */
