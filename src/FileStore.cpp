#include "FileStore.hpp"
#include <fstream>
#include <sstream>
#include <cassert>

namespace patchscript {

std::string readFully(std::ifstream& in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
     return sstr.str();
}

std::map<std::string, std::string> readKV(std::istream& in) {
	std::map<std::string, std::string> kv;
	std::string line;
	while (std::getline(in, line)) {
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, '=')) {
			std::string value;
			if (std::getline(is_line, value)) {
				kv[key] = value;
			}
		}
	}
	return kv;
}

FileStore::FileStore(const string& strDirectory) : fsDir_(strDirectory) {
}

FileStore::~FileStore() {
}

void FileStore::store(std::vector<PatchObject> list) {
	std::reverse(list.begin(), list.end());
	string lastName;

	for (const auto& po : list) {
		fs::path outRev(po.name_ + ".rev");
		fs::path outPathRev = fsDir_ / outRev;

		if (po.name_ != lastName) {
			fs::path outCode(po.name_ + ".pat");
			fs::path outMeta(po.name_ + ".met");

			fs::path outPathCode = fsDir_ / outCode;
			fs::path outPathMeta = fsDir_ / outMeta;

			//write code to pat file
			std::ofstream ofsCode(outPathCode.string());
			ofsCode << po.code_;
			ofsCode.close();

			//write meta data met file
			std::ofstream ofsMeta(outPathMeta.string());
			ofsMeta << "revision=" << po.revision_ << '\n';
			ofsMeta << "runtimeName=" << po.runtimeName_ << '\n';
			ofsMeta << "runtimeVersion=" << po.runtimeVersion_ << '\n';
			ofsMeta << "description=" << po.description_ << '\n';
			ofsMeta << "date=" << po.date_ << '\n';
			ofsMeta << "layout=" << po.layout_ << '\n';
			ofsMeta << "parameters=" << po.parameters_ << '\n';
			ofsMeta << "keyboardBindings=" << po.keyboardBindings_ << '\n';
			ofsMeta << "midiBindings=" << po.midiBindings_ << '\n';
			ofsMeta.close();

			//prune the revisions file
			std::ofstream ofsRev(outPathRev.string());
			ofsRev.close();
		} else {
			//append each revision meta data and code terminated by "\x00"
			std::ofstream ofsRev(outPathRev.string(), std::ios_base::app);
			ofsRev << "revision=" << po.revision_ << '\n';
			ofsRev << "runtimeName=" << po.runtimeName_ << '\n';
			ofsRev << "runtimeVersion=" << po.runtimeVersion_ << '\n';
			ofsRev << "description=" << po.description_ << '\n';
			ofsRev << "date=" << po.date_ << '\n';
			ofsRev << "layout=" << po.layout_ << '\n';
			ofsRev << "parameters=" << po.parameters_ << '\n';
			ofsRev << "keyboardBindings=" << po.keyboardBindings_ << '\n';
			ofsRev << "midiBindings=" << po.midiBindings_ << '\n' << (char) 0;
			ofsRev << po.code_  << '\n' << (char) 0;
		}
		lastName = po.name_;
	}
}


void FileStore::retrieve(std::vector<PatchObject>& list) {
	for (const auto & entry : fs::directory_iterator(fsDir_)) {
		if(entry.path().extension().string()  == ".pat") {
			PatchObject po;
			fs::path codePath(entry.path());
			fs::path metaPath((entry.path().parent_path() / entry.path().stem()).string() + ".met");
			fs::path revPath((entry.path().parent_path() / entry.path().stem()).string() + ".rev");
			std::ifstream codeIfs(codePath.string());
			std::ifstream metaIfs(metaPath.string());
			std::ifstream revIfs(revPath.string());

			po.name_ = entry.path().stem().string();
			po.code_ = readFully(codeIfs);
			auto metaMap = readKV(metaIfs);
			po.revision_ = std::atoll(metaMap["revision"].c_str());
			po.runtimeName_ = metaMap["runtimeName"];
			po.runtimeVersion_ = metaMap["runtimeVersion"];
			po.description_= metaMap["description"];
			po.date_ = std::atoll(metaMap["date"].c_str());
			po.layout_ = metaMap["layout"];
			po.parameters_ = metaMap["parameters"];
			po.keyboardBindings_ = metaMap["keyboardBindings"];
			po.midiBindings_= metaMap["midiBindings"];
			list.push_back(po);

			while(revIfs) {
				PatchObject poRev;
				string metaRev;
				if(!getline(revIfs, metaRev, (char)0))
					break;
				string codeRev;

				if(!getline(revIfs, codeRev, (char)0))
					break;
				poRev.name_ = po.name_;
				poRev.code_ = codeRev;

				std::istringstream metaRevIs(metaRev);
				auto metaRevMap = readKV(metaRevIs);
				poRev.revision_ = std::atoll(metaRevMap["revision"].c_str());
				poRev.runtimeName_ = metaRevMap["runtimeName"];
				poRev.runtimeVersion_ = metaRevMap["runtimeVersion"];
				poRev.description_= metaRevMap["description"];
				poRev.date_ = std::atoll(metaRevMap["date"].c_str());
				poRev.layout_ = metaRevMap["layout"];
				poRev.parameters_ = metaRevMap["parameters"];
				poRev.keyboardBindings_ = metaRevMap["keyboardBindings"];
				poRev.midiBindings_= metaRevMap["midiBindings"];
				list.push_back(poRev);
			}
		}
	}
}
} /* namespace patchscript */
