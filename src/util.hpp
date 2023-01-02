#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <optional>

using namespace std;

namespace util {		

	auto findFirst = [](const auto& collection){
		return collection.size() > 0 ? make_optional(collection[0]) : nullopt;
	};

	auto isFileExtension = [](const string& fileextension){
		auto firstChar = findFirst(fileextension);
		return firstChar.has_value() && firstChar.value() == '.';
	};

	auto split = [](const string& text){
		stringstream sstream(text);
		string tmp;
		vector<string> out;

		while (getline(sstream, tmp, ' ')) {
			out.push_back(tmp);
		}

		return out;
	};
	
}

#endif